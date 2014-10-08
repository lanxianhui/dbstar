package com.dbstar.app;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;

import com.dbstar.R;
import com.dbstar.app.adapter.GalleryAdapter;
import com.dbstar.app.components.FlashGallery;
import com.dbstar.app.launcher.GDLauncherActivity;
import com.dbstar.bean.ImageSet;
import com.dbstar.bean.QueryPoster;
import com.dbstar.bean.QueryPosterData;
import com.dbstar.bean.QueryRecommand;
import com.dbstar.bean.QueryRecommandData;
import com.dbstar.http.HttpConnect;
import com.dbstar.http.SimpleWorkPool.ConnectWork;
import com.dbstar.http.SimpleWorkPool.ReadSDCardData;
import com.dbstar.http.SimpleWorkPool.SimpleWorkPoolInstance;
import com.dbstar.service.DeviceInitController;
import com.dbstar.service.GDDataProviderService;
import com.dbstar.util.Constants;
import com.dbstar.util.DbstarUtil;
import com.dbstar.util.LogUtil;

public class DbstarOTTActivity extends Activity {

	public static final String ColumnIDCNTV = "L97";
	
	private static final int Btn_StarTV_Sequence = 10;
	private static final int Btn_CntvTV_Sequence = 11;
	private static final int Btn_AppShop_Sequence = 12;
	private static final int Btn_MyApp_Sequence = 13;
	private static final int Btn_Setting_Sequence = 2;
	
	private static int Ethernet_Network_Mode = 0;
	private static final String Ethernet_Mode = "ethernet_mode";
	
	private FlashGallery gallery;
	private ImageView imgStarTV;
	private ImageView imgCntv;
	private ImageView imgAppShop;
	private ImageView imgMyApp;
	private ImageView imgSetting;
	
	private TextView txtStarTV;
	private TextView txtCntv;
	private TextView txtAppShop;
	private TextView txtMyApp;
	private TextView txtSetting;
	
	private int index = 0;
	private ImageSet mImageSet;
	private Integer[] pictures = {R.drawable.main_img1, R.drawable.main_img2, R.drawable.main_img3, R.drawable.main_img4, R.drawable.main_img5};
	
	private GalleryAdapter adapter2;
	private boolean isToAthorAvtivity = false;
	private boolean timerTag = false;
	
	private Intent intentSer; 
	private GalleryTask task;
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case 2:
				adapter2.notifyDataSetChanged();
				gallery.setSelection(index);
				gallery.setFocusable(false);
				break;
			case 3:
				LogUtil.d("Intent Service", "跨进程调用intentSettings========" + new Intent("com.settings.service.action.OTTSettingsService").toString());
				startService(new Intent("com.settings.service.action.OTTSettingsService"));
				
				timerTag = true;
				
				//  每次进入程序15s后，都要看一下是否注册及登录。
				DbstarUtil.login(DbstarOTTActivity.this);
				break;
			default:
				break;
			}
		};
	};
	
	class GalleryTask extends TimerTask {

		@Override
		public void run() {
			Message message = new Message();
			message.what = 2;
			index = gallery.getSelectedItemPosition();
			index++;
			handler.sendMessage(message);
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// 设置成无标题
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.lt_dbstar_main_luncher_flash);
		
		getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);
		getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

		mImageSet = new ImageSet(adapter2);
		// 检查联网情况
		boolean isNetworkConnected = DbstarUtil.isNetworkAvailable(this);
		//  先判断是否联网
		if (!isNetworkConnected) {
			//  如果没有网络，则读取文件中的数据，如果没有文件，则展现pictures里面的图片
			readQueryPosterFromSDCard();
			readQueryRecommandFromSDCard();
		} else {
			getQueryPoster();
			getQueryRecommand();
		}
		
		String defaultStorage = "/storage/external_storage/sda1/";
		File file = new File(defaultStorage);
		if (file.exists()){
			LogUtil.d("DbstarOTTActivity", " before wait time = " + System.currentTimeMillis());
			LogUtil.d("DbstarOTTActivity", " file.canWrite() = " + file.canWrite());
			if (!file.canWrite()) {				
				synchronized (this) {			
					for (int i = 0; i < 10; i++){
						try {
							
							wait(1000);
							
							if (file.canWrite()) {
								LogUtil.d("DbstarOTTActivity", "storage " + defaultStorage + "is ready at loop " + i);
								break;
							} else {
								LogUtil.d("DbstarOTTActivity", "storage " + defaultStorage + "is not ready at loop " + i);
							}
						} catch (InterruptedException e) {							
							LogUtil.d("DbstarOTTActivity", " wait is interrupted");
							e.printStackTrace();
						}
					}
				}
			}
			LogUtil.d("DbstarOTTActivity", " after wait time = " + System.currentTimeMillis());
		}
		
		intentSer = new Intent();
		intentSer.setClass(this, GDDataProviderService.class);
		startService(intentSer);
		LogUtil.d("Intent Service", "跨进程调用service========" + intentSer.toString());
		
		startService(new Intent(this, DbstarService.class));
		
		findViews();
		imgStarTV.requestFocus();
		populateData();
		setEventListener();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		stopService(intentSer);
		stopService(new Intent(this, DbstarService.class));
//		stopService(new Intent("com.settings.service.action.OTTSettingsService"));
	}

	private void readQueryPosterFromSDCard() {
		// 读取海报
		String filePath = "/data/dbstar/posterData1.txt";
		ReadSDCardData<HashMap<String, String>> readWork = new ReadSDCardData<HashMap<String,String>>(filePath) {
			
			@Override
			public HashMap<String, String> processResult(HashMap<String, String> hashMap) {
				return hashMap;
			}
			
			@Override
			public void connectComplete(HashMap<String, String> hashMap) {
					for (int i = 0; i < hashMap.size(); i++) {
						String iconUrl = hashMap.get((i + 1) + "");
						final String urlPath = Constants.Server_Url_Image + iconUrl;
						
						ConnectWork<Bitmap> connectWork = new ConnectWork<Bitmap>(HttpConnect.GET, urlPath, null) {

							@Override
							public Bitmap processResult(HttpEntity entity) {
								return getBitmapFromEntity(entity);
							}

							@Override
							public void connectComplete(Bitmap bitmap) {
								if (bitmap != null) {
									mImageSet.add(urlPath, null, bitmap);
									adapter2.add(urlPath, null, bitmap);
								}
							}
						};
						SimpleWorkPoolInstance.instance().execute(connectWork);
					}
			}
		};
		SimpleWorkPoolInstance.instance().execute(readWork);
	}
	
	private void readQueryRecommandFromSDCard() {
		// 读取推荐位
		String filePath = "/data/dbstar/queryRecommand1.txt";
		ReadSDCardData<HashMap<String, String>> readWork = new ReadSDCardData<HashMap<String,String>>(filePath) {
			
			@Override
			public HashMap<String, String> processResult(HashMap<String, String> hashMap) {
				return hashMap;
			}
			
			@Override
			public void connectComplete(HashMap<String, String> hashMap) {
				// hashMap.size()/3是因为每个sequence对应一个name，而每个name则对应一个url,而每个url又对应一个linkedUrl
				for (int i = 0; i < hashMap.size() / 3; i++) {
					// 根据sequence得到name
					final String name = hashMap.get((i + 1) + "");
					String iconUrl = hashMap.get(name);
					final int sequence = i + 1;
					
					final String urlPath = Constants.Server_Url_Image + iconUrl;
					String linkUrl = hashMap.get(iconUrl);
					String[] split = linkUrl.split(":");
					String packageName = split[0];
					LogUtil.i("packageName", packageName);

					ConnectWork<Bitmap> connectWork = new ConnectWork<Bitmap>(HttpConnect.GET, urlPath, null) {

						@Override
						public Bitmap processResult(HttpEntity entity) {
							return getBitmapFromEntity(entity);
						}

						@Override
						public void connectComplete(Bitmap bitmap) {
							if (bitmap != null) {
								if (sequence == 1) {
									imgStarTV.setImageBitmap(bitmap);
								} else if (sequence == 2) {
									imgCntv.setImageBitmap(bitmap);
								} else if (sequence == 3) {
									imgAppShop.setImageBitmap(bitmap);
								} else if (sequence == 4) {
									imgMyApp.setImageBitmap(bitmap);
								} else {
									imgSetting.setImageBitmap(bitmap);
								}
							}
						}
					};
					SimpleWorkPoolInstance.instance().execute(connectWork);
					
					populateRecommondNameAndPackageName(sequence, name, packageName);
				}
			}
		};
		SimpleWorkPoolInstance.instance().execute(readWork);
	}

	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		LogUtil.d(getClass().getName(), "<<<<<<///onActivityResult///>>>>>>>>" + data);
		if (requestCode == Btn_CntvTV_Sequence) {
			imgCntv.requestFocus();
		} else if (requestCode == Btn_AppShop_Sequence) {
			imgAppShop.requestFocus();
		} else if (requestCode == Btn_MyApp_Sequence) {
			imgMyApp.requestFocus();
		} else if (requestCode == Btn_Setting_Sequence) {
			LogUtil.d(getClass().getName(), "<<<<<<///data///>>>>>>>>" + data);
			if (data != null) {
				boolean isFinishSet = data.getBooleanExtra("isFinish", false);
				Ethernet_Network_Mode = data.getIntExtra(Ethernet_Mode, 0);
				if (isFinishSet) {
					LogUtil.d(getClass().getName(), "<<<<<<///isFinishSet///>>>>>>>>" + isFinishSet);
					DeviceInitController.handleBootFirstTime();
				}
			}
			imgSetting.requestFocus();
		}else {
			imgStarTV.requestFocus();
		}
		
		if (mImageSet == null || mImageSet.getCount() == 0) {
			mImageSet = new ImageSet(adapter2);
		}
		// 检查联网情况
		boolean isNetworkConnected = DbstarUtil.isNetworkAvailable(this);
		// 先判断是否联网，如果没有联网，就停留在海报页面

		if (!isNetworkConnected) {
			// 如果没有网络，则判断sd卡中是否存在已经存储的文件
			readQueryPosterFromSDCard();
			readQueryRecommandFromSDCard();
		} else {
			boolean isNetworkAvailable = DbstarUtil.isNetworkAvailable(this);
			if (isNetworkAvailable) {					
				// 如果有网络，则登录，并从服务器端取得数据
				getQueryPoster();
				getQueryRecommand();
			} else {
				// 如果有网络，但没有连接成功，则判断sd卡中是否存在已经存储的文件
				readQueryPosterFromSDCard();
				readQueryRecommandFromSDCard();
			}
		}

		if (adapter2 == null) {
			adapter2 = new GalleryAdapter(this, pictures);
		} else {
			adapter2.notifyDataSetChanged();
		}
	}
	
	private void setEventListener() {
		
//		imgStarTV.setOnClickListener(new BtnOnClickListener("com.dbstar"));
		imgStarTV.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent();
				intent.setClass(DbstarOTTActivity.this, GDLauncherActivity.class);
				startActivityForResult(intent, Btn_StarTV_Sequence);
				isToAthorAvtivity = true;
			}
		});
		imgCntv.setOnClickListener(new BtnOnClickListener("tv.icntv.ott", Btn_CntvTV_Sequence));
		imgAppShop.setOnClickListener(new BtnOnClickListener("com.guozi.appstore", Btn_AppShop_Sequence));
		imgMyApp.setOnClickListener(new BtnOnClickListener("com.dbstar.myapplication", Btn_MyApp_Sequence));
		// TODO：调用设置应用的时候有可能会用到jni
//		imgSetting.setOnClickListener(new BtnOnClickListener("com.mbx.settingsmbox"));
//		imgSetting.setOnClickListener(new BtnOnClickListener("com.settings.ottsettings", Btn_Setting_Sequence));
		imgSetting.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = startDbstarSettingActivity("OTTSettingsActivity");
				if (intent != null) {
					Bundle bundle = new Bundle();
					bundle.putInt(Ethernet_Mode, Ethernet_Network_Mode);
					intent.putExtra("mode", bundle);
					startActivityForResult(intent, Btn_Setting_Sequence);
				}
			}
		});
	}
	
	private void getQueryPoster() {
		List<NameValuePair> paramsList = new LinkedList<NameValuePair>();
		// TODO:参数可以动态获取吗？
		paramsList.add(new BasicNameValuePair("BIZTYPE", "2"));
		// 对参数进行编码
		String param = URLEncodedUtils.format(paramsList, "UTF-8");
		String url = Constants.Server_Url_QueryPoster + param;
		
		ConnectWork<QueryPosterData> work = new ConnectWork<QueryPosterData>(HttpConnect.POST, url, paramsList) {
			
			@Override
			public QueryPosterData processResult(HttpEntity entity) {
				
				QueryPosterData posterData = null;
				// 响应码为200，则是正常返回，这里就要处理一下HttpEntity
				if (entity != null) {
					posterData = parseQueryPosterData(entity);
				} 
				return posterData;
			}

			@Override
			public void connectComplete(QueryPosterData posterData) {

				if (posterData == null) {
					readQueryPosterFromSDCard();
					return;
				}

				HashMap<String, String> hashMap = new HashMap<String, String>();
				List<QueryPoster> queryPosters = posterData.getQueryPosters();
				if (queryPosters != null && queryPosters.size() > 0) {
					for (final QueryPoster poster : queryPosters) {

						final String urlPath = Constants.Server_Url_Image + poster.getIconUrl();

						ConnectWork<Bitmap> connectWork = new ConnectWork<Bitmap>(HttpConnect.GET, urlPath, null) {

							@Override
							public Bitmap processResult(HttpEntity entity) {
								return getBitmapFromEntity(entity);
							}

							@Override
							public void connectComplete(Bitmap bitmap) {
								if (bitmap != null) {
									// 将得到的海报存放在软引用中
									mImageSet.add(urlPath, null, bitmap);
									adapter2.add(urlPath, null, bitmap);
								}
							}
						};
						
						SimpleWorkPoolInstance.instance().execute(connectWork);
						hashMap.put(String.valueOf(poster.getSequence()),poster.getIconUrl());
					}
					// 将图片保存在文件中
					DbstarUtil.saveHashMap(DbstarOTTActivity.this, hashMap, "posterData1.txt");
				}
			}
		};
		SimpleWorkPoolInstance.instance().execute(work);
	}

	private Bitmap getBitmapFromEntity(HttpEntity entity) {
		Bitmap bitmap = null;
		if (entity != null) {
			try {
				InputStream inputStream = entity.getContent();
				bitmap = BitmapFactory.decodeStream(inputStream);
			} catch (IllegalStateException e) {
				LogUtil.d("getBitmapFromEntity", "从实体获取图片异常：：" + e);
			} catch (IOException e) {
				LogUtil.d("getBitmapFromEntity", "从实体获取图片异常：：" + e);
			}
		}
		return bitmap;
	}
	
	private void getQueryRecommand() {
		List<NameValuePair> paramsList = new LinkedList<NameValuePair>();
		paramsList.add(new BasicNameValuePair("BIZTYPE", "2"));
		// 对参数进行编码
		String param = URLEncodedUtils.format(paramsList, "UTF-8");
		String url = Constants.Server_Url_QueryRecommand + param;
		
		ConnectWork<QueryRecommandData> work = new ConnectWork<QueryRecommandData>(HttpConnect.POST, url, paramsList) {
			
			@Override
			public QueryRecommandData processResult(HttpEntity entity) {
				QueryRecommandData recommandData = null;
				if (entity != null) {
					recommandData = parseQueryRecommand(entity);
				}
				return recommandData;
			}
			
			@Override
			public void connectComplete(QueryRecommandData recommandData) {
				if (recommandData == null) {
					readQueryRecommandFromSDCard();
					return;
				}
				
				HashMap<String, String> hashMap = new HashMap<String, String>();
				List<QueryRecommand> queryRecommands = recommandData.getQueryRecommands();

				if (queryRecommands != null && queryRecommands.size() > 0) {

					for (final QueryRecommand recommand : queryRecommands) {

						String urlPath = Constants.Server_Url_Image + recommand.getIcon1Url();
						String linkUrl = recommand.getLinkURI();
						String[] split = linkUrl.split(":");
						String packageName = split[0];
						LogUtil.i("packageName", packageName);

						ConnectWork<Bitmap> connectWork = new ConnectWork<Bitmap>(HttpConnect.GET, urlPath, null) {

							@Override
							public Bitmap processResult(HttpEntity entity) {
								return getBitmapFromEntity(entity);
							}

							@Override
							public void connectComplete(Bitmap bitmap) {
								if (bitmap != null) {
									// 这里需要设置下面一排按钮的图片和文字，并保存
									if (recommand.getSequence() == 1) {
										imgStarTV.setImageBitmap(bitmap);
									} else if (recommand.getSequence() == 2) {
										imgCntv.setImageBitmap(bitmap);
									} else if (recommand.getSequence() == 3) {
										imgAppShop.setImageBitmap(bitmap);
									} else if (recommand.getSequence() == 4) {
										imgMyApp.setImageBitmap(bitmap);
									} else {
										imgSetting.setImageBitmap(bitmap);
									}
								}

							}
						};
						SimpleWorkPoolInstance.instance().execute(connectWork);

						// 保存
						hashMap.put(String.valueOf(recommand.getSequence()), recommand.getName());
						hashMap.put(recommand.getName(), recommand.getIcon1Url());
						hashMap.put(recommand.getIcon1Url(), recommand.getLinkURI());
						
						populateRecommondNameAndPackageName(recommand.getSequence(), recommand.getName(), packageName);
					}

					// 将会HashMap存放在文件中
					DbstarUtil.saveHashMap(DbstarOTTActivity.this, hashMap, "queryRecommand1.txt");
				}
				
			}

		};
		SimpleWorkPoolInstance.instance().execute(work);
	}
	
	private void populateRecommondNameAndPackageName(int sequence, String name, String packageName) {
		if (sequence == 1) {
			txtStarTV.setText(name);
//			imgStarTV.setOnClickListener(new BtnOnClickListener("com.dbstar"));
			imgStarTV.setOnClickListener(new OnClickListener() {
				
				@Override
				public void onClick(View v) {
					Intent intent = new Intent();
					intent.setClass(DbstarOTTActivity.this, GDLauncherActivity.class);
//					startActivity(intent);
					startActivityForResult(intent, Btn_StarTV_Sequence);
					isToAthorAvtivity = true;
				}
			});
		} else if (sequence == 2) {
			txtCntv.setText(name);
			imgCntv.setOnClickListener(new BtnOnClickListener(packageName, Btn_CntvTV_Sequence));
		} else if (sequence == 3) {
			txtAppShop.setText(name);
			// TODO:调用接口返回的apk本地没有
			imgAppShop.setOnClickListener(new BtnOnClickListener("com.guozi.appstore", Btn_AppShop_Sequence));
		} else if (sequence == 4) {
			txtMyApp.setText(name);
			imgMyApp.setOnClickListener(new BtnOnClickListener("com.dbstar.myapplication", Btn_MyApp_Sequence));
		} else {
			txtSetting.setText(name);
			imgSetting.setOnClickListener(new BtnOnClickListener("com.settings.ottsettings", Btn_Setting_Sequence));
		}
	}
	
	private QueryPosterData parseQueryPosterData(HttpEntity entity) {
		QueryPosterData posterData = new QueryPosterData();
		
		try {
			String entityString = EntityUtils.toString(entity, "UTF-8");
			
			JSONObject jsonObject = new JSONObject(entityString);
			JSONObject json = jsonObject.getJSONObject("Response");
			
			JSONObject jsonObj = json.getJSONObject("Body");
			int count = jsonObj.getInt("Count");
			posterData.setCount(count);
			int totalCount = jsonObj.getInt("TotalCount");
			posterData.setTotalCount(totalCount);
			int pageSize = jsonObj.getInt("PageSize");
			posterData.setPageSize(pageSize);
			int pageIndex = jsonObj.getInt("PageIndex");
			posterData.setPageIndex(pageIndex);

			JSONArray jsonArray = jsonObj.getJSONArray("Items");
			List<QueryPoster> queryPosters = new ArrayList<QueryPoster>();
			for (int j = 0; j < jsonArray.length(); j++) {
				JSONObject object = jsonArray.getJSONObject(j);
				QueryPoster queryPoster = new QueryPoster();

				int type = object.getInt("Type");
				queryPoster.setType(type);
				String title = object.getString("Title");
				queryPoster.setTitle(title);
				// 图片路径
				String iconUrl = object.getString("Icon2");
				queryPoster.setIconUrl(iconUrl);

				String parentCode = object.getString("ParentCode");
				queryPoster.setParentCode(parentCode);
				// 顺序编号
				int sequence = object.getInt("Sequence");
				queryPoster.setSequence(sequence);
				String code = object.getString("Code");
				queryPoster.setCode(code);

				// int ratingLevel = object.getInt("Ratinglevel");
				// queryPoster.setRatingLevel(ratingLevel);

				queryPosters.add(queryPoster);
			}
			posterData.setQueryPosters(queryPosters);
		} catch (ParseException e) {
			LogUtil.e("processResult", "ParseException 解析失败", e);
			e.printStackTrace();
		} catch (IOException e) {
			LogUtil.e("processResult", "IOException 失败", e);
			e.printStackTrace();
		} catch (JSONException e) {
			LogUtil.e("processResult", "JSONException JSON解析失败", e);
			e.printStackTrace();
		}
		return posterData;
	}
	
	private QueryRecommandData parseQueryRecommand(HttpEntity entity) {
		QueryRecommandData recommandData = new QueryRecommandData();
		
		try {
			String entityString = EntityUtils.toString(entity, "UTF-8");
			
			JSONObject jsonObject = new JSONObject(entityString);
			JSONObject json = jsonObject.getJSONObject("Response");
			
			JSONObject jsonObj = json.getJSONObject("Body");

			int count = jsonObj.getInt("Count");
			recommandData.setCount(count);
			int totalCount = jsonObj.getInt("TotalCount");
			recommandData.setTotalCount(totalCount);
			int pageSize = jsonObj.getInt("PageSize");
			recommandData.setPageSize(pageSize);
			int pageIndex = jsonObj.getInt("PageSize");
			recommandData.setPageIndex(pageIndex);

			JSONArray jsonArray = jsonObj.getJSONArray("COLUMNS");
			List<QueryRecommand> recommands = new ArrayList<QueryRecommand>();
			for (int j = 0; j < jsonArray.length(); j++) {
				JSONObject object = jsonArray.getJSONObject(j);
				QueryRecommand queryRecommand = new QueryRecommand();

				int sequence = object.getInt("Sequence");
				queryRecommand.setSequence(sequence);
				String name = object.getString("Name");
				queryRecommand.setName(name);
				String rESName = object.getString("RESName");
				queryRecommand.setrESName(rESName);
				String code = object.getString("Code");
				queryRecommand.setCode(code);

				String iconUrl = object.getString("Icon1");
				queryRecommand.setIcon1Url(iconUrl);

				String linkType = object.getString("LinkType");
				queryRecommand.setLinkType(linkType);
				String linkURI = object.getString("LinkURI");
				queryRecommand.setLinkURI(linkURI);
				String parentCode = object.getString("ParentCode");
				queryRecommand.setParentCode(parentCode);
				String columnType = object.getString("ColumnType");
				queryRecommand.setColumnType(columnType);

				recommands.add(queryRecommand);
			}

			recommandData.setQueryRecommands(recommands);
		} catch (ParseException e) {
			LogUtil.e("processResult", "ParseException 解析失败", e);
			e.printStackTrace();
		} catch (IOException e) {
			LogUtil.e("processResult", "IOException 失败", e);
			e.printStackTrace();
		} catch (JSONException e) {
			LogUtil.e("processResult", "JSONException JSON解析失败", e);
			e.printStackTrace();
		}
		return recommandData;
	}
	
	private void populateData() {
		gallery.setImageActivity(this, pictures, mImageSet);
		adapter2 = new GalleryAdapter(this, pictures);
        gallery.setAdapter(adapter2);
        
        Timer timer = new Timer();
        if (timer != null) {
        	if (task != null) {
        		// 将原任务从队列中删除
        		task.cancel();
        	}
        	
        	task = new GalleryTask();
        	timer.schedule(task, 5000, 5000);
        }
		
		gallery.setFocusable(false);
		
		final Timer APServiceTimer = new Timer();
		TimerTask timerTask = new TimerTask() {
			
			@Override
			public void run() {
				Message message = new Message();
				message.what = 3;
				handler.sendMessage(message);
				LogUtil.d("DbstarOTTActivity", "------------APServiceTimer==" + System.currentTimeMillis());
				if (APServiceTimer != null) {					
					APServiceTimer.cancel();
				}
			}
		};
		
		if (APServiceTimer != null && !timerTag) {
			APServiceTimer.schedule(timerTask, 15*1000);
		}
		
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			return true;			
		} else if (keyCode == KeyEvent.KEYCODE_MENU) {
			// 主要是针对新遥控器上的设置键,但是新遥控器上设置的键值是旧遥控器上的菜单键的键值
			Intent intent = startDbstarSettingActivity("OTTSettingsActivity");
			if (intent != null) {
				Bundle bundle = new Bundle();
				bundle.putInt(Ethernet_Mode, Ethernet_Network_Mode);
				intent.putExtra("mode", bundle);
				startActivityForResult(intent, Btn_Setting_Sequence);
			}
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	private void findViews() {
		gallery = (FlashGallery) findViewById(R.id.flash_gallery);
		imgStarTV = (ImageView) findViewById(R.id.flash_startv_img);
		imgCntv = (ImageView) findViewById(R.id.flash_cntv_img);
		imgAppShop = (ImageView) findViewById(R.id.flash_appshop_img);
		imgMyApp = (ImageView) findViewById(R.id.flash_myapp_img);
		imgSetting = (ImageView) findViewById(R.id.flash_setting_img);
		
		txtStarTV = (TextView) findViewById(R.id.flash_startv_txt);
		txtCntv = (TextView) findViewById(R.id.flash_cntv_txt);
		txtAppShop = (TextView) findViewById(R.id.flash_appshop_txt);
		txtMyApp = (TextView) findViewById(R.id.flash_myapp_txt);
		txtSetting = (TextView) findViewById(R.id.flash_setting_txt);
	}
	
	private Intent startComponent(String packageName, String activityName) {
		Intent intent = new Intent();
		String componentName = packageName + "." + activityName;
		intent.setComponent(new ComponentName(packageName, componentName));
		intent.setAction("android.intent.action.VIEW");

		return intent;
	}

	private Intent startDbstarSettingActivity(String activityName) {
		return startComponent("com.settings.ottsettings", activityName);
	}
	
	private class BtnOnClickListener implements OnClickListener {
		private long lastClick = 0l;
		private String packageName;
		private int btnSequence;
		
		public BtnOnClickListener(String packageName, int btnSequence) {
			this.packageName = packageName;
			this.btnSequence = btnSequence;
		}
		
		@Override
		public void onClick(View v) {
			if (System.currentTimeMillis() - lastClick < 800l) {
				return;
			}
			lastClick = System.currentTimeMillis();
			
			if (packageName.equals("com.dbstar")) {				
				Bundle bundle = new Bundle();
				bundle.putString("packageName", packageName);
				Intent intent = new Intent();
				intent.putExtras(bundle);
				intent.setClass(DbstarOTTActivity.this, MainActivity.class);
				startActivityForResult(intent, btnSequence);
			} else {
				Intent intent = getPackageManager().getLaunchIntentForPackage(packageName);
				LogUtil.d("DbstarOTTActivity", "BtnOnClickListener btnsequence = " + btnSequence);
				startActivityForResult(intent, btnSequence);
			}
			
			isToAthorAvtivity = true;
			LogUtil.d("BtnOnClickListener", "包名：：" + packageName);
		}
	}
}

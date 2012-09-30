package com.dbstar.guodian.model;

import java.io.UnsupportedEncodingException;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.dbstar.guodian.service.IDbstarService;

public class GDDBStarClient {
	private static final String TAG = "GDDBStarClient";
	private static final String DOWNLOAD_FINISH_ACTION = "com.dbstar.DbstarDVB.DOWNLOAD_FINISHED";

	private static final int DBSTARSERVICE_NONE = -1;
	private static final int DBSTARSERVICE_START = 0;
	private static final int DBSTARSERVICE_STOP = 1;

	Context mContext;
	private Intent mIntent = new Intent();

	private int mDbStarServiceState = DBSTARSERVICE_NONE;
	private int mDbStarServiceTargetState = DBSTARSERVICE_NONE;

	private IDbstarService mDbstarService = null;
	private ComponentName mComponentName = new ComponentName(
			"com.dbstar.DbstarDVB", "com.dbstar.DbstarDVB.DbstarService");

	private ServiceConnection mConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className, IBinder service) {

			Log.d(TAG, "++++++++++++++++GDDBStarClient onServiceConnected++++++++++++++");
			
			mDbstarService = IDbstarService.Stub.asInterface(service);

			if (mDbStarServiceTargetState == DBSTARSERVICE_START
					&& mDbStarServiceState != DBSTARSERVICE_START) {
				startDvbpush();
			} else if (mDbStarServiceTargetState == DBSTARSERVICE_STOP
					&& mDbStarServiceState != DBSTARSERVICE_STOP) {
				stopDvbpush();
			}
		}

		public void onServiceDisconnected(ComponentName className) {
			mDbstarService = null;
		}
	};


	public GDDBStarClient(Context context) {
		mContext = context;
	}

	public void start() {
		mIntent.setComponent(mComponentName);
		mContext.startService(mIntent);
		mContext.bindService(mIntent, mConnection, Context.BIND_AUTO_CREATE);
	}

	public void stop() {
		mContext.stopService(mIntent);
		mContext.unbindService(mConnection);
	}

	public void setListener() {

	}

	public void startDvbpush() {
		if (mDbstarService != null) {
			try {
				mDbstarService.startDvbpush();
				mDbStarServiceState = DBSTARSERVICE_START;

			} catch (RemoteException e) {
				e.printStackTrace();
			}
		} else {
			mDbStarServiceTargetState = DBSTARSERVICE_START;
		}
	}

	public void stopDvbpush() {
		if (mDbstarService != null) {
			try {
				mDbstarService.stopDvbpush();
				mDbStarServiceState = DBSTARSERVICE_STOP;

			} catch (RemoteException e) {
				e.printStackTrace();
			}
		} else {
			mDbStarServiceTargetState = DBSTARSERVICE_STOP;
		}
	}

	public boolean startTaskInfo() {
		boolean result = false;
		if (mDbstarService != null) {
			try {
				mDbstarService.startTaskInfo();
				result = true;
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}

		return result;
	}

	public boolean stopTaskInfo() {
		boolean result = false;
		if (mDbstarService != null) {
			try {
				mDbstarService.stopTaskInfo();
				result = true;
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}

		return result;
	}

	// "1001|taska|23932|23523094823\n1002|����2|234239|12349320\n"
	public ReceiveEntry[] getTaskInfo() {
		ReceiveEntry[] entries = null;
		try {
			Intent it = mDbstarService.getTaskInfo();
			byte[] bytes = it.getByteArrayExtra("taskinfo");
			try {
				String info = new String(bytes, "utf-8");
				Log.d(TAG, "TaskInfo: " + info);
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		} catch (RemoteException e) {
			e.printStackTrace();
		}

		return entries;
	}

}

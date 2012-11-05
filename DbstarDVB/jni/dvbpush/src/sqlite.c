﻿#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"
#include "sqlite3.h"
#include "sqlite.h"
#include "porting.h"

/*
数据库必须及时打开、及时关闭，不能采用打开计数器的方式，因为上层应用也可能在使用
*/

///used in this file
static sqlite3* g_db = NULL;												///the pointer of database created or opened
static int s_sqlite_init_flag = 0;

static int createTable(char* name);
static void closeDatabase();
static int localcolumn_init();
static int global_info_init();

static int createDatabase()
{
	char	*errmsgOpen=NULL;
	int		ret = 0;
	
	if(g_db!=NULL){
		DEBUG("the database has opened\n");
		ret = 0;
	}
	else
	{
		char database_uri[64];
		memset(database_uri, 0, sizeof(database_uri));
		if(-1==database_uri_get(database_uri, sizeof(database_uri))){
			DEBUG("get database uri failed\n");
			return -1;
		}
		
		DEBUG("database uri: %s\n", database_uri);
		if(-1==dir_exist_ensure(database_uri)){
			return -1;
		}

		if(SQLITE_OK!=sqlite3_open(database_uri,&g_db)){
			ERROROUT("can't open database: %s\n", database_uri);
			ret = -1;
		}
		else{
			/// open foreign key support
			if(sqlite3_exec(g_db,"PRAGMA foreign_keys=ON;",NULL,NULL,&errmsgOpen)
				|| NULL!=errmsgOpen){
				ERROROUT("can't open foreign_keys\n");
				DEBUG("database errmsg: %s\n", errmsgOpen);
				ret = -1;
			}
			else{
				ret = 0;
				
				int createtable_ret = createTable("Global");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Initialize");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Channel");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Service");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResStr");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResPoster");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResTrailer");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResSubTitle");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResExtension");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ResExtensionFile");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Column");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ColumnEntity");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Product");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("PublicationsSet");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Publication");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("MultipleLanguageInfoVA");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("MultipleLanguageInfoRM");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("MultipleLanguageInfoApp");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Message");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("GuideList");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("ProductDesc");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("Preview");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("RejectRecv");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
				
				createtable_ret = createTable("SProduct");
				if(-1==createtable_ret){
					ret = -1;
					goto CREATE_TABLE_END;
				}
				else{
					ret += createtable_ret;
				}
CREATE_TABLE_END:
				DEBUG("shot tables finished, ret=%d\n", ret);
			}
			sqlite3_free(errmsgOpen);
		}
		closeDatabase();
	}
	
	return ret;
}

static int openDatabase()
{
	int ret = -1;
	
	if(g_db!=NULL){
		DEBUG("the database has opened\n");
		ret = 0;
	}
	else
	{
		char database_uri[64];
		memset(database_uri, 0, sizeof(database_uri));
		if(-1==database_uri_get(database_uri, sizeof(database_uri))){
			DEBUG("get database uri failed\n");
			return -1;
		}
		if(SQLITE_OK!=sqlite3_open(database_uri,&g_db)){
			ERROROUT("can't open database\n");
			ret = -1;
		}
		else
			ret = 0;
	}
	
	return ret;
}

static void closeDatabase()
{
	if(g_db!=NULL)
	{
		sqlite3_close(g_db);
		g_db=NULL;
	}
	else{
		DEBUG("g_db is NULL, can not do database close action\n");
	}
	
	return;
}

/*
返回值：
0——指定的表存在，成功；
-1——创建表失败；
1——指定的表创建成功。
*/
static int createTable(char* name)
{
	char* errmsg=NULL;
	char ** l_result=NULL;									    	///result of tables in database
	int l_row=0;                                            	///the row of result
	int l_column=0;									        	///the column of result
	char sqlite_cmd[1024];
	int ret = -1;
	
	memset(sqlite_cmd, 0, sizeof(sqlite_cmd));
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "SELECT name FROM sqlite_master WHERE type='table' AND name='%s';", name);
	if(sqlite3_get_table(g_db,sqlite_cmd,&l_result,&l_row,&l_column,&errmsg))
	{
		ERROROUT("read tables from database failed.");
		ret = -1;
	}
	else{
		if(l_row>0){
			DEBUG("tabel \"%s\" is exist, OK\n", name);
			ret = 0;
		}
		else{
			sqlite3_free(errmsg);
			ret = 1;
			/*
			这里建立表的目的是查询，不是存储，所以不能用于查询的图片、结构体、描述等
			*/
			if(!strcmp(name,"Global"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
Name	NVARCHAR(64) PRIMARY KEY,\
Value	NVARCHAR(128),\
Param	NVARCHAR(1024));", name);
			}
			else if(!strcmp(name,"Initialize"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PushFlag	NVARCHAR(64) PRIMARY KEY,\
XMLName	NVARCHAR(64),\
Version	NVARCHAR(64),\
StandardVersion	NVARCHAR(32),\
URI	NVARCHAR(256));", name);
			}
			else if(!strcmp(name,"Channel"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
pid	NVARCHAR(64) PRIMARY KEY,\
pidtype	NVARCHAR(64),\
URI NVARCHAR(256),\
EffectFlag INTEGER DEFAULT 1);", name);
			}
			else if(!strcmp(name,"Service"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ServiceID	NVARCHAR(64) PRIMARY KEY,\
RegionCode	NVARCHAR(64),\
OnlineTime	RCHAR(32),\
OfflineTime	RCHAR(32));", name);
			}
			else if(!strcmp(name,"ResStr"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(64),\
EntityID	NVARCHAR(64),\
StrLang		NVARCHAR(32),\
StrName		NVARCHAR(64),\
Extension	NVARCHAR(64) DEFAULT '',\
StrValue	NVARCHAR(1024),\
PRIMARY KEY (ObjectName,EntityID,StrLang,StrName,Extension));", name);
			}
			else if(!strcmp(name,"ResPoster"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(64),\
EntityID	NVARCHAR(64),\
PosterID	NVARCHAR(64),\
PosterName	NVARCHAR(64),\
PosterURI	NVARCHAR(256),\
PRIMARY KEY (ObjectName,EntityID,PosterID));", name);
			}
			else if(!strcmp(name,"ResTrailer"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(64),\
EntityID	NVARCHAR(64),\
TrailerID	NVARCHAR(64),\
TrailerName	NVARCHAR(64),\
TrailerURI	NVARCHAR(256),\
PRIMARY KEY (ObjectName,EntityID,TrailerID));", name);
			}
			else if(!strcmp(name,"ResSubTitle"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(64),\
EntityID	NVARCHAR(64),\
SubTitleID	NVARCHAR(64),\
SubTitleName	NVARCHAR(64),\
SubTitleLanguage	NVARCHAR(64),\
SubTitleURI	NVARCHAR(256),\
PRIMARY KEY (ObjectName,EntityID,SubTitleID));", name);
			}
			else if(!strcmp(name,"ResExtension"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(256),\
EntityID	NVARCHAR(64),\
Name	NVARCHAR(64),\
Type	NVARCHAR(64),\
PRIMARY KEY (ObjectName,EntityID,Name));", name);
			}
			else if(!strcmp(name,"ResExtensionFile"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ObjectName	NVARCHAR(256),\
EntityID	NVARCHAR(64),\
FileID	NVARCHAR(64),\
FileName	NVARCHAR(64),\
FileURI	NVARCHAR(256),\
PRIMARY KEY (ObjectName,EntityID,FileID));", name);
			}
			else if(!strcmp(name,"Column"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ColumnID	NVARCHAR(64) PRIMARY KEY,\
ParentID	NVARCHAR(64),\
Path	NVARCHAR(256),\
ColumnType	NVARCHAR(256),\
ColumnIcon_losefocus	NVARCHAR(256),\
ColumnIcon_getfocus	NVARCHAR(256),\
ColumnIcon_onclick	NVARCHAR(256),\
SequenceNum	INTEGER,\
URI	NVARCHAR(256));", name);
			}
			else if(!strcmp(name,"ColumnEntity"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ColumnID	NVARCHAR(64),\
EntityID	NVARCHAR(64),\
EntityType	NVARCHAR(64));", name);
			}
			else if(!strcmp(name,"Product"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ProductID	NVARCHAR(64) PRIMARY KEY,\
ProductType	NVARCHAR(64),\
Flag	NVARCHAR(64),\
OnlineDate	CHAR(32),\
OfflineDate	CHAR(32),\
IsReserved	CHAR(32),\
Price	NVARCHAR(32),\
CurrencyType	NVARCHAR(32),\
DRMFile	NVARCHAR(256),\
ColumnID	NVARCHAR(64),\
IsAuthorized	NVARCHAR(64),\
VODNum	NVARCHAR(64),\
VODPlatform	NVARCHAR(256));", name);
			}
			else if(!strcmp(name,"PublicationsSet"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
SetID	NVARCHAR(64),\
ColumnID	NVARCHAR(64),\
ProductID	NVARCHAR(64),\
URI	NVARCHAR(256),\
TotalSize	NVARCHAR(64),\
ProductDescID	NVARCHAR(64),\
ReceiveStatus	NVARCHAR(64) DEFAULT '0',\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
IsReserved	NVARCHAR(64),\
Visible	NVARCHAR(64) DEFAULT '1',\
Favorite	NVARCHAR(64) DEFAULT '0',\
IsAuthorized	NVARCHAR(64),\
VODNum	NVARCHAR(64),\
VODPlatform	NVARCHAR(256),\
Deleted NVARCHAR(256) DEFAULT '0',\
PRIMARY KEY (SetID,ColumnID));", name);
			}
			else if(!strcmp(name,"Publication"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PublicationID	NVARCHAR(64),\
ColumnID	NVARCHAR(64),\
ProductID	NVARCHAR(64),\
URI	NVARCHAR(256),\
DescURI	NVARCHAR(256),\
TotalSize	NVARCHAR(64),\
ProductDescID	NVARCHAR(64),\
ReceiveStatus	NVARCHAR(64) DEFAULT '0',\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
PublicationType	NVARCHAR(64),\
IsReserved	CHAR(32),\
Visible	CHAR(32) DEFAULT '1',\
DRMFile	NVARCHAR(256),\
SetID	NVARCHAR(64),\
IndexInSet	NVARCHAR(32),\
Favorite	NVARCHAR(32) DEFAULT '0',\
Bookmark	NVARCHAR(32) DEFAULT '0',\
IsAuthorized	NVARCHAR(64),\
VODNum	NVARCHAR(64),\
VODPlatform	NVARCHAR(256),\
Deleted NVARCHAR(256) DEFAULT '0',\
FileID	NVARCHAR(64),\
FileSize	NVARCHAR(64),\
FileURI	NVARCHAR(256),\
FileType	NVARCHAR(64),\
FileFormat	NVARCHAR(32),\
Duration	NVARCHAR(32),\
Resolution	NVARCHAR(32),\
BitRate	NVARCHAR(32),\
CodeFormat	NVARCHAR(32),\
PRIMARY KEY (PublicationID,ColumnID));", name);
			}
			else if(!strcmp(name,"MultipleLanguageInfoVA"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PublicationID	NVARCHAR(64),\
infolang	NVARCHAR(64),\
PublicationDesc	NVARCHAR(1024),\
ImageDefinition	NVARCHAR(32),\
Keywords	NVARCHAR(256),\
Area	NVARCHAR(64),\
Language	NVARCHAR(64),\
Episode	NVARCHAR(32),\
AspectRatio	NVARCHAR(32),\
AudioChannel	NVARCHAR(32),\
Director	NVARCHAR(128),\
Actor	NVARCHAR(256),\
Audience	NVARCHAR(64),\
Model	NVARCHAR(32),\
PRIMARY KEY (PublicationID,infolang));", name);
			}
			else if(!strcmp(name,"MultipleLanguageInfoRM"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PublicationID	NVARCHAR(64),\
infolang	NVARCHAR(64),\
PublicationDesc	NVARCHAR(1024),\
Keywords	NVARCHAR(256),\
Publisher	NVARCHAR(128),\
Area	NVARCHAR(64),\
Language	NVARCHAR(64),\
Episode	NVARCHAR(32),\
AspectRatio	NVARCHAR(32),\
VolNum	NVARCHAR(64),\
ISSN	NVARCHAR(64),\
PRIMARY KEY (PublicationID,infolang));", name);
			}
			else if(!strcmp(name,"MultipleLanguageInfoApp"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PublicationID	NVARCHAR(64),\
infolang	NVARCHAR(64),\
PublicationDesc	NVARCHAR(1024),\
Keywords	NVARCHAR(256),\
Category	NVARCHAR(64),\
Released	NVARCHAR(64),\
AppVersion	NVARCHAR(64),\
Language	NVARCHAR(64),\
Developer	NVARCHAR(64),\
Rated	NVARCHAR(64),\
Requirements	NVARCHAR(64),\
PRIMARY KEY (PublicationID,infolang));", name);
			}
			else if(!strcmp(name,"Message"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
MessageID	NVARCHAR(64),\
type	NVARCHAR(64),\
displayForm	NVARCHAR(64),\
StartTime	CHAR(32),\
EndTime		CHAR(32),\
Interval	CHAR(32));", name);
			}
			else if(!strcmp(name,"GuideList"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
DateValue	NVARCHAR(64),\
GuideListID	NVARCHAR(64),\
productID	NVARCHAR(64),\
PublicationID	NVARCHAR(64),\
URI	NVARCHAR(256),\
TotalSize	NVARCHAR(64),\
ProductDescID	NVARCHAR(64),\
ReceiveStatus	NVARCHAR(64) DEFAULT '0',\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
UserStatus	NVARCHAR(64) DEFAULT '1',\
PRIMARY KEY (DateValue,PublicationID));", name);
			}
			else if(!strcmp(name,"ProductDesc"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ReceiveType	NVARCHAR(64),\
ProductDescID	NVARCHAR(64),\
ID	NVARCHAR(64),\
TotalSize	NVARCHAR(64),\
URI	NVARCHAR(256),\
ReceiveStatus	NVARCHAR(64) DEFAULT '0',\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
PRIMARY KEY (ReceiveType,ID));", name);
			}
			else if(!strcmp(name,"Preview"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
PreviewID	NVARCHAR(64) PRIMARY KEY,\
ProductID	NVARCHAR(64),\
PreviewType	NVARCHAR(64),\
PreviewSize	NVARCHAR(64),\
ShowTime	NVARCHAR(64),\
PreviewURI	NVARCHAR(256),\
PreviewFormat	NVARCHAR(64),\
Duration	NVARCHAR(64),\
Resolution	NVARCHAR(64),\
BitRate	NVARCHAR(64),\
CodeFormat	NVARCHAR(64),\
URI	NVARCHAR(256),\
TotalSize	NVARCHAR(64),\
ProductDescID	NVARCHAR(64),\
ReceiveStatus	NVARCHAR(64) DEFAULT '0',\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
StartTime	CHAR(32),\
EndTime	CHAR(32),\
PlayMode	NVARCHAR(64));", name);
			}
			else if(!strcmp(name,"RejectRecv"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
ServiceID	NVARCHAR(64),\
ID	NVARCHAR(64),\
URI	NVARCHAR(512),\
Type	NVARCHAR(64),\
PushStartTime	NVARCHAR(64),\
PushEndTime	NVARCHAR(64),\
PRIMARY KEY (ServiceID,ID));", name);
			}
			else if(!strcmp(name,"SProduct"))
			{
				snprintf(sqlite_cmd, sizeof(sqlite_cmd),\
					"CREATE TABLE %s(\
SType	NVARCHAR(64),\
Name	NVARCHAR(64),\
URI	NVARCHAR(512));", name);
			}
			
			else{
				DEBUG("baby: table %s is not defined, so can not create it\n", name);
				memset(sqlite_cmd, 0, sizeof(sqlite_cmd));
				ret = -1;
			}
			
			if(strlen(sqlite_cmd)>0){
				if(0==sqlite3_exec(g_db,sqlite_cmd,NULL,NULL,&errmsg))
				{
					DEBUG("create table '%s' success\n", name);
					ret = 1;
				}
				else
				{
					ERROROUT("create '%s' failed: %s\n", name, sqlite_cmd);
					DEBUG("sqlite errmsg: %s\n", errmsg);
					ret = -1;
				}
			}
			else
				ret = -1;
		}
	}

	sqlite3_free_table(l_result);
	sqlite3_free(errmsg);
	return ret;
}
/***sqlite_init() brief init sqlite, include open database, create table, and so on.
 * param null
 *
 * retval int,0 if successful or -1 failed
 ***/
int sqlite_init()
{
	if(0==s_sqlite_init_flag){
		g_db = NULL;
		int ret = createDatabase();
		if(0==ret)
			DEBUG("open database success\n");
		else if(ret>0){
			DEBUG("create database success(some/all tables are created)\n");
			localcolumn_init();
			global_info_init();
		}
		else{						///open database failed
			DEBUG("create/open database failed\n");
			return -1;
		}
			
		s_sqlite_init_flag = 1;
	}

	return 0;						/// quit
}

int sqlite_uninit()
{
	return 0;
}

/***getGlobalPara() brief get some global variables from sqlite, such as 'version'.
 * param name[in], the name of global param
 *
 * retval int,0 if successful or -1 failed
 ***/
int sqlite_execute(char *exec_str)
{
	char* errmsg=NULL;
	int ret = -1;
	
	//open database
	if(-1==openDatabase())
	{
		ERROROUT("Open database failed\n");
		ret = -1;
	}
	else{
		DEBUG("%s\n", exec_str);
		if(sqlite3_exec(g_db,exec_str,NULL,NULL,&errmsg)){
			DEBUG("sqlite3 errmsg: %s\n", errmsg);
			ret = -1;
		}
		else{
			//DEBUG("sqlite3_exec success\n");
			ret = 0;
		}
		
		sqlite3_free(errmsg);								///	release the memery possessed by error message
		closeDatabase();									///	close database
	}
	
	return ret;	
}

/*
功能：	执行SELECT语句
输入：	sqlite_cmd				——sql SELECT语句
		receiver				——用于处理SELECT结果的参数，如果sqlite_read_callback为NULL，则receiver也可以为NULL
		sqlite_read_callback	——用于处理SELECT结果的回调，如果只是想知道查询到几条记录，则此回调可以为NULL
返回：	-1——失败；其他值——查询到的记录数
*/
int sqlite_read(char *sqlite_cmd, void *receiver, int (*sqlite_read_callback)(char **result, int row, int column, void *receiver))
{
	char* errmsg=NULL;
	char** l_result = NULL;
	int l_row = 0;
	int l_column = 0;
	int ret = 0;
	int (*sqlite_callback)(char **,int,int,void *) = sqlite_read_callback;

	DEBUG("sqlite read: %s\n", sqlite_cmd);
	
	///open database
	if(-1==openDatabase())
	{
		ERROROUT("Open database failed\n");
		ret = -1;
	}
	else{	// open database ok
		
		if(sqlite3_get_table(g_db,sqlite_cmd,&l_result,&l_row,&l_column,&errmsg)
			|| NULL!=errmsg)
		{
			ERROROUT("sqlite cmd: %s\n", sqlite_cmd);
			DEBUG("errmsg: %s\n", errmsg);
			ret = -1;
		}
		else{ // inquire table ok
			if(0==l_row){
				DEBUG("no row, l_row=0, l_column=%d", l_column);
				int i = 0;
				for(i=0;i<l_column;i++)
					printf("\t\t%s", l_result[i]);
				printf("\n");
			}
			else{
				DEBUG("sqlite select OK, %s\n", NULL==sqlite_callback?"no callback fun":"do callback fun");
				if(sqlite_callback)
					sqlite_callback(l_result, l_row, l_column, receiver);
				else{
					DEBUG("l_row=%d, l_column=%d\n", l_row, l_column);
//					int i = 0;
//					for(i=0;i<(l_column+1);i++)
//						printf("\t\t%s\n", l_result[i]);
				}
			}
			ret = l_row;
		}
		sqlite3_free_table(l_result);
		sqlite3_free(errmsg);
		closeDatabase();
	}
	
	return ret;
}

int sqlite_table_clear(char *table_name)
{
	DEBUG("CAUTION: will clear table '%s'\n", table_name);
	char sqlite_cmd[256];	
	
	memset(sqlite_cmd, 0, sizeof(sqlite_cmd));
	snprintf(sqlite_cmd,sizeof(sqlite_cmd),"DELETE FROM %s;", table_name);
	DEBUG("sqlite cmd str: %s\n", sqlite_cmd);

	int ret = sqlite_execute(sqlite_cmd);
	if(0==ret){
		DEBUG("table '%s' clear successfully\n", table_name);
		return 0;
	}
	else{
		DEBUG("table '%s' reset failed\n", table_name);
		return -1;
	}
}

#if 0
/*
 将一系列sqlite语句封装为一个事务，各个语句间用'\n'结尾。
*/
int sqlite_transaction(char *sqlite_cmd)
{
	if(NULL==sqlite_cmd || 0==strlen(sqlite_cmd)){
		DEBUG("invalid argument\n");
		return -1;
	}
	DEBUG("%s\n", sqlite_cmd);
	int ret = -1;
	
	if(-1==openDatabase())
	{
		ERROROUT("Open database failed\n");
		ret = -1;
	}
	else{
		ret = sqlite3_exec(g_db, "begin transaction", NULL, NULL, NULL);
		if(SQLITE_OK == ret){
			char *p_cmd = sqlite_cmd;
			char *p = NULL;
			unsigned int killed_len = 0;
			do{
				killed_len = 0;
				p = strchr(p_cmd, '\n');
				if(p){
					*p = '\0';
					killed_len += 1;
				}
				killed_len += strlen(p_cmd);
				
				DEBUG("sqlite cmd: %s, p=%s, killed_len=%d\n", p_cmd, p, killed_len);
				if(strlen(p_cmd)>0){
					ret = sqlite3_exec(g_db, p_cmd, NULL, NULL, NULL);
					if(SQLITE_OK == ret){
						;
					}
					else{
						DEBUG("sqlite3 errmsg: %s\n", sqlite3_errmsg(g_db));
						ret = -1;
						break;
					}
				}
				p_cmd += killed_len;
			}while(p_cmd && strlen(p_cmd)>0);
			
			if(SQLITE_OK == ret){
				ret = sqlite3_exec(g_db, "commit transaction", NULL, NULL, NULL);
			}
			else{
				DEBUG("rollback transaction\n");
				ret = sqlite3_exec(g_db, "rollback transaction", NULL, NULL, NULL);
			}
			
			if(SQLITE_OK == ret){
				ret = 0;
			}
			else{
				DEBUG("%s\n", sqlite3_errmsg(g_db));
				ret = -1;
			}
		}
		else{
			DEBUG("sqlite3 errmsg: %s\n", sqlite3_errmsg(g_db));
			ret = -1;
		}
		
		closeDatabase();									///	close database
	}
	
	return ret;
}
#endif

/*
 数据库事务使用规则：
 1、事务的开始和结束必须是成对出现的。事务不允许嵌套。
 2、对于整体替换的数据表，应将数据表清除、数据录入工作封装在一个事务中。
 3、所有Res开头的数据表，它们的数据都是依存于其他数据表才有意义，
 	因此这些数据表的插入、删除、更新等动作，均是某个宿主事务的一部分，它们自身不构成一个单独的事务。
*/
typedef enum{
	SQL_TRAN_STATUS_END = 0,
	SQL_TRAN_STATUS_BEGIN,
	SQL_TRAN_STATUS_LOADING
}SQL_TRAN_STATUS_E;
static SQL_TRAN_STATUS_E s_sql_tran_status = SQL_TRAN_STATUS_END;


/*
 函数1：事务开始
*/
int sqlite_transaction_begin()
{
	int ret = -1;
	
	DEBUG("sqlite_transaction_begin>>\n");
	if(SQL_TRAN_STATUS_BEGIN==s_sql_tran_status || SQL_TRAN_STATUS_LOADING==s_sql_tran_status){
		DEBUG("######### SQLITE TRANSACTION STATUS is abnormally #########\n");
		DEBUG("expect SQL_TRAN_STATUS_END but %d\n", s_sql_tran_status);
		DEBUG("###########################################################\n");
		return -1;
		// ret = sqlite_transaction_end(0);
	}
	
	if(SQL_TRAN_STATUS_END == s_sql_tran_status){
		if(-1==openDatabase())
		{
			ERROROUT("Open database failed\n");
			ret = -1;
		}
		else{
			ret = sqlite3_exec(g_db, "begin transaction", NULL, NULL, NULL);
			if(SQLITE_OK == ret){
				s_sql_tran_status = SQL_TRAN_STATUS_BEGIN;
				ret = 0;
				//DEBUG(">>>>>>>>>>>>>>>> 	ok\n");
			}
			else{
				DEBUG("sqlite3 errmsg: %s\n", sqlite3_errmsg(g_db));
				ret = -1;
			}
		}
	}
	
	return ret;
}
/*
 函数2：事务中的sqlite语句。
*/
int sqlite_transaction_exec(char *sqlite_cmd)
{
	if(NULL==sqlite_cmd || 0==strlen(sqlite_cmd)){
		DEBUG("invalid argument\n");
		return -1;
	}
	DEBUG("%s\n", sqlite_cmd);
	
	int ret = -1;
	
	if(SQL_TRAN_STATUS_END == s_sql_tran_status){
		DEBUG("######### SQLITE TRANSACTION STATUS is abnormally #########\n");
		DEBUG("expect SQL_TRAN_STATUS_BEGIN but %d\n", s_sql_tran_status);
		DEBUG("###########################################################\n");
		return -1;
		//ret = sqlite_transaction_begin();
	}
	
	if(SQL_TRAN_STATUS_BEGIN == s_sql_tran_status || SQL_TRAN_STATUS_LOADING == s_sql_tran_status){
		ret = sqlite3_exec(g_db, sqlite_cmd, NULL, NULL, NULL);
		if(SQLITE_OK == ret){
			s_sql_tran_status = SQL_TRAN_STATUS_LOADING;
			ret = 0;
		}
		else{
			DEBUG("sqlite3 errmsg: %s\n", sqlite3_errmsg(g_db));
			ret = -1;
		}
	}
	
	return ret;
}
/*
 函数3：清理数据表，在事务中执行。
*/
int sqlite_transaction_table_clear(char *table_name)
{
	DEBUG("CAUTION: will clear table '%s'\n", table_name);
	char sqlite_cmd[256];	
	
	snprintf(sqlite_cmd,sizeof(sqlite_cmd),"DELETE FROM %s;", table_name);
	
	return sqlite_transaction_exec(sqlite_cmd);
}

/*
 函数4：事务结束。
 参数：commin_flag——0：提交事务；-1：回滚事务。
*/
int sqlite_transaction_end(int commit_flag)
{
	int ret = -1;
	
	DEBUG("sqlite_transaction_end<< %s\n", 1==commit_flag?"commit":"rollback");
	if(SQL_TRAN_STATUS_BEGIN==s_sql_tran_status || SQL_TRAN_STATUS_LOADING==s_sql_tran_status){
		if(1==commit_flag){
			DEBUG("commit transaction\n");
			ret = sqlite3_exec(g_db, "commit transaction", NULL, NULL, NULL);
		}
		else{
			DEBUG("rollback transaction\n");
			ret = sqlite3_exec(g_db, "rollback transaction", NULL, NULL, NULL);
		}
		
		if(SQLITE_OK == ret){
			s_sql_tran_status = SQL_TRAN_STATUS_END;
			ret = 0;
			//DEBUG("<<<<<<<<<<<<<<<<<<<< 	ok\n");
		}
		else{
			DEBUG("%s\n", sqlite3_errmsg(g_db));
			ret = -1;
		}
		closeDatabase();
	}
	else{
		DEBUG("sqlite transaction is end, do nothing\n");
		ret = -1;
	}
	
	return ret;
}


/*
 本地栏目的初始化，目前是“设置”和“个人中心”
 注意和下发的Column.xml字段识别保持一致
*/
int localcolumn_init()
{
	char sqlite_cmd[512];
	
	DEBUG("init local column, such as 'Settings' or 'My Center\n");
	
	sqlite_transaction_begin();
	
	
	/*
	 一级菜单“个人中心”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',98);",
		"98","-1","98",COLUMN_MYCENTER,"LocalColumnIcon/MyCenter_losefocus.png","LocalColumnIcon/MyCenter_getfocus.png","MyCenter_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","98","chi","DisplayName","个人中心","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","98","eng","DisplayName","My Center","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“选择接收”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',1);",
		"9801","98","98/9801",COLUMN_MYCENTER,"LocalColumnIcon/Receiving_losefocus.png","LocalColumnIcon/Receiving_getfocus.png","Receiving_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9801","chi","DisplayName","选择接收","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9801","eng","DisplayName","Receiving","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“下载状态”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',2);",
		"9802","98","98/9802",COLUMN_MYCENTER,"LocalColumnIcon/Download_losefocus.png","LocalColumnIcon/Download_getfocus.png","Download_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9802","chi","DisplayName","下载状态","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9802","eng","DisplayName","Download","");
	sqlite_transaction_exec(sqlite_cmd);
	
	
	/*
	 一级菜单“设置”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',99);",
		"99","-1","99",COLUMN_SETTING,"LocalColumnIcon/Setting_losefocus.png","LocalColumnIcon/Setting_getfocus.png","Setting_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","99","chi","DisplayName","设置","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","99","eng","DisplayName","Setting","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－网络设置”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',1);",
		"9901","99","99/9901",COLUMN_SETTING,"LocalColumnIcon/Network_losefocus.png","LocalColumnIcon/Network_getfocus.png","Network_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9901","chi","DisplayName","网络设置","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9901","eng","DisplayName","Network","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－视频设置”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',2);",
		"9902","99","99/9902",COLUMN_SETTING,"LocalColumnIcon/Video_losefocus.png","LocalColumnIcon/Video_getfocus.png","Video_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9902","chi","DisplayName","视频设置","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9902","eng","DisplayName","Video","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－音频设置”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',3);",
		"9903","99","99/9903",COLUMN_SETTING,"LocalColumnIcon/Audio_losefocus.png","LocalColumnIcon/Audio_getfocus.png","Audio_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9903","chi","DisplayName","音频设置","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9903","eng","DisplayName","Audio","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－用户信息”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',4);",
		"9904","99","99/9904",COLUMN_SETTING,"LocalColumnIcon/UserInfo_losefocus.png","LocalColumnIcon/UserInfo_getfocus.png","UserInfo_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9904","chi","DisplayName","用户信息","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9904","eng","DisplayName","User Info","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－本机信息”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',5);",
		"9905","99","99/9905",COLUMN_SETTING,"LocalColumnIcon/DeviceInfo_losefocus.png","LocalColumnIcon/DeviceInfo_getfocus.png","DeviceInfo_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9905","chi","DisplayName","本机信息","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9905","eng","DisplayName","Device Info","");
	sqlite_transaction_exec(sqlite_cmd);
	/*
	 二级菜单“设置－帮助”
	*/
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Column(ColumnID,ParentID,Path,ColumnType,ColumnIcon_losefocus,ColumnIcon_getfocus,ColumnIcon_onclick,SequenceNum) VALUES('%s','%s','%s','%d','%s','%s','%s',6);",
		"9906","99","99/9906",COLUMN_SETTING,"LocalColumnIcon/Help_losefocus.png","LocalColumnIcon/Help_getfocus.png","Help_onclick.png");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9906","chi","DisplayName","帮助","");
	sqlite_transaction_exec(sqlite_cmd);
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO ResStr(ObjectName,EntityID,StrLang,StrName,StrValue,Extension) VALUES('%s','%s','%s','%s','%s','%s');",
		"Column","9906","eng","DisplayName","Help","");
	sqlite_transaction_exec(sqlite_cmd);
	
	
	return sqlite_transaction_end(1);
}

static int global_info_init()
{
	DEBUG("init table 'Global', set default records\n");
	
	char sqlite_cmd[1024];
	sqlite_transaction_begin();
	
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Global(Name,Value,Param) VALUES('%s','%s','');",
		GLB_NAME_PREVIEWPATH,DBSTAR_PREVIEWPATH);
	sqlite_transaction_exec(sqlite_cmd);
	
	snprintf(sqlite_cmd, sizeof(sqlite_cmd), "REPLACE INTO Global(Name,Value,Param) VALUES('%s','%s','');",
		GLB_NAME_CURLANGUAGE,"chi");
	sqlite_transaction_exec(sqlite_cmd);
	
	return sqlite_transaction_end(1);
}

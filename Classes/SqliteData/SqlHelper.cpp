/**********天鸣**********/
#include "SqlHelper.h"

#define TAG "SqlHelper"
mutex SqlHelper::sqlLock;
SqlHelper::SqlHelper()
{
	SqlDB = nullptr;
	msg = nullptr;
	openSql();
}

SqlHelper::~SqlHelper()
{
	sqlite3_close(SqlDB);
}

void SqlHelper::openSql()
{
	std::string path = FileUtils::getInstance()->getWritablePath();
	path += "gameOfLine.db";
	int result = sqlite3_open(path.c_str(), &SqlDB);//打开数据库，没有则创建 
	if(result == SQLITE_OK){//成功的话
		bool isHave;
		string sqlstr;//创建表语句 
		////////////////-----userInfo-----////////////////
		//demo-//isHave = isHaveTable(SqlDBData::userdbName);
		//if(!isHave){//没有的话创建
		//	SqlHelper::sqlLock.lock();
		//	sqlstr="create table "+SqlDBData::userdbName+"( userId INTEGER PRIMARY KEY, userName TEXT , diamondNum INTEGER , brushNum INTEGER , brushJiLuTime TEXT )"; 
		//	result = sqlite3_exec( SqlDB, sqlstr.c_str() , NULL, NULL, &msg );   
		//	SqlHelper::sqlLock.unlock();
		//	if( result != SQLITE_OK ) {
		//		LOGD(TAG,"创建表失败，错误码:%d ，错误原因:%s\n" , result, msg );
		//		sqlite3_close(SqlDB);
		//		sqlite3_free(msg);
		//	}else 
		//		userInfoAdd();	
		//}
		////////////////-----item-----////////////////
		//isHave = isHaveTable(SqlDBData::itemdbName);
		//demo-//if(!isHave){//没有的话创建
		//	SqlHelper::sqlLock.lock();
		//	sqlstr="create table "+SqlDBData::itemdbName+"( itemId INTEGER , userId INTEGER , itemName TEXT , itemNum INTEGER , itemPrice INTEGER )"; 
		//	result = sqlite3_exec( SqlDB, sqlstr.c_str() , NULL, NULL, &msg );   
		//	SqlHelper::sqlLock.unlock();
		//	if( result != SQLITE_OK ) {
		//		LOGD(TAG,"创建表失败，错误码:%d ，错误原因:%s\n" , result, msg );
		//		sqlite3_close(SqlDB);
		//		sqlite3_free(msg);
		//	}else 
		//		itemInfoAdd();		
		//}
		////////////////-----score-----////////////////
		//demo-//isHave = isHaveTable(SqlDBData::scoredbName);
		//if(!isHave){//没有的话创建
		//	SqlHelper::sqlLock.lock();
		//	sqlstr="create table "+SqlDBData::scoredbName+"( scoreId INTEGER PRIMARY KEY AUTOINCREMENT, userId INTEGER , gameMode INTEGER , score INTEGER )"; 
		//	result = sqlite3_exec( SqlDB, sqlstr.c_str() , NULL, NULL, &msg );   
		//	SqlHelper::sqlLock.unlock();
		//	if( result != SQLITE_OK ) {
		//		LOGD(TAG,"创建表失败，错误码:%d ，错误原因:%s\n" , result, msg );
		//		sqlite3_close(SqlDB);
		//		sqlite3_free(msg);
		//	}
		//}
		////////////////-----circle-----////////////////
		//demo-//isHave = isHaveTable(SqlDBData::circledbName);
		//if(!isHave){//没有的话创建
		//	SqlHelper::sqlLock.lock();
		//	sqlstr="create table "+SqlDBData::circledbName+"( circleId INTEGER PRIMARY KEY AUTOINCREMENT, userId INTEGER , gameMode INTEGER , colorName TEXT , removeNum INTEGER )"; 
		//	result = sqlite3_exec( SqlDB, sqlstr.c_str() , NULL, NULL, &msg );   
		//	SqlHelper::sqlLock.unlock();
		//	if( result != SQLITE_OK ) {
		//		LOGD(TAG,"创建表失败，错误码:%d ，错误原因:%s\n" , result, msg );
		//		sqlite3_close(SqlDB);
		//		sqlite3_free(msg);
		//	}else 
		//		circleInfoAdd();	
		//}
	}
}

bool SqlHelper::isHaveTable(string tableName){
	bool isHave = false; 
	string sqlstr="select count(type) from sqlite_master where type='table' and name='"+tableName+"'"; 
	sqlite3_exec( SqlDB, sqlstr.c_str() , isExisted, &isHave, &msg ); 
	LOGD("","isHave: %d",isHave);
	return isHave;
}

int isExisted( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
    bool *isExisted_=(bool*)para; 
    *isExisted_=(**column_value)!='0'; 
    return 0; 
} 

void SqlHelper::userInfoAdd()
{
	SqlHelper::sqlLock.lock();
	string sqlstr;
	#if MK_APK_PUBLISH
		sqlstr = sqlite3_mprintf("insert into %s values( %d , '%s' , %d , %d , '%s' )",SqlDBData::userdbName.c_str(),0,"",0,5,"0");
	#else
		sqlstr = sqlite3_mprintf("insert into %s values( %d , '%s' , %d , %d , '%s' )",SqlDBData::userdbName.c_str(),0,"",0,5,"0");
	#endif
	int result = sqlite3_exec( SqlDB, sqlstr.c_str() , NULL, NULL, &msg );   
	if(result != SQLITE_OK )  {
		LOGD(TAG,"插入记录失败,错误码:%d,错误原因:%s\n" , result, msg ); 
		sqlite3_free(msg);
	}
	SqlHelper::sqlLock.unlock();
}

void SqlHelper::itemInfoAdd()
{
	SqlHelper::sqlLock.lock();
	string* sqlstr = new string[Item::itemQty];
	#if MK_APK_PUBLISH
		for(int i = 0;i<Item::itemQty;i++){
			sqlstr[i] = sqlite3_mprintf("insert into %s values( %d , %d , '%s' , %d , %d )",SqlDBData::itemdbName.c_str(),i,0,Item::itemStr[i].c_str(),0,Item::priceItem[i]);
		}
	#else
		for(int i = 0;i<Item::itemQty;i++){
			sqlstr[i] = sqlite3_mprintf("insert into %s values( %d , %d , '%s' , %d , %d )",SqlDBData::itemdbName.c_str(),i,0,Item::itemStr[i].c_str(),0,Item::priceItem[i]);
		}
	#endif
	for(int i = 0;i<Item::itemQty;i++){
		int result = sqlite3_exec( SqlDB, sqlstr[i].c_str() , NULL, NULL, &msg );   
		if(result != SQLITE_OK )  {
			LOGD(TAG,"插入记录失败,错误码:%d,错误原因:%s\n" , result, msg ); 
			sqlite3_free(msg);
		}
	}
	CC_SAFE_DELETE_ARRAY(sqlstr);
	SqlHelper::sqlLock.unlock();
}

void SqlHelper::circleInfoAdd()
{
	//demo-//SqlHelper::sqlLock.lock();
	//string* sqlstr = new string[Circle::colorModeOneQty*2];
	//for(int i = 0;i<Circle::colorModeOneQty*2;i++){
	//	sqlstr[i] = sqlite3_mprintf("insert into %s values( null , %d , %d , '%s' , %d )",SqlDBData::circledbName.c_str(),0,i/Circle::colorModeOneQty,Circle::colorStr[i%Circle::colorModeOneQty].c_str(),0);
	//}
	//for(int i = 0;i<Circle::colorModeOneQty*2;i++){
	//	int result = sqlite3_exec( SqlDB, sqlstr[i].c_str() , NULL, NULL, &msg );   
	//	if(result != SQLITE_OK )  {
	//		LOGD(TAG,"插入记录失败,错误码:%d,错误原因:%s\n" , result, msg ); 
	//		sqlite3_free(msg);
	//	}
	//}
	//SqlHelper::sqlLock.unlock();
}

void SqlHelper::addInfo(string sql_str)
{
	SqlHelper::sqlLock.lock();
	int result = sqlite3_exec( SqlDB, sql_str.c_str() , NULL, NULL, &msg );   
	if(result != SQLITE_OK )  {
		LOGD(TAG,"插入记录失败,错误码:%d,错误原因:%s\n" , result, msg ); 
		sqlite3_free(msg);
	}
	SqlHelper::sqlLock.unlock();
}

//获取用户表所有信息
void SqlHelper::selectUserInfo(string sql_str,userInfoStruct& userInfo)
{
	sqlite3_exec( SqlDB, sql_str.c_str() , getUserInfoResult, &userInfo, &msg);
}
int getUserInfoResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
//demo-	userInfoStruct* userInfo = (userInfoStruct*) para;
//	userInfo->userId = stoi(column_value[0]);
//	userInfo->userName = column_value[1];
//	userInfo->diamondNum = stoi(column_value[2]);
//	userInfo->brushNum = stoi(column_value[3]);
//	userInfo->brushJiLuTime = stoi(column_value[4]);
    return 0; 
}
//获取道具表所有信息
void SqlHelper::selectItemInfos(string sql_str,vector<itemInfoStruct*>& itemInfos)
{
	sqlite3_exec( SqlDB, sql_str.c_str() , getItemInfoResult, &itemInfos, &msg);
}
int getItemInfoResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	//demo-	vector<itemInfoStruct*>* itemsInfos = (vector<itemInfoStruct*>*) para;
//	itemInfoStruct* itemsInfo = new itemInfoStruct;
//	itemsInfo->itemId = stoi(column_value[0]);
//	itemsInfo->userId = stoi(column_value[1]);
//	itemsInfo->itemName = column_value[2];
//	itemsInfo->itemNum = stoi(column_value[3]);
//	itemsInfo->itemPrice = stoi(column_value[4]);
//	itemsInfos->push_back(itemsInfo);
	
    return 0; 
}
//获取球球表所有信息
void SqlHelper::selectCircleInfos(string sql_str,vector<circleInfoStruct*> &circleInfos)
{
	sqlite3_exec( SqlDB, sql_str.c_str() , getCircleInfoResult, &circleInfos, &msg);
}
int getCircleInfoResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	//demo-	vector<circleInfoStruct*>* circleInfos = (vector<circleInfoStruct*>*) para;
//	circleInfoStruct* circleInfo = new circleInfoStruct;
//	circleInfo->circleId = stoi(column_value[0]);
//	circleInfo->userId = stoi(column_value[1]);
//	circleInfo->gameMode = stoi(column_value[2]);
//	circleInfo->colorName = column_value[3];
//	circleInfo->removeNum = stoi(column_value[4]);
//	circleInfos->push_back(circleInfo);
	
    return 0; 
}
//获取单个数值
void  SqlHelper::selectDataInteger(string sql_str,int &itemrInfos)
{
	sqlite3_exec( SqlDB, sql_str.c_str() , getDataIntegerResult, &itemrInfos, &msg);
}
int getDataIntegerResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	//demo-	int* targetValue = (int*) para;
//	(*targetValue) = stoi(column_value[0]);
    return 0; 
}
void  SqlHelper::selectDataString(string sql_str,string &itemrInfos)
{
	sqlite3_exec( SqlDB, sql_str.c_str() , getDataStringResult, &itemrInfos, &msg);
}
int getDataStringResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	string* targetValue = (string*) para;
	(*targetValue) = column_value[0];	
    return 0; 
}
//获取多个数值 
void SqlHelper::selectDataIntegers(string sql_str,vector<int>& scores)
{
	LOGD("", "-----------getScores-------------------selectDataIntegers sql_str %s", sql_str.c_str());
	sqlite3_exec(SqlDB, sql_str.c_str(), getDataIntegersResult, &scores, &msg);
}
int getDataIntegersResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	//demo-	LOGD("", "-----------getScores-------------------selectDataIntegers column_value[0] %s", column_value[0]);
//	vector<int>* scores = (vector<int>*)para;
//	int score = stoi(column_value[0]);
//	scores->push_back(score);
    return 0; 
}

void SqlHelper::updateData(string sql_str)
{
	SqlHelper::sqlLock.lock();
	int result = sqlite3_exec( SqlDB, sql_str.c_str() , NULL, NULL, &msg); 
	if(result != SQLITE_OK )  {
		LOGD(TAG,"更新失败,错误码:%d,错误原因:%s\n" , result, msg ); 
		sqlite3_free(msg);
	}
	SqlHelper::sqlLock.unlock();
}

void SqlHelper::orderAndDelete(int max_num,int user_id,int mode_type)
{
	//demo-//SqlHelper::sqlLock.lock();
	//string sql_str = sqlite3_mprintf("select count(*) as scoreCount from scoreInfo where userId = %d and gameMode = %d",MainStatus::UserId,mode_type);
	//int scoreCount;
	//sqlite3_exec( SqlDB, sql_str.c_str() , getDataIntegerResult, &scoreCount, &msg);
	//if(scoreCount>max_num){
	//	//查询要删除的 
	//	int limit_num = scoreCount-max_num;
	//	sql_str = sqlite3_mprintf("select scoreId from scoreInfo where userId = %d and gameMode = %d order by score desc limit %d offset 10",MainStatus::UserId,mode_type,limit_num);
	//	vector<int> scoreId;
	//	sqlite3_exec( SqlDB, sql_str.c_str() , getDataIntegerVecResult, &scoreId, &msg);
	//	//删除 
	//	for(int i = 0;i<scoreId.size();i++){
	//		sql_str = sqlite3_mprintf("delete from scoreInfo where scoreId = %d",scoreId.at(i));
	//		int result = sqlite3_exec( SqlDB, sql_str.c_str() , NULL, NULL, &msg); 
	//		if(result != SQLITE_OK )  {
	//			LOGD(TAG, "删除失败,错误码:%d,错误原因:%s\n" , result, msg ); 
	//			sqlite3_free(msg);
	//		}
	//	}
	//}
	//SqlHelper::sqlLock.unlock();
}
int getDataIntegerVecResult( void * para, int n_column, char ** column_value, char ** column_name ) 
{
	//demo-	vector<int>* targetValues = (vector<int>*) para;
//	int targetValue = stoi(column_value[0]);
//	targetValues->push_back(targetValue);
    return 0; 
}

/**********天鸣**********/
#include "SqlMath.h"

SqlMath* SqlMath::_sqlMath = nullptr;

SqlMath* SqlMath::getInstance()
{
    if (! _sqlMath)
    {
        _sqlMath = new SqlMath();
    }
    return _sqlMath;
}

void SqlMath::destroyInstance()
{
    CC_SAFE_DELETE(_sqlMath);
}


SqlMath::SqlMath()
{
	sqlHelper = new SqlHelper();
}

SqlMath::~SqlMath()
{
	CC_SAFE_DELETE(sqlHelper);
}

void SqlMath::getUserInfo(vector<DBcolumn>* dbcolumns,userInfoStruct& userInfo)
{
	//demo-//	string sqlstr = sqlite3_mprintf("select * from %s where userId = %d", SqlDBData::userdbName.c_str(), Mainstatus::UserId);
//	if(dbcolumns!=nullptr){
//		for(int i = 0;i<dbcolumns->size();i++){
//			sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
//		}
//		CC_SAFE_DELETE(dbcolumns);
//	}
//	sqlHelper->selectUserInfo(sqlstr,userInfo);
}

void SqlMath::getItemInfo(vector<DBcolumn>* dbcolumns,vector<itemInfoStruct*>& itemInfos)
{
	//demo-//string sqlstr = sqlite3_mprintf("select * from %s where userId = %d", SqlDBData::itemdbName.c_str(), Mainstatus::UserId);
	//if(dbcolumns!=nullptr){
	//	for(int i = 0;i<dbcolumns->size();i++){
	//		sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
	//	}
	//	CC_SAFE_DELETE(dbcolumns);
	//}
	//sqlHelper->selectItemInfos(sqlstr,itemInfos);
}

void SqlMath::getCircleInfo(vector<DBcolumn>* dbcolumns,vector<circleInfoStruct*>& circleInfos)
{
	//demo-//string sqlstr = sqlite3_mprintf("select * from %s where userId = %d", SqlDBData::circledbName.c_str(), Mainstatus::UserId);
	//if(dbcolumns!=nullptr){
	//	for(int i = 0;i<dbcolumns->size();i++){
	//		sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
	//	}
	//	CC_SAFE_DELETE(dbcolumns);
	//}
	//sqlHelper->selectCircleInfos(sqlstr,circleInfos);
}

void SqlMath::getDataInteger(string table_name,string column_name,vector<DBcolumn>* dbcolumns,int &targetValue)
{
	//demo-//string sqlstr = sqlite3_mprintf("select %s from %s where userId = %d", column_name.c_str(), table_name.c_str(), Mainstatus::UserId);
	//if(dbcolumns!=nullptr){
	//	for(int i = 0;i<dbcolumns->size();i++){
	//		sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
	//	}
	//	CC_SAFE_DELETE(dbcolumns);
	//}
	//sqlHelper->selectDataInteger(sqlstr,targetValue);
}
void SqlMath::getDataString(string table_name,string column_name,vector<DBcolumn>* dbcolumns,string &targetValue)
{
	//demo-//string sqlstr = sqlite3_mprintf("select %s from %s where userId = %d", column_name.c_str(), table_name.c_str(), Mainstatus::UserId);
	//if(dbcolumns!=nullptr){
	//	for(int i = 0;i<dbcolumns->size();i++){
	//		sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
	//	}
	//	CC_SAFE_DELETE(dbcolumns);
	//}
	//sqlHelper->selectDataString(sqlstr,targetValue);
}
void SqlMath::setData(string table_name,DBcolumn& dbcolumn,vector<DBcolumn>* dbcolumns){
	//demo-//string sqlstr = sqlite3_mprintf("update %s set %s = '%s' where userId = %d", table_name.c_str(), dbcolumn.columnName.c_str(), dbcolumn.columnValue.c_str(), Mainstatus::UserId);
	//if(dbcolumns!=nullptr){
	//	for(int i = 0;i<dbcolumns->size();i++){
	//		sqlstr = sqlite3_mprintf("%s and %s = '%s' ",sqlstr.c_str(),dbcolumns->at(i).columnName.c_str(),dbcolumns->at(i).columnValue.c_str());
	//	}
	//	CC_SAFE_DELETE(dbcolumns);
	//}
	//sqlHelper->updateData(sqlstr);
}

void SqlMath::addScore(int score,int mode_type)
{
	//demo-////插入
	//string sql_str = sqlite3_mprintf("insert into %s values( null , %d , %d , %d )", SqlDBData::scoredbName.c_str(), Mainstatus::UserId, mode_type, score);
	//sqlHelper->addInfo(sql_str);
	////排序并将多余删除 
	//sqlHelper->orderAndDelete(10, Mainstatus::UserId, mode_type);
}

void SqlMath::getScores(int num,vector<int>& scores,int& score)
{
	//demo-////初始化 
	//for (int i = 0; i < num; i++){
	//	scores.push_back(0);
	//}
	//string sql_str = sqlite3_mprintf("select score from scoreInfo where gameMode = 0 and userId = 0 order by score desc limit %d offset 0", num);//模式一才要全部的
	//LOGD("", "-----------getScores-------------------sql_str %s", sql_str.c_str());
	//sqlHelper->selectDataIntegers(sql_str, scores);
	//score = scores.at(0);
}

void SqlMath::addCircleNum(int* colorRemoveNum,int mode_type){
	//demo-////得到所有的 
	//vector<DBcolumn>* dbcolumnTJs = new vector<DBcolumn>;
	//DBcolumn dbcolumnTJ = {"gameMode",to_string(mode_type)};
	//dbcolumnTJs->push_back(dbcolumnTJ);
	//vector<circleInfoStruct*> circleInfos;
	//getCircleInfo(dbcolumnTJs,circleInfos);
	////更新数值 
	//for(int i = 0;i<circleInfos.size();i++){
	//	circleInfos.at(i)->removeNum += colorRemoveNum[(circleInfos.at(i)->circleId-1)%circleInfos.size()];
	//}
	////更新数据库 
	//for(int i = 0;i<circleInfos.size();i++){
	//	DBcolumn dbcolumnTarget = {"removeNum",to_string(circleInfos.at(i)->removeNum)};

	//	dbcolumnTJs = new vector<DBcolumn>;
	//	DBcolumn dbcolumnTJ = {"circleId",to_string(circleInfos.at(i)->circleId)};
	//	dbcolumnTJs->push_back(dbcolumnTJ);
	//	DBcolumn dbcolumnTJ2 = {"gameMode",to_string(mode_type)};
	//	dbcolumnTJs->push_back(dbcolumnTJ2);
	//	setData(SqlDBData::circledbName,dbcolumnTarget,dbcolumnTJs);
	//	CC_SAFE_DELETE(circleInfos.at(i));
	//}
}
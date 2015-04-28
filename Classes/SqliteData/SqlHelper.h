#ifndef __SQL_HEIPER_H__
#define __SQL_HEIPER_H__
#include "MainStatus.h"
#include "Sqlite3/sqlite3.h"
#include "../ObjectNode/ItemNode.h"
#include "TMUtility.h"
#include "SqlDBData.h"

#include <mutex>
#include <string>
using namespace std;

class SqlHelper
{
public:
	SqlHelper();
	~SqlHelper();

	void selectUserInfo(string sql_str,userInfoStruct &userInfo);
	void selectItemInfos(string sql_str,vector<itemInfoStruct*> &itemInfos);
	void selectCircleInfos(string sql_str,vector<circleInfoStruct*> &circleInfos);

	void selectDataInteger(string sql_str,int &targetValue);
	void selectDataIntegers(string sql_str,vector<int>& scores);
	void selectDataString(string sql_str,string &targetValue);
	void updateData(string sql_str);
	void addInfo(string sql_str);

	void orderAndDelete(int max_num,int user_id,int mode_type);

private:
	sqlite3* SqlDB ;
	char* msg;
	//Item item;

	void openSql();//打开数据库，没有则创建 
	bool isHaveTable(string tableName);//现在有没有这个表 
	void userInfoAdd();//添加用户信息
	void itemInfoAdd();//添加道具信息
	void circleInfoAdd();//添加球球信息 

	static mutex sqlLock;
};

int isExisted(void * para, int n_column, char ** column_value, char ** column_name);
int getUserInfoResult(void * para, int n_column, char ** column_value, char ** column_name);
int getItemInfoResult(void * para, int n_column, char ** column_value, char ** column_name);
int getCircleInfoResult(void * para, int n_column, char ** column_value, char ** column_name);
int getDataIntegerResult(void * para, int n_column, char ** column_value, char ** column_name);
int getDataIntegersResult(void * para, int n_column, char ** column_value, char ** column_name);
int getDataStringResult(void * para, int n_column, char ** column_value, char ** column_name);
int getDataIntegerVecResult(void * para, int n_column, char ** column_value, char ** column_name);


#endif
/**********天鸣**********/
#ifndef __SQL_MATH_H__
#define __SQL_MATH_H__

#include "SqliteData/SqlHelper.h"
#include "TMUtility.h"

class SqlMath
{
public:
	SqlMath();
	~SqlMath();
    static SqlMath* getInstance();
	static void destroyInstance();

	void getUserInfo(vector<DBcolumn>* dbcolumns,userInfoStruct& userInfo);
	void getItemInfo(vector<DBcolumn>* dbcolumns,vector<itemInfoStruct*>& itemInfos);
	void getCircleInfo(vector<DBcolumn>* dbcolumns,vector<circleInfoStruct*>& circleInfos);


	void getDataInteger(string table_name,string column_name,vector<DBcolumn>* dbcolumns,int &targetValue);
	void getDataString(string table_name,string column_name,vector<DBcolumn>* dbcolumns,string &targetValue);
	void setData(string table_name,DBcolumn& dbcolumn,vector<DBcolumn>* dbcolumns);

	void addScore(int score,int mode_type);
	void getScores(int num,vector<int>& scores,int& score);

	void addCircleNum(int* colorRemoveNum,int mode_type);
private:
    static SqlMath* _sqlMath;
	SqlHelper* sqlHelper;
};


#endif

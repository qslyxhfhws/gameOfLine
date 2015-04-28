/**********天鸣**********/
#ifndef __SQL_DBDATA_H__
#define __SQL_DBDATA_H__

#include <string>
using namespace std;
struct userInfoStruct{
	int userId;
	string userName;
	int diamondNum;
	int brushNum;
	string brushJiLuTime;
};

struct itemInfoStruct{
	int itemId;
	int userId;
	string itemName;
	int itemNum;
	int itemPrice;
};

struct circleInfoStruct{
	int circleId;
	int userId;
	int gameMode;
	string colorName;
	int removeNum;
};

struct DBcolumn{
	string columnName;
	string columnValue;
};

class SqlDBData{
	////////////////////////////数据库////////////////////////////
public:
	const static string userdbName;//默认用户id = 0；
	const static string itemdbName;
	const static string scoredbName;
	const static string circledbName;
	enum scoreGameMode
	{
		gameOne,
		gameTwo,
	};
};


#endif
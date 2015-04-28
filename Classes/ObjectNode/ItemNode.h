/**********天鸣**********/
#ifndef __ITEM_NODE_H__
#define __ITEM_NODE_H__

#include "TMUtility.h"
#include "TM_CCPlatformMacros.h"
#include <string>
using namespace std;

typedef std::function<void(Frame* frame)> onFrameListener;

class Item
{
public :
	Item(ActionTimeline* item_atl,int item_id);
	~Item();
	enum ItemId
	{
		itemOne_AddTime,
		itemTwo_ImproveScore,
		itemThree_ResetCircle,
		itemThree_RemoveCircle,
	};
	struct ItemInfo
	{
		int id;
		string name;
	};
	const static int itemQty;
	const static int priceItem[];
	const static string itemStr[];
	static map<string,int> itemTypeMap;

	ActionTimeline* itemAtl;
	int itemId;

	void getItemNum(int &item_num);
	void setItemNum(int &item_num);
private :
	CREATE_EVENT_FRAME();
	void init();
	void onFrameEvent(Frame* frame);
};
#endif
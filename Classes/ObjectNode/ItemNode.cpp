/**********天鸣**********/
#include "ItemNode.h"
#include "SqliteData/SqlMath.h"
const int Item::itemQty = 4;
const string Item::itemStr[] = {"addTime","improveScore","resetCircle","removeCircle"};//要与前面的枚举值一样 
const int Item::priceItem[] = {80,100,120,140};
map<string,int> Item::itemTypeMap;
Item::Item(ActionTimeline* item_atl,int item_id)
{
	if(itemTypeMap.size() <= 0){
		for(int i = 0;i<Item::itemQty;i++){
			Item::itemTypeMap.insert(pair<string,int>(Item::itemStr[i],i));
		}
	}
	itemAtl = item_atl;
	itemId = item_id;
	init();
}

Item::~Item()
{
}

void Item::init()
{
	itemAtl->gotoFrameAndPause(itemAtl->getEndFrame());

	auto timeline = Timeline::create();
	timeline->addFrame(createEventFrame(0, "-1", true));
	char itemIdCh[32];
	sprintf(itemIdCh,"%d",itemId);
	timeline->addFrame(createEventFrame(itemAtl->getEndFrame(),itemIdCh,true));
	itemAtl->addTimeline(timeline);
	itemAtl->setFrameEventCallFunc(CC_CALLBACK_1(Item::onFrameEvent, this));
}

void Item::getItemNum(int &item_num)
{
//demo-	vector<DBcolumn>* dbcolumns = new vector<DBcolumn>;
//	DBcolumn dbcolumn = {"itemId",to_string(itemId)};
//	dbcolumns->push_back(dbcolumn);
//	SqlMath::getInstance()->getDataInteger(SqlDBData::itemdbName,"itemNum",dbcolumns,item_num);
}

void Item::setItemNum(int &item_num)
{
	//demo-	vector<DBcolumn>* dbcolumns = new vector<DBcolumn>;
//	DBcolumn dbcolumn = {"itemId",to_string(itemId)};
//	dbcolumns->push_back(dbcolumn);
//	DBcolumn target_column = {"itemNum",to_string(item_num)};
//	SqlMath::getInstance()->setData(SqlDBData::itemdbName,target_column,dbcolumns);
}

void Item::onFrameEvent(Frame* frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    string str = evnt->getEvent();
	int num = atoi(str.c_str());
	if(num>=0&&num<Item::itemQty)
		NotificationCenter::getInstance()->postNotification("onFrameEvent",(Ref*)num);

}

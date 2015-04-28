/**********天鸣**********/
#include "Shop.h"
#include "MainStatus.h"
#include "SqliteData/SqlMath.h"
#define TAG "Shop"

Shop::Shop()
{
	itemList = nullptr;
	itemTbf = nullptr;
	itemBtn = nullptr;
}

Shop::~Shop()
{
	for(int i = 0;i<itemList->size();i++){
		CC_SAFE_DELETE(itemList->at(i));
	}
	CC_SAFE_DELETE(itemList);
	CC_SAFE_DELETE_ARRAY(itemTbf);
	CC_SAFE_DELETE_ARRAY(itemBtn);
}

bool Shop::init()
{
	if(!Layer::init()){
		return false;
	}
    winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	shopNode = CSLoader::createNode("bg10Scene.csb"); 
	this->addChild(shopNode,1); 

	//钻石 
	int dimamondNum ;
	SqlMath::getInstance()->getDataInteger(SqlDBData::userdbName,"diamondNum",nullptr,dimamondNum);
	diamondNumTbf = dynamic_cast<TextBMFont*>(shopNode->getChildByTag(902));
	char diamondCH[32];
	sprintf(diamondCH,"%d",dimamondNum);
	diamondNumTbf->setString(diamondCH);

	//demo-//技能图标
	//int itemNodeTag[] = {823,813,806,798};
	//int itemTbfTag[] = {901,904,906,908};
	//int itemNum;
	itemList = new vector<Item*>();
	//itemList->clear();
	//itemTbf = new TextBMFont*[Item::itemQty];
	//for(int i = 0;i<Item::itemQty;i++){
	//	Node* itemNode = shopNode->getChildByTag(itemNodeTag[i]);
	//	itemTbf[i] = dynamic_cast<TextBMFont*>(shopNode->getChildByTag(itemTbfTag[i]));
	//	ActionTimeline* itemAtl = dynamic_cast<ActionTimeline*>(itemNode->getActionByTag(itemNode->getTag()));

	//	Item* item = new Item(itemAtl,i);
	//	itemList->push_back(item);
	//}

	//for(int i = 0;i<itemList->size();i++){
	//	int itemNum;
	//	itemNum = 0;
	//	//demo-itemList->at(i)->getItemNum(itemNum);
	//	char itemNumCh[32];
	//	sprintf(itemNumCh,"%d",itemNum);
	//	itemTbf[i]->setString(itemNumCh);
	//}

	//购买按钮 
	int itemBtnTag[] = {862,865,868,871};
	itemBtn = new Button*[Item::itemQty];
	for(int i = 0;i<Item::itemQty;i++){
		itemBtn[i] = dynamic_cast<Button*>(shopNode->getChildByTag(itemBtnTag[i]));
		itemBtn[i]->addTouchEventListener(CC_CALLBACK_2(Shop::touchEvent,this));
		itemBtn[i]->setTouchEnabled(false);
	}
	initTouch();


	return true;
}

void Shop::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(Shop::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
	//一般
}

void Shop::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED){//end的时候呼叫 
		for(int i = 0;i<Item::itemQty;i++){
			if(itemBtn[i] == pSender){
				itemList->at(i)->itemAtl->gotoFrameAndPlay(0,false);
				buyItem(i);
			}
		}
	}
}

void Shop::buyItem(int itemId)
{
	int dimamondNum ;
	dimamondNum = 9999;
	//demo-SqlMath::getInstance()->getDataInteger(SqlDBData::userdbName,"diamondNum",nullptr,dimamondNum);
	if(dimamondNum>=Mainstatus::Price_Item[itemId]){
		int itemNum ;
		itemList->at(itemId)->getItemNum(itemNum);

		dimamondNum -= Mainstatus::Price_Item[itemId];
		itemNum ++;

		//DBcolumn target_column = {"diamondNum",to_string(dimamondNum)};
		//SqlMath::getInstance()->setData(SqlDBData::userdbName,target_column,nullptr);

		itemList->at(itemId)->setItemNum(itemNum);//存item 

		char diamondCH[32];
		sprintf(diamondCH,"%d",dimamondNum);
		diamondNumTbf->setString(diamondCH);
		char itemCH[32];
		sprintf(itemCH,"%d",itemNum);
		itemTbf[itemId]->setString(itemCH);
	}else {//钻石不足 

	}
}

void Shop::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(Shop::removeSelf,this)));
	}
}

void Shop::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void Shop::onExit()
{
	Layer::onExit();
}
void Shop::onEnter()
{
	Layer::onEnter();
}


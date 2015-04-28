#include "FirstLayer.h"
#include "MainStatus.h"
#include "LockLayer.h"
#include "SysSetting.h"
#include "ChooseMode.h"
#include "PaiMing.h"
#include "ExitLayer.h"
#include "SqliteData/SqlMath.h"
#define TAG "FirstLayer"

Scene* First::createScene()
{
    auto scene = Scene::create();
    auto firstlayer = First::create();
	scene->addChild(firstlayer, Mainstatus::ZORDER_FIRST, Mainstatus::TAG_FIRST);
    return scene;
}

First::First()
{
}

First::~First()
{
	SqlMath::destroyInstance();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool First::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    winSize = Director::getInstance()->getVisibleSize();
	SqlMath::getInstance();
	//userInfoStruct a;
	//SqlMath::getInstance()->getUserInfo(nullptr,0,a);
	//vector<itemInfoStruct*> b;
	//vector<DBcolumn>* dbcolumn = new vector<DBcolumn>;
	//DBcolumn a = {"itemId","1"};
	//dbcolumn->push_back(a);
	//SqlMath::getInstance()->getItemInfo(dbcolumn,b);

	firstNode = CSLoader::createNode("MainScene.csb"); 
	this->addChild(firstNode); 

	actNode = firstNode->getChildByTag(108); 
	//获取动画对象，这个动画相当于这个节点在时间轴里的所有动画内容。且这个节点需要是导入的csd文件类型节点 
	actAtl = dynamic_cast<ActionTimeline*>(actNode->getActionByTag(actNode->getTag())); 
	actAtl->gotoFrameAndPlay(0, false);//设定不要循环 

	playGameBtn = dynamic_cast<Button*>(actNode->getChildByTag(370)->getChildByTag(58));
	lockBtn = dynamic_cast<Button*>(actNode->getChildByTag(370)->getChildByTag(60));
	highScoreBtn = dynamic_cast<Button*>(actNode->getChildByTag(370)->getChildByTag(59));
	settingBtn = dynamic_cast<Button*>(actNode->getChildByTag(370)->getChildByTag(61));
	playGameBtn->addTouchEventListener(CC_CALLBACK_2(First::touchEvent, this));
	lockBtn->addTouchEventListener(CC_CALLBACK_2(First::touchEvent, this));
	highScoreBtn->addTouchEventListener(CC_CALLBACK_2(First::touchEvent, this));
	settingBtn->addTouchEventListener(CC_CALLBACK_2(First::touchEvent, this));

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(First::backToFirst), "backToFirst", NULL);

	initTouch();

	return true;
}

void First::backToFirst(Ref* ref)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> allchildren = nowScene->getChildren();
	for (int i = allchildren.size() - 1; i >= 0; i--){
		if (allchildren.at(i)->getTag() != Mainstatus::TAG_FIRST&&allchildren.at(i)->getTag()!=-1){
			LOGD(TAG, "TAG_FIRST  %d           target tag %d    this tag %d", Mainstatus::TAG_FIRST, allchildren.at(i)->getTag(),this->getTag());
			allchildren.at(i)->removeFromParentAndCleanup(true);
		}
	}
	//nowScene->getChildByTag(1301)->removeFromParentAndCleanup(true);
	//nowScene->getChildByTag(1101)->removeFromParentAndCleanup(true);
}

void First::initTouch()
{
	//跳过动画 
	auto layerTouchLis = EventListenerTouchOneByOne::create();
	layerTouchLis->setSwallowTouches(true);
	layerTouchLis->onTouchBegan = [&](Touch* touch,Event* event){
		if(!actAtl->isPlaying()) 
			return false;
		auto target = static_cast<Layer*>(event->getCurrentTarget());//获取的当前触摸的目标   
		Size s = target->getContentSize(); 
		Rect rect = Rect(0, 0, s.width, s.height);  
		Point locationInNode;  
		if (rect.containsPoint(locationInNode)){//判断触摸点是否在目标的范围内  
			return true;  
		}else{
			return false;
		}
	};
	layerTouchLis->onTouchMoved = [](Touch* touch,Event* event){
	};
	layerTouchLis->onTouchEnded = [&](Touch* touch,Event* event){
		if(!actAtl->isPlaying()) 
			return;
		actAtl->gotoFrameAndPause(actAtl->getEndFrame());
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis,this);

	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(First::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
}

void First::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED){//end的时候呼叫 
		if(pSender == playGameBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			ChooseMode* chooseMode = ChooseMode::create();
			nowScene->addChild(chooseMode, Mainstatus::ZORDER_CHOOSEMODE, Mainstatus::TAG_CHOOSEMODE);
			LOGD(TAG, "------------ chooseMode  ox%x", chooseMode);
			LOGD(TAG, "------------ chooseMode  ox%x", nowScene->getChildByTag(Mainstatus::TAG_CHOOSEMODE));
		}
		else if (pSender == lockBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			Lock* lockLayer = Lock::create();
			nowScene->addChild(lockLayer, Mainstatus::ZORDER_LOCK, Mainstatus::TAG_LOCK);
		}else if(pSender == highScoreBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			PaiMing* paiMing = PaiMing::create();
			nowScene->addChild(paiMing, Mainstatus::ZORDER_PAIMING, Mainstatus::TAG_PAIMING);
		}else if(pSender == settingBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			SysSetting* sysSetting = SysSetting::create();
			nowScene->addChild(sysSetting, Mainstatus::ZORDER_SETTING, Mainstatus::TAG_SETTING);
		}
	}
}

void First::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		ExitLayer* exitLayer = ExitLayer::create();
		nowScene->addChild(exitLayer, Mainstatus::ZORDER_EXIT, Mainstatus::TAG_EXIT);
	}
}

void First::onExit()
{
	Layer::onExit();
}
void First::onEnter()
{
	Layer::onEnter();
	Scene* nowScene = Director::getInstance()->getRunningScene();
}

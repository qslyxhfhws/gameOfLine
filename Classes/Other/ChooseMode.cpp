/**********天鸣**********/
#include "ChooseMode.h"
#include "MainStatus.h"
#include "Achievement.h"
#include "Shop.h"
#include "Game/GameLayer.h"
#include "SqliteData/SqlMath.h"
#define TAG "ChooseMode"

const string ChooseMode::ActEndStr[] = {"achievementEnd","shopEnd","levelUpEnd","chooseMode"};

ChooseMode::ChooseMode()
{
	brushSpr = nullptr;
}

ChooseMode::~ChooseMode()
{
	CC_SAFE_DELETE_ARRAY(brushSpr);
}

bool ChooseMode::init()
{
	if(!Layer::init()){
		return false;
	}
	nowModeTag = -1;
	isCanTouch = true;
	//demo-SqlMath::getInstance()->getDataInteger(SqlDBData::userdbName,"brushNum",nullptr,brushNum);
	brushNum = 99999;
	//demo-convertJiLuToCD();

    winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	chooseModeNode = CSLoader::createNode("bg3Scene.csb"); 
	this->addChild(chooseModeNode,1); 

	Node* achievementNode = chooseModeNode->getChildByTag(272)->getChildByTag(163); 
	achievementAtl = dynamic_cast<ActionTimeline*>(achievementNode->getActionByTag(achievementNode->getTag())); 
	achievementAtl->pause();
	auto achievement_timeline = Timeline::create();
	achievement_timeline->addFrame(createEventFrame(0, "", true));
	achievement_timeline->addFrame(createEventFrame(achievementAtl->getEndFrame(), ChooseMode::ActEndStr[AtlList::ATL_ACHIEVEMENT].c_str(), true));
	achievementAtl->addTimeline(achievement_timeline);
	achievementAtl->setFrameEventCallFunc(CC_CALLBACK_1(ChooseMode::onFrameEvent, this));
	achievementSpr = dynamic_cast<Sprite*>(achievementNode->getChildByTag(5));

	Node* shopNode = chooseModeNode->getChildByTag(272)->getChildByTag(153); 
	shopAtl = dynamic_cast<ActionTimeline*>(shopNode->getActionByTag(shopNode->getTag())); 
	shopAtl->pause();
	auto shop_timeline = Timeline::create();
	shop_timeline->addFrame(createEventFrame(0, "", true));
	shop_timeline->addFrame(createEventFrame(shopAtl->getEndFrame(), ChooseMode::ActEndStr[AtlList::ATL_SHOP].c_str(), true));
	shopAtl->addTimeline(shop_timeline);
	shopAtl->setFrameEventCallFunc(CC_CALLBACK_1(ChooseMode::onFrameEvent, this));
	shopSpr = dynamic_cast<Sprite*>(shopNode->getChildByTag(23));

	Node* levelUpNode = chooseModeNode->getChildByTag(272)->getChildByTag(200); //113
	levelUpAtl = dynamic_cast<ActionTimeline*>(levelUpNode->getActionByTag(levelUpNode->getTag())); 
	levelUpAtl->pause();
	auto levelUp_timeline = Timeline::create();
	levelUp_timeline->addFrame(createEventFrame(0, "", true));
	levelUp_timeline->addFrame(createEventFrame(levelUpAtl->getEndFrame(), ChooseMode::ActEndStr[AtlList::ATL_LEVELUP].c_str(), true));
	levelUpAtl->addTimeline(levelUp_timeline);
	levelUpAtl->setFrameEventCallFunc(CC_CALLBACK_1(ChooseMode::onFrameEvent, this));
	levelUpSpr = dynamic_cast<Sprite*>(levelUpNode->getChildByTag(113));
	
	Node* centerAtlNode = chooseModeNode->getChildByTag(235); 
	centerAtl = dynamic_cast<ActionTimeline*>(centerAtlNode->getActionByTag(centerAtlNode->getTag()));
	centerAtl->gotoFrameAndPlay(0, false);//设定不要循环 

	//中间模式 
	centerView = dynamic_cast<PageView*>(centerAtlNode->getChildByTag(481));
    centerView->addEventListener(CC_CALLBACK_2(ChooseMode::pageViewEvent, this));
	modeOneSpr = dynamic_cast<Sprite*>(centerView->getChildByTag(482)->getChildByTag(196));
	modeTwoSpr = dynamic_cast<Sprite*>(centerView->getChildByTag(524)->getChildByTag(609));

	//钻石 
	int dimamondNum ;
	dimamondNum = 9999;
	char diamondCh[32];
	sprintf(diamondCh,"%d",dimamondNum);
	//demo-SqlMath::getInstance()->getDataInteger(SqlDBData::userdbName,"diamondNum",nullptr,dimamondNum);
	diamondNumTbf = dynamic_cast<TextBMFont*>(chooseModeNode->getChildByTag(184)->getChildByTag(158));
	diamondNumTbf->setString(diamondCh);

	//画笔 
	//画笔数量 
	brushNumTbf = dynamic_cast<TextBMFont*>(chooseModeNode->getChildByTag(184)->getChildByTag(154));
	char brushNumCh[32];
	sprintf(brushNumCh,"%d",brushNum);
	brushNumTbf->setString(brushNumCh);
	//大于时间回复最大值时的画笔动画 
	Node* brushLingWaiNode = chooseModeNode->getChildByTag(499);
	brushLingWaiAtl = dynamic_cast<ActionTimeline*>(brushLingWaiNode->getActionByTag(brushLingWaiNode->getTag()));
	auto brushLingWaiAtl_timeline = Timeline::create();
	brushLingWaiAtl_timeline->addFrame(createEventFrame(0, "", true));
	brushLingWaiAtl_timeline->addFrame(createEventFrame(50, ChooseMode::ActEndStr[AtlList::ATL_CHOOSEMODE].c_str(), true));
	brushLingWaiAtl->addTimeline(brushLingWaiAtl_timeline);
	brushLingWaiAtl->setFrameEventCallFunc(CC_CALLBACK_1(ChooseMode::onFrameEvent, this));
	brushLingWaiAtl->pause();
	if(brushNum<=Mainstatus::brushMax){
		brushLingWaiNode->setVisible(false);
	}

	//小于时间回复最大值时的画笔动画 
	Node* chooseModeDingNode = chooseModeNode->getChildByTag(184);
	chooseModeDingAtl = dynamic_cast<ActionTimeline*>(chooseModeDingNode->getActionByTag(chooseModeDingNode->getTag()));
	chooseModeDingAtl->pause();
	auto chooseModeDingAtl_timeline = Timeline::create();
	chooseModeDingAtl_timeline->addFrame(createEventFrame(0, "", true));
	for(int i = 49;i<250;i+=50){
		chooseModeDingAtl_timeline->addFrame(createEventFrame(i, ChooseMode::ActEndStr[AtlList::ATL_CHOOSEMODE].c_str(),true));		
	};
	chooseModeDingAtl->addTimeline(chooseModeDingAtl_timeline);
	chooseModeDingAtl->setFrameEventCallFunc(CC_CALLBACK_1(ChooseMode::onFrameEvent, this));


	brushSpr = new Sprite*[Mainstatus::brushMax];
	int brushTag[] = {152,285,288,291,294};
	for (int i = 0; i<Mainstatus::brushMax; i++){
		brushSpr[i] = dynamic_cast<Sprite*>(chooseModeNode->getChildByTag(184)->getChildByTag(146)->getChildByTag(brushTag[i]));
		if(i >= brushNum) brushSpr[i]->setVisible(false);
	}

//demo-	if (brushNum<Mainstatus::brushMax){
//		scheduleBrush(0.0f);
//		this->schedule(schedule_selector(ChooseMode::scheduleBrush),1.0f);
//	}
	initTouch();

	return true;
}

void ChooseMode::convertJiLuToCD()
{
	string brushJiLuTimeStr;
	SqlMath::getInstance()->getDataString(SqlDBData::userdbName,"brushJiLuTime",nullptr,brushJiLuTimeStr);//当时记录的时间
	char nowTime[64];
	time_t t = time(0); 
	sprintf(nowTime, "%lld", (long long int)t);//现在时间 

//demo-	long long startTimeNum = stoll(brushJiLuTimeStr);
//	long long nowTimeNum = stoll(nowTime);
	//DBcolumn target_column = {"brushJiLuTime",nowTime};
	//SqlMath::getInstance()->setData(SqlDBData::userdbName,target_column,nullptr);//当时记录的时间

//	brushShiJCha = nowTimeNum - startTimeNum;
	LOGD(TAG,"SHIJIANCHA--------------%d",brushShiJCha);
}

void ChooseMode::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(ChooseMode::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
	//一般
	auto layerTouchLis = EventListenerTouchOneByOne::create();
	layerTouchLis->setSwallowTouches(true);
	layerTouchLis->onTouchBegan = [&](Touch* touch,Event* event){
		if(!isCanTouch) return false;
		auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标   
		Size s = target->getContentSize(); 
		Rect rect = Rect(0, 0, s.width, s.height);  
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());  
		if (rect.containsPoint(locationInNode)){//判断触摸点是否在目标的范围内  
			return true;  
		}else{
			return false;
		}
	};
	layerTouchLis->onTouchMoved = [](Touch* touch,Event* event){
	};
	layerTouchLis->onTouchEnded = [&](Touch* touch,Event* event){
		if(!isCanTouch) return;
		auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标   
		if(target == achievementSpr){
			isCanTouch = false;
			achievementAtl->gotoFrameAndPlay(0,false);
		}else if(target == shopSpr){
			isCanTouch = false;
			shopAtl->gotoFrameAndPlay(0,false);
		}else if(target == levelUpSpr){
			isCanTouch = false;
			levelUpAtl->gotoFrameAndPlay(0,false);
		}else if(target == modeOneSpr){
			chooseGameMode(Mainstatus::TAG_GAME_ONE);
		}else if(target == modeTwoSpr){
			//demo-//chooseGameMode(MainStatus::TAG_GAME_TWO);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis,achievementSpr);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis->clone(),shopSpr);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis->clone(),levelUpSpr);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis->clone(),modeOneSpr);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis->clone(),modeTwoSpr);

}

void ChooseMode::chooseGameMode(int targetMode)
{
	if(brushNum>0){
		isCanTouch = false;
		nowModeTag = targetMode;
		char brushNumch[32];
		sprintf(brushNumch,"%d",brushNum-1);
		brushNumTbf->setString(brushNumch);
		if(brushNum>Mainstatus::brushMax){
			brushLingWaiAtl->gotoFrameAndPlay(0,false);
		}else {
			chooseModeDingAtl->gotoFrameAndPlay((Mainstatus::brushMax-brushNum)*50,(Mainstatus::brushMax-brushNum+1)*50-1,false);
		}
	}
}


void ChooseMode::scheduleBrush(float dt)
{
	brushShiJCha++;
	//LOGD("","brushShiJCha------------%d  MainStatus::brushNeedTime %d",brushShiJCha,MainStatus::brushNeedTime);
	if(brushShiJCha>=Mainstatus::brushNeedTime){
		//demo-this->unschedule(schedule_selector(ChooseMode::scheduleBrush));

		saveTimeNow(false);
		convertJiLuToCD();

		if(brushNum<Mainstatus::brushMax){
			brushNum++;
//demo-			DBcolumn target_column = {"brushNum",to_string(brushNum)};
//			SqlMath::getInstance()->setData(SqlDBData::userdbName,target_column,nullptr);
//			brushSpr[brushNum-1]->setVisible(true);
//			brushNumTbf->setString(to_string(brushNum));
		}
//demo-		if(brushNum<Mainstatus::brushMax){
//			this->schedule(schedule_selector(ChooseMode::scheduleBrush),1.0f);
//		}
	}
}

void ChooseMode::saveTimeNow(bool isFirst)
{
	char nowTime[64];
	time_t t = time(0); 
	long long int nowTimeInt = t;

	if(!isFirst)
		nowTimeInt = nowTimeInt-(brushShiJCha-Mainstatus::brushNeedTime);

	sprintf(nowTime, "%lld", nowTimeInt);//现在时间 
	DBcolumn target_column = {"brushJiLuTime",nowTime};
	SqlMath::getInstance()->setData(SqlDBData::userdbName,target_column,nullptr);//当时记录的时间
}

void ChooseMode::pageViewEvent(Ref *pSender, PageView::EventType type)
{
	if(type == PageView::EventType::TURNING){
		//centerView->getCurPageIndex();
		centerAtl->gotoFrameAndPlay(0, false);//设定不要循环 
	}
}

void ChooseMode::onFrameEvent(Frame* frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    string str = evnt->getEvent();
	for(int i = 0;i<sizeof(ActEndStr)/sizeof(string);i++){
		if(str == ActEndStr[i]){
			if(i == AtlList::ATL_ACHIEVEMENT){
				Scene* nowScene = Director::getInstance()->getRunningScene();
				Achievement* achievement = Achievement::create();
				nowScene->addChild(achievement, Mainstatus::ZORDER_ACHIEVEMENT, Mainstatus::TAG_ACHIEVEMENT);
			}else if(i == AtlList::ATL_SHOP){
				Scene* nowScene = Director::getInstance()->getRunningScene();
				Shop* shop = Shop::create();
				nowScene->addChild(shop, Mainstatus::ZORDER_SHOP, Mainstatus::TAG_SHOP);
			}else if(i == AtlList::ATL_LEVELUP){
			}else if(i == AtlList::ATL_CHOOSEMODE){
				if(brushNum>0){
					if (brushNum<Mainstatus::brushMax)
						brushSpr[brushNum-1]->setVisible(false);

					//demo-this->unschedule(schedule_selector(ChooseMode::scheduleBrush));
					if (brushNum == Mainstatus::brushMax){
						saveTimeNow(true);
						convertJiLuToCD();
					}

					Scene* nowScene = Director::getInstance()->getRunningScene();
					if (nowModeTag == Mainstatus::TAG_GAME_ONE){
						Game* game = Game::create();
						nowScene->addChild(game, Mainstatus::ZORDER_GAME, Mainstatus::TAG_GAME_ONE);
						//demo-ModeOne* modeOne = ModeOne::create();
						//nowScene->addChild(modeOne,MainStatus::ZORDER_GAME,MainStatus::TAG_GAME_ONE);
					}else if(nowModeTag == Mainstatus::TAG_GAME_TWO){
						//demo-ModeTwo* modeTwo = ModeTwo::create(0);
						//nowScene->addChild(modeTwo,MainStatus::ZORDER_GAME,MainStatus::TAG_GAME_TWO);
					}
					nowModeTag = -1;

					brushNum--;
//demo-					DBcolumn target_column = {"brushNum",to_string(brushNum)};
//					SqlMath::getInstance()->setData(SqlDBData::userdbName,target_column,nullptr);

					if (brushNum<Mainstatus::brushMax){
						//demo-this->schedule(schedule_selector(ChooseMode::scheduleBrush),1.0f);
					}
				}
			}
			isCanTouch = true;
			break;
		}
	}
}

void ChooseMode::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(ChooseMode::removeSelf,this)));
	}
}

void ChooseMode::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void ChooseMode::onExit()
{
	//demo-this->unschedule(schedule_selector(ChooseMode::scheduleBrush));
	//saveTimeNow();
	Layer::onExit();
}
void ChooseMode::onEnter()
{
	Layer::onEnter();
}


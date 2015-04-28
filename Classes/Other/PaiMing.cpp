/**********天鸣**********/
#include "PaiMing.h"
#include "SqliteData/SqlMath.h"
#define TAG "PaiMing"

const int PaiMing::allShowQty = 3;

PaiMing::PaiMing()
{
	showNode = nullptr;
	showAtl = nullptr;
	showSpr = nullptr;
}

PaiMing::~PaiMing()
{
	CC_SAFE_DELETE_ARRAY(showNode);
	CC_SAFE_DELETE_ARRAY(showAtl);
	CC_SAFE_DELETE_ARRAY(showSpr);
}

bool PaiMing::init()
{
	if(!Layer::init()){
		return false;
	}
    winSize = Director::getInstance()->getVisibleSize();
	nowStep = PaiMingOrder::one_SelfScore;
	nowMode = ShowMode::showMode_One;
	isCanTouch = true;

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	paiMingNode = CSLoader::createNode("bg5Scene.csb"); 
	this->addChild(paiMingNode,1); 

	//中间模式 
	int showTag[] = {135,304,51};
	int showSprTag[] = {34,52,16};
	showNode = new Node*[PaiMing::allShowQty];
	showAtl = new ActionTimeline*[PaiMing::allShowQty];
	showSpr = new Sprite*[PaiMing::allShowQty];
	for(int i = 0;i<PaiMing::allShowQty;i++){
		showNode[i] = paiMingNode->getChildByTag(showTag[i]);
		showNode[i]->setVisible(false);

		showAtl[i] = dynamic_cast<ActionTimeline*>(showNode[i]->getActionByTag(showNode[i]->getTag()));
		showAtl[i]->pause();
		auto timeline = Timeline::create();
		timeline->addFrame(createEventFrame(0, "", true));
		timeline->addFrame(createEventFrame(75,"center",true));
		timeline->addFrame(createEventFrame(showAtl[i]->getEndFrame(),"end",true));
		showAtl[i]->addTimeline(timeline);
		showAtl[i]->setFrameEventCallFunc(CC_CALLBACK_1(PaiMing::onFrameEvent, this));


		showSpr[i] = dynamic_cast<Sprite*>(showNode[i]->getChildByTag(showSprTag[i]));
		showSpr[i]->setVisible(false);
	}
	
	heightScoreTbf = dynamic_cast<TextBMFont*>(paiMingNode->getChildByTag(44)->getChildByTag(13));

	switchMode();

	//上面最高分 
	isCanTouch = false;

	initTouch();

	return true;
}

void PaiMing::switchMode()
{
	if(nowMode == ShowMode::showMode_One){
		Node* uiNode = paiMingNode->getChildByTag(44);
		uiNode->setVisible(true);
		ActionTimeline* uiAtl = dynamic_cast<ActionTimeline*>(uiNode->getActionByTag(uiNode->getTag()));
		uiAtl->gotoFrameAndPlay(0,false);

		paiMingNode->getChildByTag(366)->setVisible(false);

		modeOneBtn = nullptr;
		modeTwoBtn = nullptr;
		modeOneBtn = dynamic_cast<Button*>(uiNode->getChildByTag(14));
		modeTwoBtn = dynamic_cast<Button*>(uiNode->getChildByTag(15));
		modeOneBtn->addTouchEventListener(CC_CALLBACK_2(PaiMing::touchEvent,this));
		modeTwoBtn->addTouchEventListener(CC_CALLBACK_2(PaiMing::touchEvent,this));

		if(nowStep == PaiMingOrder::two_GlobalScore){
			showNode[nowStep]->setVisible(false);
			showSpr[nowStep]->setVisible(false);
			nowStep = PaiMingOrder::one_SelfScore;
		}
		switchShow();
	}else if(nowMode == ShowMode::showMode_Two){
		//demo-//Node* uiNode = paiMingNode->getChildByTag(366);
		//uiNode->setVisible(true);
		////ActionTimeline* uiAtl = dynamic_cast<ActionTimeline*>(uiNode->getActionByTag(uiNode->getTag()));
		////uiAtl->gotoFrameAndPlay(0,false);

		//paiMingNode->getChildByTag(44)->setVisible(false);

		//modeOneBtn = nullptr;
		//modeTwoBtn = nullptr;
		//modeOneBtn = dynamic_cast<Button*>(uiNode->getChildByTag(11));
		//modeTwoBtn = dynamic_cast<Button*>(uiNode->getChildByTag(12));
		//modeOneBtn->addTouchEventListener(CC_CALLBACK_2(PaiMing::touchEvent,this));
		//modeTwoBtn->addTouchEventListener(CC_CALLBACK_2(PaiMing::touchEvent,this));

		//if(nowStep == PaiMingOrder::one_SelfScore){
		//	showNode[nowStep]->setVisible(false);
		//	showSpr[nowStep]->setVisible(false);
		//	nowStep = PaiMingOrder::two_GlobalScore;
		//}
		//switchShow();
	}
}

void PaiMing::switchShow()
{
	if(nowStep == PaiMing::one_SelfScore){
		//显示 
		showNode[nowStep]->setVisible(true);
		showSpr[nowStep]->setVisible(true);

		int scoreTbfTag[] = {104,106,108,110,112,114,116};
		vector<int> scores;
		int heighestScore = sizeof(scoreTbfTag) / sizeof(int) * 100;
		for (int i = 0; i < sizeof(scoreTbfTag) / sizeof(int); i++){
			scores.push_back((sizeof(scoreTbfTag) / sizeof(int)-i)*100);
		}
		//demo-SqlMath::getInstance()->getScores(sizeof(scoreTbfTag)/sizeof(int),scores,heighestScore);//得到最高分和前十名的分数 

		char heightScorech[32];
		sprintf(heightScorech,"%d",heighestScore);
		heightScoreTbf->setString(heightScorech);
		for(int i = 0;i<sizeof(scoreTbfTag)/sizeof(int);i++){
			TextBMFont* scoreTbf = dynamic_cast<TextBMFont*>(showNode[nowStep]->getChildByTag(scoreTbfTag[i]));
			char scoreCh[32];
			sprintf(scoreCh,"%d",scores.at(i));
			scoreTbf->setString(scoreCh);
		}

		showAtl[nowStep]->gotoFrameAndPlay(0,75,false);
	}else if(nowStep == PaiMing::two_GlobalScore){
		showNode[nowStep]->setVisible(true);
		showSpr[nowStep]->setVisible(true);

		showAtl[nowStep]->gotoFrameAndPlay(0,75,false);
	}else if(nowStep == PaiMing::three_circleRemoveNum){
		showNode[nowStep]->setVisible(true);
		showSpr[nowStep]->setVisible(true);

		//得到所有的 
		vector<DBcolumn>* dbcolumnTJs = new vector<DBcolumn>;
		char nowModech[32];
		sprintf(nowModech,"%d",nowModech);
		DBcolumn dbcolumnTJ = {"gameMode",nowModech};
		dbcolumnTJs->push_back(dbcolumnTJ);
		vector<circleInfoStruct*> circleInfos;
		SqlMath::getInstance()->getCircleInfo(dbcolumnTJs,circleInfos);

		TextBMFont** numTbf = new TextBMFont*[circleInfos.size()];
		int numTag[] = {36,42,38,34,40}; 
		for(int i = 0;i<circleInfos.size();i++){
			numTbf[i] = dynamic_cast<TextBMFont*>(showNode[nowStep]->getChildByTag(numTag[i]));
			char removeNumCh[32];
			sprintf(removeNumCh,"%d",circleInfos.at(i)->removeNum);
			numTbf[i]->setString(removeNumCh);
			CC_SAFE_DELETE(circleInfos.at(i));
		}

		CC_SAFE_DELETE_ARRAY(numTbf);

		showAtl[nowStep]->gotoFrameAndPlay(0,75,false);
	}
}

void PaiMing::onFrameEvent(Frame* frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    string str = evnt->getEvent();
	if(str.compare("center") == 0){
		isCanTouch = true;
	}else if(str.compare("end") == 0){
		bool isChange = true;
		//隐藏 
		showNode[nowStep]->setVisible(false);
		showSpr[nowStep]->setVisible(false);
		if(nowStep == PaiMing::one_SelfScore){
			if(isNext)
				nowStep = PaiMing::two_GlobalScore;
			else 
				isChange = false;
		}else if(nowStep == PaiMing::two_GlobalScore){
			if(isNext)
				nowStep = PaiMing::three_circleRemoveNum;
			else{
				if(nowMode == ShowMode::showMode_One)
					nowStep = PaiMing::one_SelfScore;
				else 
					isChange = false;			
			}
		}else if(nowStep == PaiMing::three_circleRemoveNum){
			if(isNext)
				isChange = false;
			else 
				nowStep = PaiMing::two_GlobalScore;
		}
		if(isChange)
			switchShow();
		else 
			isCanTouch = true;
	}
}

void PaiMing::touchEvent(Ref* ref,Widget::TouchEventType touchType)
{
	if(touchType == Widget::TouchEventType::ENDED){
		if(ref == modeOneBtn){
			if(nowMode!=ShowMode::showMode_One){
				nowMode = ShowMode::showMode_One;
				switchMode();
			}
		}else if(ref == modeTwoBtn){
			if(nowMode!=ShowMode::showMode_Two){
				nowMode = ShowMode::showMode_Two;
				switchMode();
			}
		}
	}
}

void PaiMing::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(PaiMing::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
	//操作 
	auto layerTouch = EventListenerTouchOneByOne::create();
	layerTouch->setSwallowTouches(false);
	layerTouch->onTouchBegan = [&](Touch* touch,Event* event){
		if(!isCanTouch) return false;
		auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标   
		if(!target->isVisible()) return false;
		Size s = target->getContentSize(); 
		Rect rect = Rect(0, 0, s.width, s.height);  
		downPos = target->convertToNodeSpace(touch->getLocation());  
		if (rect.containsPoint(downPos)){//判断触摸点是否在目标的范围内  
			return true;
		}else {
			return false;
		}
	};
	layerTouch->onTouchMoved = [&](Touch* touch,Event* event){
		if(!isCanTouch) return;
		auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标   
		if(!target->isVisible()) return;
		Point localPos = target->convertToNodeSpace(touch->getLocation());  
		if((localPos.x-downPos.x)>=target->getContentSize().width/3){//往右拉，前一页 
			if(nowStep != PaiMing::one_SelfScore&&!(nowStep == PaiMing::two_GlobalScore&&nowMode == ShowMode::showMode_Two)){
				isCanTouch = false;
				isNext = false;
				LOGD(TAG,"--------isNext %d",isNext);
				showAtl[nowStep]->gotoFrameAndPlay(showAtl[nowStep]->getCurrentFrame(),showAtl[nowStep]->getDuration(),false);
			}
		}else if((downPos.x-localPos.x)>=target->getContentSize().width/3){//往左拉，后一页 
			if(nowStep != PaiMing::three_circleRemoveNum){
				isCanTouch = false;
				isNext = true;
				LOGD(TAG,"--------isNext %d",isNext);
				showAtl[nowStep]->gotoFrameAndPlay(showAtl[nowStep]->getCurrentFrame(),showAtl[nowStep]->getDuration(),false);
			}
		}
	};
	layerTouch->onTouchEnded = [](Touch* touch,Event* event){
	};
	for(int i = 0;i<PaiMing::allShowQty;i++){
		if(i == 0)
			_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouch,showSpr[i]);
		else 
			_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouch->clone(),showSpr[i]);
	}
}

void PaiMing::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(PaiMing::removeSelf,this)));
	}
}

void PaiMing::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void PaiMing::onExit()
{
	Layer::onExit();
}

void PaiMing::onEnter()
{
	Layer::onEnter();
}

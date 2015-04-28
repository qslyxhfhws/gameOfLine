/**********天鸣**********/
#include "GameOver.h"
#include "MainStatus.h"
#include "../Other/ChooseMode.h"
#include "../Other/Shop.h"
#include "../Other/Achievement.h"
#include "GameLayer.h"
#define TAG "GameOver"

const int GameOver::allShowQty = 4;

GameOver* GameOver::create(GameResut* game_resut, Game* game)
{
	GameOver *pRet = new GameOver(game_resut, game);
	if (pRet && pRet->init()) 
    { 
        pRet->autorelease(); 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
    } 
	return pRet;
}

GameOver::GameOver(GameResut* game_resut, Game* game)
{
	gameResut = game_resut;
	overShowNode = nullptr;
	_game = game;
	//数据库 
	//分数 
	//demo-int sqlMode ;
	//if(gameResut.mode_type == MainStatus::GameMode::ModeOne||gameResut.mode_type == MainStatus::GameMode::ModeOneReward){
	//	sqlMode = SqlDBData::scoreGameMode::gameOne;
	//}else if(gameResut.mode_type == MainStatus::GameMode::ModeTwoBase||gameResut.mode_type == MainStatus::GameMode::ModeTwoLock){
	//	sqlMode = SqlDBData::scoreGameMode::gameTwo;
	//}
	//SqlMath::getInstance()->addScore(gameResut.score,sqlMode);
	////球球 
	//SqlMath::getInstance()->addCircleNum(gameResut.colorRemoveNum,sqlMode);
	//CC_SAFE_DELETE_ARRAY(gameResut.colorRemoveNum);//报错 

}

GameOver::~GameOver()
{
	CC_SAFE_DELETE_ARRAY(overShowNode);//报错 
}

bool GameOver::init()
{
	if(!Layer::init()){
		return false;
	}
	if (gameResut->starNum == 0)
		nowStep = 1;
	else nowStep = 0;
	winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	gameOverNode = CSLoader::createNode("bg8Scene.csb"); 
	this->addChild(gameOverNode,1); 

	achievementBtn = dynamic_cast<Button*>(gameOverNode->getChildByTag(9)->getChildByTag(3));
	achievementBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));

	shopBtn = dynamic_cast<Button*>(gameOverNode->getChildByTag(9)->getChildByTag(4)) ;
	shopBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));

	backBtn = dynamic_cast<Button*>(gameOverNode->getChildByTag(9)->getChildByTag(5)) ;
	backBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));

	shareBtn = dynamic_cast<Button*>(gameOverNode->getChildByTag(9)->getChildByTag(6)) ;
	shareBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));

	//影藏掉 
	Node* mid1Node = gameOverNode->getChildByTag(41);
	mid1Node->setVisible(false);

	int overShowTag[] = { 85, 2010, 131, 125 ,41,120 };
	overShowNode = new Node*[GameOver::allShowQty];
	for(int i = 0;i<sizeof(overShowTag)/sizeof(int);i++){
		if (i < GameOver::allShowQty){
			overShowNode[i] = gameOverNode->getChildByTag(overShowTag[i]);
			overShowNode[i]->setVisible(false);
		}
		else {
			gameOverNode->getChildByTag(overShowTag[i])->setVisible(false);
		}
	}

	switchMode();

	initTouch();

	return true;
}

void GameOver::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(GameOver::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
}

void GameOver::switchMode()
{
	for(int i = 0;i<GameOver::allShowQty;i++){
		overShowNode[i]->setVisible(false);
	}
	ContinueBtn = nullptr;

	overShowNode[nowStep]->setVisible(true);

	if (nowStep == overShowOrder::Order_Success){
		//按钮状态 
		int starTag[] = {2114,2119,2120};
		Sprite** starSpr = new Sprite*[sizeof(starTag)/sizeof(int)];
		for (int i = 0; i < sizeof(starTag) / sizeof(int); i++){
			starSpr[i] = dynamic_cast<Sprite*>(overShowNode[nowStep]->getChildByTag(starTag[i]));
			starSpr[i]->setColor(Color3B(150,150,150));
		}
		for (int i = 0; i < gameResut->starNum; i++){
			starSpr[i]->setColor(Color3B(255, 255, 255));
		}
		ContinueBtn = dynamic_cast<Button*>(overShowNode[nowStep]->getChildByTag(89));
		ContinueBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));
		if (gameResut->chengJiuId.size()>0 || gameResut->starNum == 3){//后面还有界面 
			ContinueBtn->loadTextureNormal("queding.png");
			ContinueBtn->loadTexturePressed("queding2.png");
		}
		else {
			ContinueBtn->loadTextureNormal("zaiwanyici.png");
			ContinueBtn->loadTexturePressed("zaiwanyici.png");
		}
	}
	else if (nowStep == overShowOrder::Order_Fail){
		//按钮状态 
		ContinueBtn = dynamic_cast<Button*>(overShowNode[nowStep]->getChildByTag(1893));
		ContinueBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));
		if (gameResut->chengJiuId.size()>0){//后面还有界面 
			ContinueBtn->loadTextureNormal("queding.png");
			ContinueBtn->loadTexturePressed("queding2.png");
		}
		else {
			ContinueBtn->loadTextureNormal("zaiwanyici.png");
			ContinueBtn->loadTexturePressed("zaiwanyici.png");
		}
	}
	else if (nowStep == overShowOrder::Order_chengJiu){
		//按钮状态 
		ContinueBtn = dynamic_cast<Button*>(overShowNode[nowStep]->getChildByTag(102));
		ContinueBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));
		if (gameResut->starNum == 3){//后面还有界面 
			ContinueBtn->loadTextureNormal("queding.png");
			ContinueBtn->loadTexturePressed("queding2.png");
		}else {
			ContinueBtn->loadTextureNormal("zaiwanyici.png");
			ContinueBtn->loadTexturePressed("zaiwanyici.png");
		}
	}
	else if (nowStep == overShowOrder::Order_Lock){//成就系统以后再做
		//按钮状态 
		ContinueBtn = dynamic_cast<Button*>(overShowNode[nowStep]->getChildByTag(663));
		ContinueBtn->addTouchEventListener(CC_CALLBACK_2(GameOver::touchEvent, this));
		ContinueBtn->loadTextureNormal("zaiwanyici.png");
		ContinueBtn->loadTexturePressed("zaiwanyici.png");
	}
}

void GameOver::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED){//end的时候呼叫 
		if(pSender == ContinueBtn){
			if (nowStep == overShowOrder::Order_Success){
				if (gameResut->chengJiuId.size() > 0){
					nowStep = overShowOrder::Order_chengJiu;
					switchMode();
				}
				else if (gameResut->starNum == 3){
					nowStep = overShowOrder::Order_Lock;
					switchMode();
				}
				else{
					Scene* nowScene = Director::getInstance()->getRunningScene();
					nowScene->getChildByTag(Mainstatus::TAG_GAME_ONE)->removeFromParentAndCleanup(true);
					ChooseMode* chooseMode = dynamic_cast<ChooseMode*>(nowScene->getChildByTag(Mainstatus::TAG_CHOOSEMODE));
					chooseMode->chooseGameMode(Mainstatus::TAG_GAME_ONE);

					this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf, this)));
				}
			}
			else if (nowStep == overShowOrder::Order_Fail){
				if (gameResut->chengJiuId.size() > 0){
					nowStep = overShowOrder::Order_chengJiu;
					switchMode();
				}
				else{
					Scene* nowScene = Director::getInstance()->getRunningScene();
					nowScene->getChildByTag(Mainstatus::TAG_GAME_ONE)->removeFromParentAndCleanup(true);
					ChooseMode* chooseMode = dynamic_cast<ChooseMode*>(nowScene->getChildByTag(Mainstatus::TAG_CHOOSEMODE));
					chooseMode->chooseGameMode(Mainstatus::TAG_GAME_ONE);

					this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf, this)));
				}
			}
			else if (nowStep == overShowOrder::Order_chengJiu){
				if (gameResut->starNum == 3){
					nowStep = overShowOrder::Order_Lock;
					switchMode();
				}
				else{
					Scene* nowScene = Director::getInstance()->getRunningScene();
					nowScene->getChildByTag(Mainstatus::TAG_GAME_ONE)->removeFromParentAndCleanup(true);
					ChooseMode* chooseMode = dynamic_cast<ChooseMode*>(nowScene->getChildByTag(Mainstatus::TAG_CHOOSEMODE));
					chooseMode->chooseGameMode(Mainstatus::TAG_GAME_ONE);

					this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf, this)));
				}
			}
			else if (nowStep == overShowOrder::Order_Lock){
				////移除vector
				Scene* nowScene = Director::getInstance()->getRunningScene();
				nowScene->getChildByTag(Mainstatus::TAG_GAME_ONE)->removeFromParentAndCleanup(true);
				ChooseMode* chooseMode = dynamic_cast<ChooseMode*>(nowScene->getChildByTag(Mainstatus::TAG_CHOOSEMODE));
				chooseMode->chooseGameMode(Mainstatus::TAG_GAME_ONE);

				this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf, this)));
			}
		}else if(pSender == achievementBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			Achievement* achievement = Achievement::create();
			nowScene->addChild(achievement, Mainstatus::ZORDER_ACHIEVEMENT, Mainstatus::TAG_ACHIEVEMENT);
		}else if(pSender == shopBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			Shop* shop = Shop::create();
			nowScene->addChild(shop, Mainstatus::ZORDER_SHOP, Mainstatus::TAG_SHOP);
		}else if(pSender == backBtn){
			_game->removeAllChildrenWithCleanup(true);
			_game->removeFromParentAndCleanup(true);
			this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf,this)));
		}else if(pSender == shareBtn){

		}
	}
}

void GameOver::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(GameOver::removeSelf,this)));
	}
}

void GameOver::removeSelf()
{

	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void GameOver::onExit()
{
	Layer::onExit();
}
void GameOver::onEnter()
{
	Layer::onEnter();
}

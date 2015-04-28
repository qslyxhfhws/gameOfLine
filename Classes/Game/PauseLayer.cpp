/**********天鸣**********/
#include "PauseLayer.h"
#include "GameLayer.h"
#define TAG "PauseLayer"

GamePause* GamePause::create(Game* game)
{
	GamePause* gamePause = new GamePause(game);
	if (gamePause && gamePause->init())
	{
		gamePause->autorelease();
	}
	else
	{
		delete gamePause;
		gamePause = NULL;
	}
	return gamePause;

}

GamePause::GamePause(Game* game)
{
	_game = game;
}

GamePause::~GamePause()
{

}

bool GamePause::init()
{
	if (!Layer::init()){
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode, 0);

	gamePauseNode = CSLoader::createNode("stop.csb");
	this->addChild(gamePauseNode, 1);

	//gamePauseNode->getChildByTag(58)->setOpacity(100);

	toMainBtn = dynamic_cast<Button*>(gamePauseNode->getChildByTag(59));
	toMainBtn->addTouchEventListener(CC_CALLBACK_2(GamePause::touchEvent, this));

	backBtn = dynamic_cast<Button*>(gamePauseNode->getChildByTag(60));
	backBtn->addTouchEventListener(CC_CALLBACK_2(GamePause::touchEvent, this));

	resetBtn = dynamic_cast<Button*>(gamePauseNode->getChildByTag(61));
	resetBtn->addTouchEventListener(CC_CALLBACK_2(GamePause::touchEvent, this));

	settingBtn = dynamic_cast<Button*>(gamePauseNode->getChildByTag(63));
	settingBtn->addTouchEventListener(CC_CALLBACK_2(GamePause::touchEvent, this));

	initTouch();

	return true;
}

void GamePause::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED){//end的时候呼叫 
		if (pSender == toMainBtn){
			NotificationCenter::getInstance()->postNotification("backToFirst", nullptr);
		}
		else if (pSender == backBtn){
			this->runAction(CallFunc::create(CC_CALLBACK_0(GamePause::removeSelf, this)));
			_game->resumeGame();
		}
		else if (pSender == resetBtn){
			this->runAction(CallFunc::create(CC_CALLBACK_0(GamePause::removeSelf, this)));
			_game->gameRest(Game::GameReset::Reset_All);
		}
		else if (pSender == settingBtn){
			Scene* nowScene = Director::getInstance()->getRunningScene();
			SysSetting* sysSetting = SysSetting::create();
			nowScene->addChild(sysSetting, Mainstatus::ZORDER_SETTING, Mainstatus::TAG_SETTING);
		}
	}
}

void GamePause::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyReleased = CC_CALLBACK_2(GamePause::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch, swallowNode);
}

void GamePause::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if (suoyou.at(suoyou.size() - 1) != this) return;
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(GamePause::removeSelf, this)));
	}
}

void GamePause::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
}

void GamePause::onExit()
{
	Layer::onExit();
}
void GamePause::onEnter()
{
	Layer::onEnter();
}

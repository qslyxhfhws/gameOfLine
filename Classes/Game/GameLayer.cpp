/**********天鸣**********/
#include "GameLayer.h"
#define TAG "GameLayer"

const string Game::guanQiaName[] = { "bg6_guanka1", "bg6_guanka2", "bg6_guanka3" };
const int Game::TAG_AREA = 55555;
const int Game::TAG_PAUSE = 55556;
const int Game::ZORDER_AREA = 1;
const int Game::ZORDER_PAUSE = 2;

//Scene* Game::createScene()
//{
//	auto scene = Scene::create();
//	auto game_layer = Game::create();
//	scene->addChild(game_layer, 1, Game::selfTag);
//	return scene;
//}

Game::Game()
{
	GameArea::ActionIsPause = false;
	gameUIMgr = nullptr;
	gameArea = nullptr;
	gamePause = nullptr;
}

Game::~Game()
{
	CC_SAFE_DELETE(gameUIMgr);
	//CC_SAFE_DELETE(gameArea); 因为是layer 用了autorelease；
}

bool Game::init()
{
	if (!Layer::init()){
		return false;
	}

	step = 0;
	starEndFrame[0] = 18;
	starEndFrame[1] = 33;
	starEndFrame[2] = 60;

	swallowNode = Node::create();
	swallowNode->setContentSize(Director::getInstance()->getVisibleSize());
	this->addChild(swallowNode, 0);

	gameNode = CSLoader::createNode("bg6_Secene.csb");
	this->addChild(gameNode);
	//UI 
	gameUIMgr = new GameUIMgr(gameNode->getChildByTag(354), this);

	//GAME 
	gameArea = GameArea::create(gameUIMgr, guanQiaName[step]);
	gameNode->addChild(gameArea, Game::ZORDER_AREA, Game::TAG_AREA);

	//STAR
	Node* starNode = gameNode->getChildByTag(962);
	starNode->setZOrder(2);
	starAtl = dynamic_cast<ActionTimeline*>(starNode->getActionByTag(starNode->getTag()));
	starAtl->gotoFrameAndPause(0);

	initTouch();

	return true;
}

void Game::initTouch(){
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch, swallowNode);
	//一般

}

void Game::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if (suoyou.at(suoyou.size() - 1) != this) return;
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		pauseGame();
	}
}

void Game::dealGameResult(GameResult game_result)
{
	switch (game_result)
	{
	case Game::Result_Success:
		LOGD(TAG,"------------------------------Result_Success-------------------");
		gameArea->removeFromParentAndCleanup(true);
		gameArea = nullptr;
		if (step < 2){
			if (step == 0){
				starAtl->gotoFrameAndPlay(0, starEndFrame[0],false);
			}
			else {
				starAtl->gotoFrameAndPlay(starEndFrame[step - 1] + 1, starEndFrame[step], false);
			}
			step++;
			gameUIMgr->resetAll();
			gameArea = GameArea::create(gameUIMgr, guanQiaName[step]);
			gameNode->addChild(gameArea, Game::ZORDER_AREA, Game::TAG_AREA);
		}
		else {
			//成功 
			step++;
			gameOver(true);
		}
		break;
	case Game::Result_Fail:
		LOGD(TAG, "------------------------------Result_Fail-------------------");
		gameOver(false);
		break;
	default:
		break;
	}
}

void Game::pauseGame()
{
	GameArea::ActionIsPause = true;
	gamePause = GamePause::create(this);
	gameNode->addChild(gamePause, Game::ZORDER_PAUSE, Game::TAG_PAUSE);
}

void Game::resumeGame()
{
	GameArea::ActionIsPause = false;
}

void Game::gameRest(GameReset game_reset)
{
	if (game_reset == GameReset::Reset_Current){
		gameArea->gameRest(true);
	}
	else if (game_reset == GameReset::Reset_All){
		step = 0;
		gameArea->removeFromParentAndCleanup(true);
		gameArea = nullptr;
		gameArea = GameArea::create(gameUIMgr, guanQiaName[step]);
		gameNode->addChild(gameArea, Game::ZORDER_AREA, Game::TAG_AREA);

		gameUIMgr->resetAll();
	}
}

void Game::gameOver(bool isSuccess)
{
	GameOver::GameResut* game_result = new GameOver::GameResut;
	game_result->starNum = step;
	game_result->diamondNum = 100;
	game_result->chengJiuId.push_back(0);
	GameOver* gameover = GameOver::create(game_result,this);
	Scene* nowScene = Director::getInstance()->getRunningScene();
	nowScene->addChild(gameover,Mainstatus::ZORDER_GAMEOVER,Mainstatus::TAG_GAMEOVER);
}

void Game::onEnter()
{
	Layer::onEnter();
}

void Game::onExit()
{
	Layer::onExit();
}

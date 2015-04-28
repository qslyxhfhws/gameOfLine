/**********天鸣**********/
#include "GameUIManager.h"
#include "../GameLayer.h"
#define TAG "GameUIManager"

const int GameUIMgr::pigmentQty = 6;
const float GameUIMgr::pigmentTotal = 12000.0f;

GameUIMgr::GameUIMgr(Node* game_UI, Game* game_layer)
{
	gameLayer = game_layer;
	pigment = nullptr;
	init(game_UI);
}

GameUIMgr::~GameUIMgr()
{
	CC_SAFE_DELETE_ARRAY(pigment);
}

void GameUIMgr::init(Node* game_UI)
{
	//重置按钮 
	resetBtn = dynamic_cast<Button*>(game_UI->getChildByTag(456));
	resetBtn->addTouchEventListener(CC_CALLBACK_2(GameUIMgr::touchEvent, this));

	//暂停按钮
	pauseBtn = dynamic_cast<Button*>(game_UI->getChildByTag(457));
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(GameUIMgr::touchEvent, this));


	nowPigment = pigmentTotal;
	pigment = new ActionTimeline*[GameUIMgr::pigmentQty];
	int tag[] = {420,426,432,438,444,450};
	for (int i = 0; i < GameUIMgr::pigmentQty; i++){
		Node* pigmentNode = game_UI->getChildByTag(tag[i]);
		pigment[i] = dynamic_cast<ActionTimeline*>(pigmentNode->getActionByTag(pigmentNode->getTag()));
		pigment[i]->gotoFrameAndPause(0);
	}
	atlFrameNum = 60;
}

bool GameUIMgr::reduceByDis(float dis)
{
	bool is_gameover = false;
	int before = nowPigment;
	nowPigment -= dis;

	if (nowPigment < GameUIMgr::pigmentTotal){
		int beforeFrame = (before*atlFrameNum*GameUIMgr::pigmentQty) / GameUIMgr::pigmentTotal;//剩余帧数 
		int beforePingZi = beforeFrame / atlFrameNum;
		int beforePingZiId = GameUIMgr::pigmentQty - beforePingZi - 1;

		int nowShengYuFrame = (nowPigment*atlFrameNum*GameUIMgr::pigmentQty) / GameUIMgr::pigmentTotal;//剩余帧数 
		int nowShengYuPingZi = nowShengYuFrame / atlFrameNum;
		int targetPingZiId = GameUIMgr::pigmentQty - nowShengYuPingZi - 1;
		int targetFrameNum = atlFrameNum - nowShengYuFrame % atlFrameNum;

		if (beforePingZiId != -1 && beforePingZiId != targetPingZiId)
			pigment[beforePingZiId]->gotoFrameAndPause(atlFrameNum);

		if (nowPigment >= 0)
			pigment[targetPingZiId]->gotoFrameAndPause(targetFrameNum);
		else 
			pigment[targetPingZiId]->gotoFrameAndPause(atlFrameNum);

	}

	if (nowPigment <= 0)
		is_gameover = true;
	return is_gameover;
}

void GameUIMgr::resetAll()
{
	nowPigment = pigmentTotal;
	for (int i = 0; i < GameUIMgr::pigmentQty; i++){
		pigment[i]->gotoFrameAndPause(0);
	}
	atlFrameNum = 60;

}

void GameUIMgr::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED){//end的时候呼叫 
		if (pSender == resetBtn){
			gameLayer->gameRest(Game::GameReset::Reset_Current);
		}
		else if (pSender == pauseBtn){
			gameLayer->pauseGame();
		}
	}
}

/**********天鸣**********/
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "Manager/GameUIManager.h"
#include "GameAreaLayer.h"
#include "PauseLayer.h"
#include "GameOver.h"
#include "TMUtility.h"
#include <string>
using namespace std;

class Game : public cocos2d::Layer
{
public:
	Game();
	~Game();
	//static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(Game);

	static const string guanQiaName[];
	static const int TAG_AREA;
	static const int TAG_PAUSE;
	static const int ZORDER_AREA;
	static const int ZORDER_PAUSE;

	enum GameResult{
		Result_Success,
		Result_Fail,
	};
	enum GameReset{
		Reset_All,
		Reset_Current,
	};
	void gameRest(GameReset game_reset);
	void dealGameResult(GameResult game_result);
	void pauseGame();
	void resumeGame();
private:
	GameUIMgr* gameUIMgr;
	GameArea* gameArea;
	GamePause* gamePause;

	Node* swallowNode;
	Node* gameNode;
	ActionTimeline* starAtl;

	int starEndFrame[3];

	int step;

	void initTouch();
	void gameOver(bool isSuccess);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};
#endif
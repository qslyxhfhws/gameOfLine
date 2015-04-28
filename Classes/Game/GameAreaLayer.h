#ifndef __GAMEAREA_LAYER_H__
#define __GAMEAREA_LAYER_H__

#include "Manager/ObstacleManager.h"
#include "Manager/CircleManager.h"
#include "Manager/GameUIManager.h"
#include "Manager/AreaManager.h"
#include "TMUtility.h"
#include <string>

using namespace std;

class Game;
class GameArea : public cocos2d::Layer
{
public:
	static GameArea* create(GameUIMgr* game_ui_mgr, string file_name);
	GameArea(GameUIMgr* game_ui_mgr, string file_name);
	~GameArea();

	static bool ActionIsPause;

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void gameRest(bool is_btn_rest);
private:
	CircleMgr* circleMgr;
	ObstacleMgr* obstacleMgr;
	AreaMgr* areaMgr;
	GameUIMgr* gameUIMgr;

	Node* gameNode;
	DrawNode* draw;
	Sprite* bgSpr;
	LayerColor* colorLayer;

	Size winSize;
	Point lastPos;
	long lasttime;
	string fileName;

	const static int ZORDER_DRAW;
	bool isContinue;
	bool isCanTouch;
	bool isFirstGameOver;
	int areaQty;
	enum DealPointResult
	{
		DealPointResult_break,
		DealPointResult_draw,
		DealPointResult_doNothing,
	};

	void initData();
	void initTouch();
	void dealWithDraw(Point locationInNode);//判断是否是游戏区域 
	void dealBgColor();//处理背景色 
	void dealRemove();//处理消除 
	void isDrawArea(Touch* touch, Event* event, Point &location_in_node,bool &is_draw_area);//判断是否是游戏区域 
	void gameFail(Ref* ref);
	void gameOver();
	DealPointResult dealWithPoint(Point targetPos);
};

#endif // __HELLOWORLD_SCENE_H__

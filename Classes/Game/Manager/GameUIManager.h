/**********天鸣**********/
#ifndef __GAMEUI_MGR_H__
#define __GAMEUI_MGR_H__

#include "TMUtility.h"
#include <string>
using namespace std;

class Game;
class GameUIMgr
{
public:
	GameUIMgr(Node* game_UI, Game* game_layer);
	~GameUIMgr();

	bool reduceByDis(float dis);//返回是否游戏结束 
	void resetAll();
private:
	Game* gameLayer;

	ActionTimeline** pigment;
	Button* resetBtn;
	Button* pauseBtn;

	const static int pigmentQty;
	const static float pigmentTotal;
	float nowPigment;
	int atlFrameNum;

	void init(Node* game_area);
	void touchEvent(Ref *pSender, Widget::TouchEventType type);
};
#endif
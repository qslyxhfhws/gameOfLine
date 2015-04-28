/**********天鸣**********/
#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "TMUtility.h"
#include <string>
using namespace std;

class Game;
class GameOver : public cocos2d::Layer
{
public :
	struct GameResut{
		int starNum;
		int diamondNum;
		vector<int> chengJiuId;
	};
	static GameOver* create(GameResut* game_resut,Game* game);
	GameOver(GameResut* game_resut, Game* game);
	~GameOver();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
private :
	Game* _game;
	Size winSize;

	Node* swallowNode;
	Node* gameOverNode;
	Node** overShowNode;
	//公用 
	Button* ContinueBtn;

	Button* achievementBtn;//成就灵 
	Button* shopBtn;//商店
	Button* backBtn;//
	Button* shareBtn;// 
	ActionTimeline* achievementAtl;//成就动画 
	ActionTimeline* shopAtl;//商店动画 


	enum overShowOrder{
		Order_Success,
		Order_Fail,
		Order_chengJiu,
		Order_Lock,
	};

	const static int allShowQty;
	int nowStep;
	GameResut* gameResut;

	void switchMode();
	void touchEvent(Ref *pSender, Widget::TouchEventType type);
	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
};

#endif
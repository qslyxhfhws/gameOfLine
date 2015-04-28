/**********天鸣**********/
#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

#include "TMUtility.h"
#include "MainStatus.h"
#include "Other/SysSetting.h"

class Game;
class GamePause : public cocos2d::Layer
{
public:
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
	static GamePause* create(Game* game);
	GamePause(Game* game);
	~GamePause();
private:
	Size winSize;
	Game* _game;

	Node* swallowNode;
	Node* gamePauseNode;
	Button* toMainBtn;
	Button* backBtn;
	Button* resetBtn;
	Button* settingBtn;

	void touchEvent(Ref *pSender, Widget::TouchEventType type);
	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void removeSelf();
};

#endif

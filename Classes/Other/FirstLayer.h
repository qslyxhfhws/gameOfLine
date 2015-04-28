#ifndef __FIRST_LAYER_H__
#define __FIRST_LAYER_H__

#include "TMUtility.h"

class First : public cocos2d::Layer
{
public:
	First();
	~First();
    static cocos2d::Scene* createScene();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(First);
private:
	Size winSize;

	Node* firstNode;
	Node* actNode;//动画的node
	ActionTimeline* actAtl;//动画对象
	Button* playGameBtn;
	Button* lockBtn;
	Button* highScoreBtn;
	Button* settingBtn;

	void initTouch();
	void touchEvent(Ref *pSender, Widget::TouchEventType type);
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void backToFirst(Ref* ref);
};

#endif

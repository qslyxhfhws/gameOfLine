/**********天鸣**********/
#ifndef __EXIT_LAYER_H__
#define __EXIT_LAYER_H__

#include "TMUtility.h"

class ExitLayer : public cocos2d::Layer
{
public:
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
	CREATE_FUNC(ExitLayer);
private:
	Size winSize;

	Node* swallowNode;
	Node* exitNode;

	Button* yesBtn;
	Button* noBtn;

	void touchEvent(Ref *pSender, Widget::TouchEventType type);
	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void removeSelf();
};

#endif
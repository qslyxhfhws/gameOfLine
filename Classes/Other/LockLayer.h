/**********天鸣**********/
#ifndef __LOCK_LAYER_H__
#define __LOCK_LAYER_H__

#include "TMUtility.h"

class Lock : public cocos2d::Layer
{
public :
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(Lock);
private :
	Size winSize;

	Node* swallowNode;
	Node* lockNode;

	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
};

#endif
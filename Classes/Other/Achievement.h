/**********天鸣**********/
#ifndef __ACHIEVEMENT_H__
#define __ACHIEVEMENT_H__

#include "TMUtility.h"

class Achievement : public cocos2d::Layer
{
public :
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(Achievement);
private :
	Size winSize;

	Node* swallowNode;
	Node* achievementNode;

	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
};

#endif
/**********天鸣**********/
#ifndef __SYS_SETTING_H__
#define __SYS_SETTING_H__

#include "TMUtility.h"

class SysSetting : public cocos2d::Layer
{
public :
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(SysSetting);
private :
	Size winSize;

	Node* swallowNode;
	Node* sysSettingNode;

	ActionTimeline* centerAtl;

	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
};

#endif
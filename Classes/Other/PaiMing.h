/**********天鸣**********/
#ifndef __PAIMING_H__
#define __PAIMING_H__

#include "TMUtility.h"
#include "TM_CCPlatformMacros.h"
#include <string>
using namespace std;

class PaiMing : public cocos2d::Layer
{
public :
	PaiMing();
	~PaiMing();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(PaiMing);
private :
	Size winSize;

	Node* swallowNode;
	Node* paiMingNode;
	
	Node** showNode;
	ActionTimeline** showAtl;
	Sprite** showSpr;
	TextBMFont* heightScoreTbf;
	Button* modeOneBtn;
	Button* modeTwoBtn;
	Point downPos;

	enum PaiMingOrder
	{
		one_SelfScore,
		two_GlobalScore,
		three_circleRemoveNum,
	};
	PaiMingOrder nowStep;
	const static int allShowQty;
	enum ShowMode
	{
		showMode_One,
		showMode_Two,
	};
	ShowMode nowMode;
	bool isCanTouch;
	bool isNext;

	CREATE_EVENT_FRAME();
	void switchMode();
	void switchShow();
	void onFrameEvent(Frame* frame);
	void touchEvent(Ref* ref,Widget::TouchEventType touchType);
	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
};

#endif
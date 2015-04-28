/**********天鸣**********/
#ifndef __CHOOSE_MODE_H__
#define __CHOOSE_MODE_H__

#include "TMUtility.h"
#include "TM_CCPlatformMacros.h"
#include <string>
using namespace std;

class ChooseMode : public cocos2d::Layer
{
public :
	ChooseMode();
	~ChooseMode();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(ChooseMode);
	void chooseGameMode(int targetMode);
private :
	Size winSize;

	Node* swallowNode;
	Node* chooseModeNode;

	ActionTimeline* achievementAtl;//成就动画 
	ActionTimeline* shopAtl;//商店动画 
	ActionTimeline* levelUpAtl;//升级动画 
	Sprite* achievementSpr;//成就圆圈背景精灵 
	Sprite* shopSpr;//商店圆圈背景精灵  
	Sprite* levelUpSpr;//升级圆圈背景精灵 
	PageView* centerView;//中间page
	ActionTimeline* centerAtl;//中间动画 
	Sprite* modeOneSpr;//模式一圆圈背景精灵  
	Sprite* modeTwoSpr;//模式二圆圈背景精灵 
	TextBMFont* diamondNumTbf;//钻石 
	ActionTimeline* chooseModeDingAtl;//是bg3ding的actiontimeline
	ActionTimeline* brushLingWaiAtl;//多余五只时的画笔
	Sprite** brushSpr;//画笔数组 
	TextBMFont* brushNumTbf;//画笔数量 

	int nowModeTag;//现在要开始的mode，参照mainstatus的tag设置 
	bool isCanTouch;
	int brushNum;
	long long brushShiJCha;//记录的时间与现在的时间差
	const static string ActEndStr[];
	enum AtlList
	{
		ATL_ACHIEVEMENT,
		ATL_SHOP,
		ATL_LEVELUP,
		ATL_CHOOSEMODE,
	};

	void initTouch();
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();
	void pageViewEvent(Ref *pSender, PageView::EventType type);
	CREATE_EVENT_FRAME();
	void onFrameEvent(Frame* frame);
	void scheduleBrush(float dt);
	void convertJiLuToCD();
	void saveTimeNow(bool isFirst);
};

#endif
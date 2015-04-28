/**********天鸣**********/
#ifndef __CIRCLE_NODE_H__
#define __CIRCLE_NODE_H__

#include "TM_CCPlatformMacros.h"
#include "TMUtility.h"
#include <string>
using namespace std;

class Circle : cocos2d::Node
{
public:
	struct ArroundObj{
		Node* kuangkuangNode;
		Sprite* qipaoSpr;
	};

	Circle(Node* circle_node, ArroundObj* arround_obj);
	~Circle();

	Sprite* circleSpr;
	Node* circleNode;
	ActionTimeline* circleAtl;
	ArroundObj* arroundObj;

	int circleType;
	int circleId;
	int areaId;
	bool isRemoved;
	bool isHiddenArea;
	const static float bombTime;
	enum ActionType
	{
		Action_Touch,
		Action_Show,
		Action_Hidden,
		Action_Zhuan,
	};
	void doAction(ActionType action_type);
	void initData();
	bool getIsCanTouch();
private:
	Size winSize;
	string actFrameName;

	enum ColorType
	{
		base_251_100_107,//红 
		base_255_231_27,//黄 
		base_118_236_115,//绿 
		base_140_213_255,//蓝
		base_141_82_251,//紫
		otherColor,
		bomb,
	};
	static const string bombDjsTime;
	static const int ColorQty;
	static map<string, int> ColorTypeMap;

	CREATE_EVENT_FRAME();
	void initCircle(Node* circle_node);
	void initBomb(Node* circle_node);
	void onFrameEvent(Frame* frame);
	void bombDJS(TextBMFont* djsTbf);

};

#endif

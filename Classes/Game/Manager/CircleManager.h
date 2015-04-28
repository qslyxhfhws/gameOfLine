/**********天鸣**********/
#ifndef __CIRCLE_MGR_H__
#define __CIRCLE_MGR_H__

#include "../Objects/CircleNode.h"
#include "CircleArroundManager.h"
#include "TMUtility.h"
#include <string>
using namespace std;

class CircleMgr : Ref
{
public:
	CircleMgr(Node* game_area,string file_name);
	~CircleMgr();

	CircleArrMgr* circleArrMgr;

	int lastTagNum;//记录之前一次touch的tag
	enum AddOrCancel
	{
		AddOrCancel_Add,
		AddOrCancel_Cancel,
		AddOrCancel_Default,
	};

	int getTouchCirclesSize();
	AddOrCancel searchIsCircle(Point targetPos);//判断是否是球球并返回结果  
	bool removeTouchListAndIsGameOver();
	void changeLineColor(float& color_r, float& color_g, float& color_b);
	void changeCircleVisible(vector<int>* hidden_area_id);
	void getAreaQty(int& area_qty);
	bool isJieSuo(Color3B color,int num);
	void resetAll();
	void changeAreaId(int& nowAreaId);
	Color3B getTouchColor();
	void dealArround(int arround_id);
	bool isAllRemove();
private:
	vector<Circle*>* circles;
	vector<Circle*>* touchCircles;

	void circletouchFinish(Ref* pSender);
	bool isAddInTouch(int nowCircleId);//是否已经画进去了 
	bool isdoContinue(int circle_id);
};

#endif

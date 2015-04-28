/**********天鸣**********/
#ifndef __AREA_MGR_H__
#define __AREA_MGR_H__

#include <string>
#include "TM_CCPlatformMacros.h"
#include "CircleManager.h"
#include "ObstacleManager.h"
#include "TMUtility.h"
using namespace std;

class AreaMgr
{
public:
	AreaMgr(Node* game_area, int area_qty, CircleMgr* circle_mgr, ObstacleMgr* obstacle_mgr);
	~AreaMgr();
	enum AreaResult
	{
		AreaResult_Nothing,
		AreaResult_DealRemove,
		AreaResult_jieSuo,
	};
	int nowAreaId;
	AreaResult JudgeArea(Point targetPos);
	void resetPanDing();//画完就重置判定区 
	void resetAll();
private:
	CREATE_EVENT_FRAME();
	struct AreaJudge{
		/*判定区的作用是确定当前区域是否在一次绘画中被点亮，默认为false，除了第一区一直为true
		判定区不属于区域本身
		但是bool型的值属于区域本身 */
		Sprite* JudgeSpr;//灯泡亮起的精灵 
		bool AreaStatus_PanDing;
		/*锁的作用是锁住当前区域，所以只有在当前区域解锁，不存在外部解锁
		有锁的地方和最后一个为true，解锁后false 
		锁属于区域本身 */
		bool AreaStatus_Lock;
		int lockNum;
		Color3B lockColor;
		Node* lockNode;
		ActionTimeline* lockAtl;
		/*灯泡作用是是否显示球，默认为false，点亮后变为true
		灯泡属于区域本身*/
		bool AreaStatus_Bulb;
		Sprite* ligthOnSpr;
	};
	AreaJudge* areaJudge;

	Node* gameNode;
	CircleMgr* circleMgr;
	ObstacleMgr* obstacleMgr;

	int areaQty;
	vector<int>* hiddenAreaId;
	Point lastPoint;

	void cloneAreaJudge();
	void changeHiddenAreaIdAndCircleObstacle();//整理出来隐藏的区域
	void onFrameEvent(Frame* frame);
	void initNode(Node* game_node);//对接小组件 
	void initStatus();//初始化状态 
};

#endif

/**********天鸣**********/
#ifndef __OBSTACLE_NODE_H__
#define __OBSTACLE_NODE_H__

#include "TMUtility.h"
#include <string>
#include <iterator>
using namespace std;

class Obstacle : cocos2d::Node
{
public:
	Obstacle(Node* obstacleNode);
	~Obstacle();

	enum ObstacleType
	{
		Stationary,
		wuyun,//移动的乌云 
		shandian,//闪电 
		actionCCS,//ccs方式读入 
	};

	static const int ObstacleQty;

	int itemType;
	int areaId;//默认为-1，

	Sprite* obstacleSpr;
	ActionTimeline* obstacleAtl;

	void doAction();
	void stopAction();
	void initData();
private:
	static map<string, int> ObstacleTypeMap;
	Size winSize;
	Point firstPos;

	void init(Node* obstacleNode);
};

#endif

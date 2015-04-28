/**********天鸣**********/
#ifndef __OBSTACLE_MGR_H__
#define __OBSTACLE_MGR_H__

#include "../Objects/ObstacleNode.h"
#include "CircleManager.h"
#include "TMUtility.h"
#include <string>
using namespace std;

class ObstacleMgr
{
public:
	ObstacleMgr(Node* game_area);
	~ObstacleMgr();

	bool isImpact(Point targetPos);
	void changeObstacleVisible(vector<int>* hidden_area_id);
	void stopallAct();
	void resetAll();
protected:
	vector<Obstacle*>* obstacles;

	void initObstacle(Node* game_area);

	bool isObstacleAlpah255(Sprite* targetSpr, Point locationInNode);
};

#endif

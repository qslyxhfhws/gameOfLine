/**********天鸣**********/
#ifndef __CIRCLEARR_MGR_H__
#define __CIRCLEARR_MGR_H__

#include "../Objects/SpecialNode.h"
#include "../Objects/CircleNode.h"
#include "TMUtility.h"
#include <string>
using namespace std;

class CircleArrMgr
{
public:
	struct CircleArround{
		int CircleTag;
		int kuangkuangTag;
		int qipaoTag;
	};
	CircleArrMgr(Node* game_area,string file_name);
	~CircleArrMgr();

	int isImpact(Point targetPos);
	void kuangkuangIsHidden(int id, int touch_size, Color3B color3b);
	Circle::ArroundObj* getArroundObj( int circle_tag);
	void resetAll();
private:
	vector<Special*>* specials;
	vector<CircleArrMgr::CircleArround*>* circleArrounds;

	void getCircleInfo(string file_name);
	void init(Node* game_area);

	bool isObstacleAlpah255(Sprite* targetSpr, Point locationInNode);
};

#endif

/**********天鸣**********/
#ifndef __SPECIAL_NODE_H__
#define __SPECIAL_NODE_H__

#include "TMUtility.h"
#include <string>
#include <iterator>
using namespace std;

class Special : cocos2d::Node
{
public:
	Special(Node* specialNode);
	~Special();

	enum SpecialType
	{
		kuangKuang,//框框 
		qipao,//气泡
	};

	static const int specialQty;

	int itemType;
	int areaId;//默认为-1，

	Node* specialNode;

	vector<Sprite*>* specialSprs;
	ActionTimeline* specialAtl;
	void reset();
private:
	static map<string, int> SpecialTypeMap;
	Size winSize;
	Point firstPos;

	void specialinit();
	void dealWithAction();
};

#endif

/**********天鸣**********/
#ifndef __MAIN_STATUS_H__
#define __MAIN_STATUS_H__

#include <string>
#include <vector>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class Mainstatus
{
public:
	const static float maxOpacity;
	const static float onceOpacity;
	static void strSplit(string targetStr, string sign, vector<char*>& result);
	static Rect getRectInTarget(Sprite* targetSpr,Node* targetNode);
	static bool isPengZhuang(Sprite* targetSpr,Point targetPos);
	const static int brushMax;
	const static int brushNeedTime;
	const static int Price_Item[];
	///////////////////////////Layer tag zorder///////////////////////////
	//过场动画 
	const static int ZORDER_TRANSITION;
	const static int TAG_TRANSITION;
	//首页 
	const static int ZORDER_FIRST;
	const static int TAG_FIRST;
	//选择模式
	const static int ZORDER_CHOOSEMODE;
	const static int TAG_CHOOSEMODE;
	//设置
	const static int ZORDER_SETTING;
	const static int TAG_SETTING;
	//解锁图案
	const static int ZORDER_LOCK;
	const static int TAG_LOCK;
	//排名
	const static int ZORDER_PAIMING;
	const static int TAG_PAIMING;
	//游戏界面
	const static int ZORDER_GAME;
	const static int TAG_GAME_ONE;
	const static int TAG_GAME_ONE_REWARD;
	const static int TAG_GAME_TWO;
	//游戏结束 
	const static int ZORDER_GAMEOVER;
	const static int TAG_GAMEOVER;
	//模式二地图 
	const static int ZORDER_MAP;
	const static int TAG_MAP;
	//成就
	const static int ZORDER_ACHIEVEMENT;
	const static int TAG_ACHIEVEMENT;
	//商店
	const static int ZORDER_SHOP;
	const static int TAG_SHOP;
	//退出 
	const static int ZORDER_EXIT;
	const static int TAG_EXIT;
private:
};
#endif

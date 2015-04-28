/**********天鸣**********/
#include "Mainstatus.h"

const float Mainstatus::maxOpacity = 0.4;
const float Mainstatus::onceOpacity = 0.04;
const int Mainstatus::brushMax = 5;
const int Mainstatus::brushNeedTime = 3600;
const int Mainstatus::Price_Item[] = { 80, 100, 120, 140 };
///////////////////////////Layer tag zorder///////////////////////////
//-------zorder-------//
const int Mainstatus::ZORDER_TRANSITION = 10000;
const int Mainstatus::ZORDER_FIRST = 0;

const int Mainstatus::ZORDER_CHOOSEMODE = 1;
const int Mainstatus::ZORDER_SETTING = 6;
const int Mainstatus::ZORDER_LOCK = 1;
const int Mainstatus::ZORDER_PAIMING = 1;

const int Mainstatus::ZORDER_MAP = 2;

const int Mainstatus::ZORDER_GAME = 3;

const int Mainstatus::ZORDER_GAMEOVER = 4;

const int Mainstatus::ZORDER_ACHIEVEMENT = 5;
const int Mainstatus::ZORDER_SHOP = 5;
const int Mainstatus::ZORDER_EXIT = 7;
//-------tag-------//getchildren 的 vector 按照tag排序 
const int Mainstatus::TAG_TRANSITION = 10000;
const int Mainstatus::TAG_FIRST = 1001;

const int Mainstatus::TAG_CHOOSEMODE = 1101;
const int Mainstatus::TAG_LOCK = 1103;
const int Mainstatus::TAG_PAIMING = 1104;

const int Mainstatus::TAG_MAP = 1201;

const int Mainstatus::TAG_GAME_ONE = 1301;
const int Mainstatus::TAG_GAME_ONE_REWARD = 1303;
const int Mainstatus::TAG_GAME_TWO = 1302;

const int Mainstatus::TAG_GAMEOVER = 1401;

const int Mainstatus::TAG_ACHIEVEMENT = 1502;
const int Mainstatus::TAG_SHOP = 1503;

const int Mainstatus::TAG_SETTING = 1602;
const int Mainstatus::TAG_EXIT = 1702;

void Mainstatus::strSplit(string targetStr, string sign, vector<char*>& result)
{
	char* target = new char[strlen(targetStr.c_str()) + 2];
	target[strlen(targetStr.c_str()) + 1] = 0;
	memcpy(target, targetStr.c_str(), strlen(targetStr.c_str()) + 1);

	result.clear();
	char* a = strtok(target, sign.c_str());
	while (a){
		result.push_back(a);
		a = strtok(NULL, sign.c_str());
	}
}

Rect Mainstatus::getRectInTarget(Sprite* targetSpr, Node* targetNode)
{
	Size s = Size(targetSpr->getContentSize().width*targetSpr->getScaleX(), targetSpr->getContentSize().height*targetSpr->getScaleY());
	Point pos;
	if (targetNode == nullptr)
		pos = targetSpr->convertToWorldSpace(Vec2(0, 0));
	Rect rect = Rect(pos.x, pos.y, s.width, s.height);
	return rect;
}

bool Mainstatus::isPengZhuang(Sprite* targetSpr, Point targetPos)
{
	Point locationInNode = targetSpr->convertToNodeSpace(targetPos);
	Size s = Size(targetSpr->getContentSize().width, targetSpr->getContentSize().height);
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
		return true;
	else
		return false;
}
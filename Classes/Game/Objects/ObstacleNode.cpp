/**********天鸣**********/
#include "ObstacleNode.h"
#include "MainStatus.h"
#define TAG "Obstacle"

const int Obstacle::ObstacleQty = 4;
map<string, int> Obstacle::ObstacleTypeMap;

Obstacle::Obstacle(Node* obstacleNode)
{
	winSize = Director::getInstance()->getVisibleSize();
	string obstacle_name[] = { "wuyun", "shandian" };
	if (ObstacleTypeMap.size() <= 0){
		for (int i = 1; i<Obstacle::ObstacleQty-1; i++){//-1为了不包括ccs
			Obstacle::ObstacleTypeMap.insert(pair<string, int>(obstacle_name[i-1], i));
		}
	}
	init(obstacleNode);
}

Obstacle::~Obstacle()
{

}

void Obstacle::init(Node* obstacleNode)
{
	obstacleSpr = nullptr;
	obstacleAtl = nullptr;
	itemType = 0;
	areaId = -1;
	firstPos = Vec2(0,0);
	vector<char*> resultStr;
	string name;
	Mainstatus::strSplit(obstacleNode->getName(), "_", resultStr);
	if (resultStr.size()>1){
		name = resultStr.at(0);
		areaId = atoi(resultStr.at(1));
	}
	else {
		name = resultStr.at(0);
	}
	auto result = Obstacle::ObstacleTypeMap.find(name);
	if (result != Obstacle::ObstacleTypeMap.end()){//已有移动方式不包括ccs档
		itemType = result->second;
	}
	if (obstacleNode->getTag() >= 20000) {
		obstacleAtl = dynamic_cast<ActionTimeline*>(obstacleNode->getActionByTag(obstacleNode->getTag()));
		//这边同样要给出obstacleSpr；
		itemType = actionCCS;
	}else {//默认为零，不用更改type
		obstacleSpr = dynamic_cast<Sprite*>(obstacleNode);
		firstPos = obstacleSpr->getPosition();
	}
}

void Obstacle::doAction()
{
	if (itemType == ObstacleType::Stationary){

	}
	else if (itemType == ObstacleType::wuyun){
		float speed = 2;
		Point targetPos;
		float min = obstacleSpr->getParent()->convertToNodeSpace(Vec2(0, 0)).x;
		float max = obstacleSpr->getParent()->convertToNodeSpace(Vec2(winSize.width, 0)).x;
		Point minPos = Vec2(min + obstacleSpr->getAnchorPoint().x*obstacleSpr->getContentSize().width, obstacleSpr->getPosition().y);
		Point maxPos = Vec2(max - (1 - obstacleSpr->getAnchorPoint().x)*obstacleSpr->getContentSize().width, obstacleSpr->getPosition().y);
		if (min == obstacleSpr->getPosition().x || max == obstacleSpr->getPosition().x){
			min == obstacleSpr->getPosition().x ? targetPos = maxPos : minPos;
		}else{
			int randomFX = rand() % 2;
			randomFX == 0 ? targetPos = maxPos : targetPos = minPos;
		}
		float dura = (abs(targetPos.x - obstacleSpr->getPosition().x)*speed) / winSize.width;

		obstacleSpr->runAction(Sequence::create(MoveTo::create(dura, targetPos), CallFunc::create(CC_CALLBACK_0(Obstacle::doAction, this)),nullptr));
	}
	else if (itemType == ObstacleType::shandian){
		obstacleSpr->setVisible(true);
		float speed = 1;
		float delayTime = 1.0;
		Sprite* bgSpr = dynamic_cast<Sprite*>(obstacleSpr->getParent()->getParent()->getChildByName("Sprite_baidi"));
		//游戏界面背景精灵在整个layer上y轴的位置
		float targetY = bgSpr->getPosition().y - bgSpr->convertToNodeSpace(Vec2(0, 0)).y - bgSpr->getAnchorPoint().y*bgSpr->getContentSize().height;
		//再把targetY转到目标精灵父类的坐标系
		Point targetPos = Vec2(obstacleSpr->getPosition().x, obstacleSpr->getParent()->convertToNodeSpace(Vec2(0, targetY)).y + obstacleSpr->getAnchorPoint().y*obstacleSpr->getContentSize().height);
		obstacleSpr->runAction(Sequence::create(MoveTo::create(speed, targetPos), FadeOut::create(0), MoveTo::create(0, firstPos), DelayTime::create(delayTime), FadeIn::create(0), CallFunc::create(CC_CALLBACK_0(Obstacle::doAction, this)), nullptr));
	}
	else if (itemType == ObstacleType::actionCCS){
	}

}

void Obstacle::stopAction()
{
	obstacleSpr->stopAllActions();
	if (itemType == ObstacleType::Stationary){

	}
	else if (itemType == ObstacleType::wuyun){
	}
	else if (itemType == ObstacleType::shandian){
		obstacleSpr->setVisible(false);
		obstacleSpr->setPosition(firstPos);
	}
	else if (itemType == ObstacleType::actionCCS){
	}
}

void Obstacle::initData()
{

}
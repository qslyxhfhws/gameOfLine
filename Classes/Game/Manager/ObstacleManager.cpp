/**********天鸣**********/
#include "ObstacleManager.h"
#define TAG "ObstacleManager"

#include "MainStatus.h"

ObstacleMgr::ObstacleMgr(Node* game_area)
{
	initObstacle(game_area);
}

ObstacleMgr::~ObstacleMgr()
{
	for (int i = 0; i < obstacles->size(); i++){
		CC_SAFE_DELETE(obstacles->at(i));
	}
	CC_SAFE_DELETE(obstacles);

}

void ObstacleMgr::initObstacle(Node* game_area){
	if (game_area->getChildByName("Node_zhangaiwu") == nullptr)
		return;
	else if ((game_area->getChildByName("Node_zhangaiwu")->getChildren()).size() == 0)
		return;

	obstacles = nullptr;
	Vector<Node*> tempObstacles = game_area->getChildByName("Node_zhangaiwu")->getChildren();
	obstacles = new vector < Obstacle* >;
	obstacles->clear();
	for (int i = 0; i < tempObstacles.size(); i++){
		Obstacle* obstacle = new Obstacle(tempObstacles.at(i));
		obstacles->push_back(obstacle);
	}
}

bool ObstacleMgr::isImpact(Point targetPos){
	bool isImpact = false;
	for (int i = 0; i < obstacles->size(); i++){
		Point locationInNode = obstacles->at(i)->obstacleSpr->convertToNodeSpace(targetPos);
		Size s = Size(obstacles->at(i)->obstacleSpr->getContentSize().width, obstacles->at(i)->obstacleSpr->getContentSize().height);
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)){
			if (isObstacleAlpah255(obstacles->at(i)->obstacleSpr, locationInNode)){
				isImpact = true;
				break;
			}
		}
	}
	return isImpact;
}

bool ObstacleMgr::isObstacleAlpah255(Sprite* targetSpr, Point locationInNode)
{
	Point pivot_pos = targetSpr->getAnchorPointInPoints();
	float angle_radians = -1 * targetSpr->getRotationX()*M_PI / 180.f;
	locationInNode = locationInNode.rotateByAngle(pivot_pos, angle_radians);

	Point yuanshiPos = targetSpr->getPosition();// Vec2(targetSpr->getPosition().x - targetSpr->getAnchorPoint().x*targetSpr->getContentSize().width, targetSpr->getPosition().y - targetSpr->getAnchorPoint().y*targetSpr->getContentSize().height);
	locationInNode = Vec2(locationInNode.x - targetSpr->getAnchorPoint().x*targetSpr->getContentSize().width, locationInNode.y - targetSpr->getAnchorPoint().y*targetSpr->getContentSize().height);
	targetSpr->setPosition(-locationInNode.x, -locationInNode.y);
	RenderTexture *render = RenderTexture::create(1, 1, Texture2D::PixelFormat::RGBA8888);
	render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	targetSpr->visit();
	render->end();
	Director::getInstance()->getRenderer()->render();
	render->retain();
	Image *finalImage = render->newImage();
	unsigned char *_data = finalImage->getData();

	ccColor4B c = { 0, 0, 0, 0 };
	unsigned int x = locationInNode.x, y = targetSpr->getContentSize().height - locationInNode.y;
	_data += 3;// (int)image->getRenderFormat();
	c.a = *(_data);
	targetSpr->setPosition(yuanshiPos);
	delete finalImage;
	LOGD(TAG, "----------------------TOUMDU %d", c.a);
	if (c.a >= 200)
		return true;
	else
		return false;
	//return false;
}

void ObstacleMgr::changeObstacleVisible(vector<int>* hidden_area_id)
{
	stopallAct();
	for (int i = 0; i < obstacles->size(); i++){
		bool isMove = true;
		for (int j = 0; j < hidden_area_id->size(); j++){
			if (obstacles->at(i)->areaId == hidden_area_id->at(j)){
				isMove = false;
			}
		}
		if (isMove){
			obstacles->at(i)->doAction();
		}
	}
}

void ObstacleMgr::stopallAct()
{
	for (int i = 0; i < obstacles->size(); i++){
		obstacles->at(i)->stopAction();
	}
}
void ObstacleMgr::resetAll()
{
	stopallAct();
	for (int i = 0; i < obstacles->size(); i++){
		obstacles->at(i)->initData();
	}
}
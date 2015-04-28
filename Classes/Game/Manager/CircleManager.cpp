/**********天鸣**********/
#include "CircleManager.h"
#define TAG "CircleManager"

CircleMgr::CircleMgr(Node* game_area, string file_name)
{
	circles = new vector < Circle* >;
	touchCircles = new vector < Circle* >;
	circleArrMgr = new CircleArrMgr(game_area, file_name);

	circles->clear();
	Vector<Node*> temp_circles = game_area->getChildByName("Node_ball")->getChildren();
	for (int i = 0; i < temp_circles.size(); i++){
		int tag = temp_circles.at(i)->getTag();
		Circle::ArroundObj* arround_obj = circleArrMgr->getArroundObj( tag);
		Circle* circle = new Circle(temp_circles.at(i), arround_obj);
		circle->circleId = i;
		circles->push_back(circle);
	}
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CircleMgr::circletouchFinish), "circletouchFinish", NULL);
}

CircleMgr::~CircleMgr()
{
	for (int i = 0; i < circles->size(); i++){
		CC_SAFE_DELETE(circles->at(i));
	}
	CC_SAFE_DELETE(circles);

	for (int i = 0; i < touchCircles->size(); i++){
		CC_SAFE_DELETE(touchCircles->at(i));
	}
	CC_SAFE_DELETE(touchCircles);

	CC_SAFE_DELETE(circleArrMgr);
	NotificationCenter::getInstance()->removeAllObservers(this);
}

int CircleMgr::getTouchCirclesSize()
{
	return touchCircles->size();
}

//判断是否是球球
CircleMgr::AddOrCancel CircleMgr::searchIsCircle(Point targetPos)
{
	AddOrCancel add_or_cancel = AddOrCancel::AddOrCancel_Default;
	Color3B colorValue = Color3B(0,0,0);
	//string colorStr;//颜色值
	if (touchCircles->size() != 0){
		colorValue = touchCircles->at(0)->circleNode->getColor();
		//colorStr = touchCircles->at(0)->circleSpr->getName();
	}
	for (int i = 0; i<circles->size(); i++){//循环判断
		if (isdoContinue(i))
			continue;
		Point locationInNode = circles->at(i)->circleSpr->convertToNodeSpace(targetPos);
		Size s = Size(circles->at(i)->circleSpr->getContentSize().width, circles->at(i)->circleSpr->getContentSize().height);
		Rect rect = Rect(0, 0, s.width, s.height);
		//LOGD(TAG, "------------===========s.width %f  s.height %f  locationInNode.x %f  locationInNode.y %f", s.width, s.height, locationInNode.x, locationInNode.y);
		if (rect.containsPoint(locationInNode)){//画中球球 
			lastTagNum = circles->at(i)->circleId;
			if (touchCircles->size() != 0){//如果之前画到过球球 ，比对颜色是否相同 
				if (circles->at(i)->circleNode->getColor() == colorValue){//如果颜色相同
				//if (circles->at(i)->circleSpr->getName().compare(colorStr) == 0){//如果颜色相同
					bool is_add = isAddInTouch(circles->at(i)->circleId);
					if (!is_add){
						circles->at(i)->doAction(Circle::ActionType::Action_Touch);
						touchCircles->push_back(circles->at(i));
						//pushTouchList(circles->at(i));
						add_or_cancel = AddOrCancel::AddOrCancel_Add;
					}
				}
				else{//颜色不同
					add_or_cancel = AddOrCancel::AddOrCancel_Cancel;
				}
			}
			else{//之前未画到球球，加入list 
				circles->at(i)->doAction(Circle::ActionType::Action_Touch);
				touchCircles->push_back(circles->at(i));
				//pushTouchList(circles->at(i));
				add_or_cancel = AddOrCancel::AddOrCancel_Add;
			}
			break;
		}
	}
	return add_or_cancel;
}

bool CircleMgr::isdoContinue(int circle_id)
{
	if (circles->at(circle_id)->isRemoved || circles->at(circle_id)->isHiddenArea || !circles->at(circle_id)->getIsCanTouch())
		return true;
	if (circles->at(circle_id)->arroundObj != nullptr){
		if (circles->at(circle_id)->arroundObj->qipaoSpr != nullptr){
			if (circles->at(circle_id)->arroundObj->qipaoSpr->isVisible()&&touchCircles->size()<=0){
				return  true;
			}
		}
	}
	return false;
}

void CircleMgr::circletouchFinish(Ref* pSender)
{
	if (touchCircles->size() == 2){
		touchCircles->at(0)->doAction(Circle::ActionType::Action_Zhuan);
		touchCircles->at(1)->doAction(Circle::ActionType::Action_Zhuan);
	}
	else if (touchCircles->size()>2){
		touchCircles->at(touchCircles->size()-1)->doAction(Circle::ActionType::Action_Zhuan);
	}

}

//是否已经画进去了 
bool CircleMgr::isAddInTouch(int nowCircleId)
{
	bool isChong = false;
	for (int i = 0; i<touchCircles->size(); i++){
		if (touchCircles->at(i)->circleId == nowCircleId){
			isChong = true;
			break;
		}
	}
	return isChong;
}

bool CircleMgr::removeTouchListAndIsGameOver()
{
	bool is_gameover = false;
	if (touchCircles->size() >= 2){
		for (int i = 0; i < touchCircles->size(); i++){
			touchCircles->at(i)->doAction(Circle::ActionType::Action_Hidden);
		}
		//判断场上这个颜色的有几个 
		//Color3B color3b = touchCircles->at(0)->circleNode->getColor();
		//auto result = count_if(circles->cbegin(), circles->cend(), [color3b](Circle* circle){
		//	return !circle->isRemoved&&circle->circleNode->getColor() == color3b;
		//});
		//if (result == 1){
		//	is_gameover = true;
		//}
	}
	else if (touchCircles->size() == 1){
		touchCircles->at(0)->circleAtl->gotoFrameAndPause(0);
	}
	touchCircles->clear();
	return is_gameover;
}

void CircleMgr::changeLineColor(float& color_r, float& color_g, float& color_b)
{
	color_r = touchCircles->at(0)->circleNode->getColor().r / 255.0;
	color_g = touchCircles->at(0)->circleNode->getColor().g / 255.0;
	color_b = touchCircles->at(0)->circleNode->getColor().b / 255.0;
}

void CircleMgr::getAreaQty(int& area_qty)
{
	sort(circles->begin(), circles->end(), [](Circle* circle1, Circle* circle2){
		return circle1->areaId < circle2->areaId;
	});
	area_qty = circles->at(circles->size() - 1)->areaId+1;
}

void CircleMgr::changeCircleVisible(vector<int>* hidden_area_id)
{
	for (int i = 0; i < circles->size(); i++){
		bool is_hidden = false;
		for (int j = 0; j < hidden_area_id->size(); j++){
			if (circles->at(i)->areaId == hidden_area_id->at(j)){
				is_hidden = true;
				break;
			}
		}
		if (is_hidden){
			circles->at(i)->isHiddenArea = true;
			circles->at(i)->circleNode->setVisible(false);
			//circles->at(i)->circleSpr->setVisible(false);
		}
		else {
			if (!circles->at(i)->isRemoved){
				if (circles->at(i)->isHiddenArea){
					circles->at(i)->doAction(Circle::ActionType::Action_Show);
				}
				circles->at(i)->circleNode->setVisible(true);
				//circles->at(i)->circleSpr->setVisible(true);
			}
			circles->at(i)->isHiddenArea = false;
		}
	}
}

bool CircleMgr::isJieSuo(Color3B color, int num)
{
	bool is_jie_suo = false;
	if (touchCircles->size()>0)
	{
		if (color == touchCircles->at(0)->circleNode->getColor() && num == touchCircles->size())
			is_jie_suo = true;
	}
	return is_jie_suo;
}

void CircleMgr::changeAreaId(int& nowAreaId)
{
	nowAreaId = touchCircles->at(0)->areaId;
}

void CircleMgr::resetAll()
{
	lastTagNum = -1;
	for (int i = 0; i < circles->size(); i++){
		circles->at(i)->initData();
	}
	circleArrMgr->resetAll();
}

Color3B CircleMgr::getTouchColor()
{
	if (touchCircles->size()>0)
		return touchCircles->at(0)->circleNode->getColor();
	else
		return Color3B(255, 255, 255);
}

void CircleMgr::dealArround(int arround_id)
{
	circleArrMgr->kuangkuangIsHidden(arround_id, touchCircles->size(), getTouchColor());
}

bool CircleMgr::isAllRemove()
{
	for (int i = 0; i < circles->size(); i++){
		if (circles->at(i)->isRemoved == false){
			LOGD(TAG,"---------------NOTREMOVEID %d",i);
			return false;
		}
	}
	return true;
}

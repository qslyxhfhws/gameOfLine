/**********天鸣**********/
#include "AreaManager.h"
#include "MainStatus.h"

#define TAG "AreaMath"

AreaMgr::AreaMgr(Node* game_area, int area_qty, CircleMgr* circle_mgr, ObstacleMgr* obstacle_mgr)
{
	areaJudge = nullptr;
	hiddenAreaId = nullptr;
	hiddenAreaId = new vector < int >;

	gameNode = game_area->getParent();
	circleMgr = circle_mgr;
	obstacleMgr = obstacle_mgr;
	areaQty = area_qty;

	initNode(game_area);
}

AreaMgr::~AreaMgr()
{
	CC_SAFE_DELETE(areaJudge);
	CC_SAFE_DELETE(hiddenAreaId);
}

void AreaMgr::initNode(Node* game_area)
{
	areaJudge = new AreaJudge[areaQty];
	for (int i = 0; i < areaQty; i++){
		areaJudge[i].JudgeSpr = nullptr;
		areaJudge[i].ligthOnSpr = nullptr;
		areaJudge[i].lockAtl = nullptr;
		areaJudge[i].lockNode = nullptr;
		areaJudge[i].lockNum = -1;
		areaJudge[i].lockColor = Color3B(0,0,0);
		areaJudge[i].AreaStatus_Lock = false;
		areaJudge[i].AreaStatus_Bulb = false;
		areaJudge[i].AreaStatus_PanDing = false;
	}
	//灯 
	if (game_area->getChildByName("Node_light") != nullptr){
		Vector<Node*> lights = game_area->getChildByName("Node_light")->getChildren();
		for (int i = 0; i < lights.size(); i++){
			vector<char*> areaIds;
			Mainstatus::strSplit(lights.at(i)->getName(), "_", areaIds);
			if (((string)areaIds.at(2)).compare("on") == 0){
				areaJudge[atoi(areaIds.at(0))].ligthOnSpr = dynamic_cast<Sprite*>(lights.at(i));
			}
		}
	}
	//判定区  
	if (game_area->getChildByName("Node_panding") != nullptr){
		Vector<Node*> panDings = game_area->getChildByName("Node_panding")->getChildren();
		for (int i = 0; i < panDings.size(); i++){
			vector<char*> areaIds;
			Mainstatus::strSplit(panDings.at(i)->getName(), "_", areaIds);
			areaJudge[atoi(areaIds.at(0))].JudgeSpr = dynamic_cast<Sprite*>(panDings.at(i));
		}
	}
	//锁  
	if (game_area->getChildByName("Node_suo") != nullptr){
		Vector<Node*> locks = game_area->getChildByName("Node_suo")->getChildren();
		for (int i = 0; i < locks.size(); i++){
			vector<char*> areaIds;
			Mainstatus::strSplit(locks.at(i)->getName(), "_", areaIds);
			int area_id = atoi(areaIds.at(0));//区域号
			areaJudge[area_id].lockNode = dynamic_cast<Node*>(locks.at(i));

			areaJudge[area_id].lockAtl = dynamic_cast<ActionTimeline*>(areaJudge[i].lockNode->getActionByTag(areaJudge[i].lockNode->getTag()));
			areaJudge[area_id].lockAtl->pause();
			auto lock_timeline = Timeline::create();
			lock_timeline->addFrame(createEventFrame(0, "", true));
			char valueCh[32];
			sprintf(valueCh, "%d_lock", i);
			lock_timeline->addFrame(createEventFrame(areaJudge[area_id].lockAtl->getEndFrame(), valueCh, true));
			areaJudge[area_id].lockAtl->addTimeline(lock_timeline);
			areaJudge[area_id].lockAtl->setFrameEventCallFunc(CC_CALLBACK_1(AreaMgr::onFrameEvent, this));

			areaJudge[area_id].lockColor.r = atoi(areaIds.at(2));
			areaJudge[area_id].lockColor.g = atoi(areaIds.at(3));
			areaJudge[area_id].lockColor.b = atoi(areaIds.at(4));

			areaJudge[area_id].lockNum = atoi(areaIds.at(1));

			locks.at(i)->getChildByTag(621)->setColor(areaJudge[area_id].lockColor);
			locks.at(i)->getChildByTag(621)->setCascadeColorEnabled(false);
			TextBMFont* lockTbf = dynamic_cast<TextBMFont*>(locks.at(i)->getChildByTag(621)->getChildByTag(84));
			lockTbf->setString(areaIds.at(1));
			lockTbf->setColor(Color3B(255, 255, 255));
		}
	}
	initStatus();
}

void AreaMgr::initStatus(){
	for (int i = 0; i < areaQty; i++){
		//灯泡 
		if (areaJudge[i].ligthOnSpr!=nullptr)
			areaJudge[i].ligthOnSpr->setVisible(false);
		//锁  
		if (areaJudge[i].lockNode != nullptr){
			areaJudge[i].lockNode->setVisible(true);
			areaJudge[i].lockAtl = dynamic_cast<ActionTimeline*>(areaJudge[i].lockNode->getActionByTag(areaJudge[i].lockNode->getTag()));
			areaJudge[i].lockAtl->gotoFrameAndPause(0);
		}
		//状态 
		if (areaJudge[i].lockAtl != nullptr)
			areaJudge[i].AreaStatus_Lock = true;
		else 
			areaJudge[i].AreaStatus_Lock = false;

		if (i == 0)//判断当前区域有没有线，默认为false ，除了第一区一直未true 
			areaJudge[i].AreaStatus_PanDing = true;
		else
			areaJudge[i].AreaStatus_PanDing = false;

		areaJudge[i].AreaStatus_Bulb = false;
	}
	nowAreaId = -1;
	lastPoint = Vec2(-1,-1);
	changeHiddenAreaIdAndCircleObstacle();//初始化隐藏区域 
}

void AreaMgr::changeHiddenAreaIdAndCircleObstacle()
{
	hiddenAreaId->clear();
	for (int i = 0; i < areaQty; i++){
		if (!areaJudge[i].AreaStatus_PanDing&&!areaJudge[i].AreaStatus_Bulb){//当前不在此区域或者灯泡未亮 
			hiddenAreaId->push_back(i);
		}
	}
	circleMgr->changeCircleVisible(hiddenAreaId);//重置球球状态 
	obstacleMgr->changeObstacleVisible(hiddenAreaId);//重置障碍物状态 
}

AreaMgr::AreaResult AreaMgr::JudgeArea(Point targetPos)
{
	AreaResult result = AreaResult::AreaResult_Nothing;
	if (nowAreaId == -1)
		return result;
	//for (int i = areaQty - 1; i >= 0; i--){
	//	if (areaJudge[i].AreaStatus_PanDing){
	//		now_area = i;
	//		break;
	//	}
	//}
	//灯泡 
	if (areaJudge[nowAreaId].ligthOnSpr != nullptr){
		if (Mainstatus::isPengZhuang(areaJudge[nowAreaId].ligthOnSpr, targetPos)){
			areaJudge[nowAreaId].ligthOnSpr->setVisible(true);
			areaJudge[nowAreaId].AreaStatus_Bulb = true;
			changeHiddenAreaIdAndCircleObstacle();	//重置球球状态 
		}
	}
	//锁 
	if (areaJudge[nowAreaId].AreaStatus_Lock){
		Sprite* lockSpr = dynamic_cast<Sprite*>(areaJudge[nowAreaId].lockNode->getChildByTag(621));
		if (Mainstatus::isPengZhuang(lockSpr, targetPos)){//有锁并碰到锁 
			bool is_jiesuo = circleMgr->isJieSuo(areaJudge[nowAreaId].lockColor, areaJudge[nowAreaId].lockNum);
			if (is_jiesuo){
				//锁 
				areaJudge[nowAreaId].AreaStatus_Lock = false;
				areaJudge[nowAreaId].lockAtl->gotoFrameAndPlay(0, false);
			}
			else {
				result = AreaResult::AreaResult_DealRemove;
				return result;//不判定判定区了 
			}
		}
	}
	//判定区 
	for (int i = 0; i < areaQty; i++){
		if (areaJudge[i].JudgeSpr == nullptr)
			continue;
		if (Mainstatus::isPengZhuang(areaJudge[i].JudgeSpr, targetPos)){
			if (Mainstatus::isPengZhuang(areaJudge[i].JudgeSpr, lastPoint)){//防止多次判定 
				break;
			}
			if (i == nowAreaId){//当前区域往上一区域  
				lastPoint = targetPos;
				nowAreaId--;
				areaJudge[nowAreaId].AreaStatus_PanDing = true;
				changeHiddenAreaIdAndCircleObstacle();	//重置球球状态 
			}
			else if (i == (nowAreaId + 1)){//当前区域往下一区域 
				lastPoint = targetPos;
				nowAreaId++;
				areaJudge[nowAreaId].AreaStatus_PanDing = true;
				changeHiddenAreaIdAndCircleObstacle();	//重置球球状态 
			}
			break;
		}
	}
	return result;
}

void AreaMgr::resetPanDing()
{
	nowAreaId = -1;
	lastPoint = Vec2(-1, -1);
	for (int i = 1; i < areaQty; i++){//除了第一个区域 
		areaJudge[i].AreaStatus_PanDing = false;
	}
	changeHiddenAreaIdAndCircleObstacle();	//重置球球状态 

}

void AreaMgr::onFrameEvent(Frame* frame){
	EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
	if (!evnt)
		return;
	vector<char*> results;
	Mainstatus::strSplit(evnt->getEvent(), "_", results);
	if (results.size() == 0)
		return;
	string eventName = (string)results.at(1);
	if (eventName.compare("lock") == 0){
		areaJudge[atoi(results.at(0))].lockAtl->stop();
		areaJudge[atoi(results.at(0))].lockNode->setVisible(false);
		areaJudge[atoi(results.at(0))].lockAtl = nullptr;
	}
}

void AreaMgr::resetAll()
{
	initStatus();
}
/**********天鸣**********/
#include "CircleNode.h"
#include "../GameAreaLayer.h"
#include "MainStatus.h"

const string Circle::bombDjsTime = "9";
const float Circle::bombTime = 5.0f;
//const float Circle::bombTime = 0.5f;

#define TAG "CircleNode"

const int Circle::ColorQty = 6;
map<string, int> Circle::ColorTypeMap;
Circle::Circle(Node* circle_node, ArroundObj* arround_obj)
{
	arroundObj = arround_obj;

	string circle_name[] = { "251_100_107", "255_231_27", "118_236_115", "140_213_255", "141_82_251" };
	if (ColorTypeMap.size() <= 0){
		for (int i = 0; i<Circle::ColorQty - 1; i++){//-1为了不包括ccs
			Circle::ColorTypeMap.insert(pair<string, int>(circle_name[i], i));
		}
	}

	Sprite* circle_spr = dynamic_cast<Sprite*>(circle_node);
	if (circle_spr == nullptr)
		initBomb(circle_node);
	else 
		initCircle(circle_node);
}

Circle::~Circle()
{

}
void Circle::initBomb(Node* circle_node)
{
	circleNode = circle_node;
	circleAtl = dynamic_cast<ActionTimeline*>(circleNode->getActionByTag(circleNode->getTag()));

	Timeline* circle_timeline = Timeline::create();
	circle_timeline->addFrame(createEventFrame(0, "", true));
	circle_timeline->addFrame(createEventFrame(95, "djsend", true));
	circleAtl->addTimeline(circle_timeline);
	circleAtl->setFrameEventCallFunc(CC_CALLBACK_1(Circle::onFrameEvent, this));

	circleSpr = dynamic_cast<Sprite*>(circleNode->getChildByTag(437));

	vector<char*> nameStrs;
	Mainstatus::strSplit(circleNode->getName(), "_", nameStrs);
	circleType = ColorType::bomb;

	circleNode->setCascadeColorEnabled(false);
	circleNode->setColor(Color3B(0,0,0));

	areaId = atoi(nameStrs.at(0));

	TextBMFont* djsTbf = TextBMFont::create(Circle::bombDjsTime, "36hao.fnt");
	djsTbf->setColor(Color3B(255, 255, 255));
	djsTbf->setPosition(Vec2((1 - circleSpr->getAnchorPoint().x)*circleSpr->getContentSize().width, (1 - circleSpr->getAnchorPoint().y)*circleSpr->getContentSize().height));
	circleSpr->addChild(djsTbf,1,20);
	djsTbf->runAction(Sequence::create(DelayTime::create(bombTime), CallFunc::create(CC_CALLBACK_0(Circle::bombDJS, this, djsTbf)), nullptr));

	initData();

}

void Circle::initCircle(Node* circle_node)
{

	Sprite* circle_spr = dynamic_cast<Sprite*>(circle_node);
	//动画 
	circleNode = CSLoader::createNode("xiaochu_.csb");
	circleNode->setPosition(circle_spr->getPosition());
	circle_spr->getParent()->addChild(circleNode);

	circleAtl = CSLoader::createTimeline("xiaochu_.csb");
	circleNode->runAction(circleAtl);
	circleNode->pause();
	circleAtl->gotoFrameAndPause(0);

	circleSpr = dynamic_cast<Sprite*>(circleNode->getChildByTag(3));
	circleNode->setScale(circle_spr->getScale());

	circle_spr->removeFromParentAndCleanup(true);

	Timeline* circle_timeline = Timeline::create();
	circle_timeline->addFrame(createEventFrame(0, "", true));
	circle_timeline->addFrame(createEventFrame(60, "hiddenEnd", true));
	circle_timeline->addFrame(createEventFrame(84, "showEnd", true));
	circle_timeline->addFrame(createEventFrame(88, "touchEnd", true));
	circle_timeline->addFrame(createEventFrame(120, "lianxianEnd", true));
	circleAtl->addTimeline(circle_timeline);
	circleAtl->setFrameEventCallFunc(CC_CALLBACK_1(Circle::onFrameEvent, this));


	vector<char*> colorRGBs;
	Mainstatus::strSplit(circle_spr->getName(), "_", colorRGBs);

	string colorName = (string)colorRGBs.at(1) + "_" + (string)colorRGBs.at(2) + "_" + (string)colorRGBs.at(3);
	auto result = Circle::ColorTypeMap.find(colorName);
	if (result != Circle::ColorTypeMap.end()){//已有移动方式不包括ccs档
		circleType = result->second;
	}
	else {
		circleType = ColorType::otherColor;
	}

	circleNode->setColor(Color3B(atoi(colorRGBs.at(1)), atoi(colorRGBs.at(2)), atoi(colorRGBs.at(3))));

	areaId = atoi(colorRGBs.at(0));

	initData();
}

void Circle::initData()
{
	isRemoved = false;
	isHiddenArea = false;
	actFrameName = "";
	circleAtl->gotoFrameAndPause(0);
	if (circleType == ColorType::bomb){
		circleSpr->setVisible(true);
		TextBMFont* djsTbf = dynamic_cast<TextBMFont*>(circleSpr->getChildByTag(20));
		djsTbf->stopAllActions();
		djsTbf->setString(Circle::bombDjsTime);
		djsTbf->setVisible(true);
		djsTbf->runAction(Sequence::create(DelayTime::create(bombTime), CallFunc::create(CC_CALLBACK_0(Circle::bombDJS, this, djsTbf)), nullptr));
	}
}

void Circle::bombDJS(TextBMFont* djsTbf)
{ 
	int time = atoi(djsTbf->getString().c_str());
	if (!GameArea::ActionIsPause){
		time--;
		char timech[16];
		sprintf(timech, "%d", time);
		djsTbf->setString(timech);
	}
	if (time == 0){
		actFrameName = "djsend";
		circleAtl->gotoFrameAndPlay(75, 95, false);
	}
	else {
		djsTbf->runAction(Sequence::create(DelayTime::create(bombTime), CallFunc::create(CC_CALLBACK_0(Circle::bombDJS, this, djsTbf)), nullptr));
	}
}

void Circle::doAction(ActionType action_type)
{
	switch (action_type)
	{
	case Circle::Action_Touch:
		if (circleType == ColorType::bomb)
			return;
		circleNode->setVisible(true);
		actFrameName = "touchEnd";
		circleAtl->gotoFrameAndPlay(85, 88, false);
		break;
	case Circle::Action_Show:
		if (circleType == ColorType::bomb)
			return;
		circleNode->setVisible(true);
		actFrameName = "showEnd";
		circleAtl->gotoFrameAndPlay(61, 84, false);
		break;
	case Circle::Action_Hidden:
		if (circleType == ColorType::bomb){
			circleSpr->getChildByTag(20)->setVisible(false);
			circleSpr->getChildByTag(20)->stopAllActions();
			circleSpr->setVisible(false);
			isRemoved = true;
		}
		else {
			bool is_remove = true;
			if (arroundObj != nullptr){
				if (arroundObj->qipaoSpr != nullptr){
					if (arroundObj->qipaoSpr->isVisible()){
						is_remove = false;
						arroundObj->qipaoSpr->setVisible(false);
						circleAtl->gotoFrameAndPlay(0, 0, false);
					}
				}
			}
			if (is_remove){
				circleAtl->pause();
				circleNode->setVisible(true);
				actFrameName = "hiddenEnd";
				circleAtl->gotoFrameAndPlay(0, 60, false);
				isRemoved = true;
			}
		}
		break;
	case Circle::Action_Zhuan:
		if (circleType == ColorType::bomb)
			return;
		actFrameName = "lianxianEnd";
		circleAtl->gotoFrameAndPlay(89, 120, true);
		break;
	default:
		break;
	}
}

void Circle::onFrameEvent(Frame* frame)
{
	EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
	if (!evnt)
		return;
	string str = evnt->getEvent();
	//防止跑到前一个timeline 
	if (actFrameName.compare("") == 0)
		return;
	if (actFrameName.compare(str) != 0)
		return;
	//判断是哪个动画 
	if (str.compare("hiddenEnd") == 0){
	}
	else if (str.compare("showEnd") == 0){
	}
	else if (str.compare("touchEnd") == 0){
		NotificationCenter::getInstance()->postNotification("circletouchFinish", nullptr);
	}
	else if (str.compare("lianxianEnd") == 0){

	}
	else if (str.compare("djsend") == 0){
		NotificationCenter::getInstance()->postNotification("gameFail", nullptr);
	}

}

bool Circle::getIsCanTouch()
{
	if (arroundObj == nullptr)
		return true;
	else if (arroundObj->kuangkuangNode != nullptr){
		if (arroundObj->kuangkuangNode->isVisible()){
			return  false;
		}else 
			return true;
	}
	else
		return true;
}

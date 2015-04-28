/**********天鸣**********/
#include "SysSetting.h"
#define TAG "SysSetting"

bool SysSetting::init()
{
	if(!Layer::init()){
		return false;
	}
    winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	sysSettingNode = CSLoader::createNode("bg2Scene.csb"); 
	this->addChild(sysSettingNode,1); 

	Node* centerNode = sysSettingNode->getChildByTag(300);
	centerAtl = dynamic_cast<ActionTimeline*>(centerNode->getActionByTag(centerNode->getTag()));
	centerAtl->gotoFrameAndPause(0);

	Button* chucunBtn = dynamic_cast<Button*>(centerNode->getChildByTag(106));
	chucunBtn->setTouchEnabled(false);

	initTouch();

	return true;
}

void SysSetting::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(SysSetting::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
}

void SysSetting::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(SysSetting::removeSelf,this)));
	}
}

void SysSetting::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void SysSetting::onExit()
{
	Layer::onExit();
}
void SysSetting::onEnter()
{
	Layer::onEnter();
}

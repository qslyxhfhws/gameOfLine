/**********天鸣**********/
#include "Achievement.h"
#define TAG "Achievement"

bool Achievement::init()
{
	if(!Layer::init()){
		return false;
	}
    winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	achievementNode = CSLoader::createNode("bg9Scene.csb"); 
	this->addChild(achievementNode,1); 

	initTouch();

	return true;
}

void Achievement::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(Achievement::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
}

void Achievement::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(Achievement::removeSelf,this)));
	}
}

void Achievement::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void Achievement::onExit()
{
	Layer::onExit();
}
void Achievement::onEnter()
{
	Layer::onEnter();
}

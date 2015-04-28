/**********天鸣**********/
#include "LockLayer.h"
#define TAG "LockLayer"

bool Lock::init()
{
	if(!Layer::init()){
		return false;
	}
    winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode,0);

	lockNode = CSLoader::createNode("bg4Scene.csb"); 
	this->addChild(lockNode,1); 

	Sprite* hulSpr = Sprite::create("huli.png");
	hulSpr->setScale(winSize.width/5*4 / hulSpr->getContentSize().width);
	hulSpr->setPosition(winSize.width/2,winSize.height/2);
	this->addChild(hulSpr, 1);

	initTouch();

	return true;
}

void Lock::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(Lock::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch,Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch,swallowNode);
}

void Lock::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if(suoyou.at(suoyou.size()-1) != this) return;
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(Lock::removeSelf,this)));
	}
}

void Lock::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);	
}

void Lock::onExit()
{
	Layer::onExit();
}
void Lock::onEnter()
{
	Layer::onEnter();
}

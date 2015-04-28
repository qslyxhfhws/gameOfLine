/**********天鸣**********/
#include "ExitLayer.h"
#define TAG "ExitLayer"

bool ExitLayer::init()
{
	if (!Layer::init()){
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();

	swallowNode = Node::create();
	swallowNode->setContentSize(winSize);
	this->addChild(swallowNode, 0);

	exitNode = CSLoader::createNode("tuichujiemian.csb");
	this->addChild(exitNode, 1);

	yesBtn = dynamic_cast<Button*>(exitNode->getChildByTag(757));
	yesBtn->addTouchEventListener(CC_CALLBACK_2(ExitLayer::touchEvent, this));

	noBtn = dynamic_cast<Button*>(exitNode->getChildByTag(758));
	noBtn->addTouchEventListener(CC_CALLBACK_2(ExitLayer::touchEvent, this));

	initTouch();

	return true;
}

void ExitLayer::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED){
		if (pSender == yesBtn){
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
				MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
				return;
			#endif

				Director::getInstance()->end();
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
			#endif

		}
		else if (pSender == noBtn){
			this->runAction(CallFunc::create(CC_CALLBACK_0(ExitLayer::removeSelf, this)));
		}
	}
}

void ExitLayer::initTouch()
{
	//实体键 
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyReleased = CC_CALLBACK_2(ExitLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	//吞噬touch
	auto swallowTouch = EventListenerTouchOneByOne::create();
	swallowTouch->setSwallowTouches(true);
	swallowTouch->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouch, swallowNode);
}

void ExitLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	Scene* nowScene = Director::getInstance()->getRunningScene();
	Vector<Node*> suoyou = nowScene->getChildren();
	if (suoyou.at(suoyou.size() - 1) != this) return;
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->runAction(CallFunc::create(CC_CALLBACK_0(ExitLayer::removeSelf, this)));
	}
}

void ExitLayer::removeSelf()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
}

void ExitLayer::onExit()
{
	Layer::onExit();
}
void ExitLayer::onEnter()
{
	Layer::onEnter();
}

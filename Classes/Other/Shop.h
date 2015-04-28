/**********天鸣**********/
#ifndef __SHOP_LAYER_H__
#define __SHOP_LAYER_H__

#include "../ObjectNode/ItemNode.h"
#include "TMUtility.h"

class Shop : public cocos2d::Layer
{
public :
	Shop();
	~Shop();
	virtual void onExit();
	virtual void onEnter();
	virtual bool init();
    CREATE_FUNC(Shop);
private :
	Size winSize;

	Node* swallowNode;
	Node* shopNode;

	TextBMFont* diamondNumTbf;//钻石 
	vector<Item*>* itemList;
	TextBMFont** itemTbf;
	Button** itemBtn;

	void initTouch();
	void touchEvent(Ref *pSender, Widget::TouchEventType type);
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event);
	void removeSelf();

	void buyItem(int itemId);
};

#endif
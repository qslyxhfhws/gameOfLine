/**********天鸣**********/
#include "SpecialNode.h"
#include "MainStatus.h"
#define TAG "SpecialNode"

const int Special::specialQty = 2;
map<string, int> Special::SpecialTypeMap;

Special::Special(Node* special_node)
{
	specialNode = special_node;
	winSize = Director::getInstance()->getVisibleSize();
	
	string special_name[] = { "kuang" ,"qipao"};
	if (SpecialTypeMap.size() <= 0){
		for (int i = 0; i<Special::specialQty; i++){
			Special::SpecialTypeMap.insert(pair<string, int>(special_name[i], i));
		}
	}
	specialinit();
}

Special::~Special()
{
	CC_SAFE_DELETE(specialSprs);
}

void Special::specialinit()
{
	specialSprs = nullptr;
	specialAtl = nullptr;
	specialSprs = new vector < Sprite* >;
	itemType = 0;
	areaId = -1;
	firstPos = Vec2(0, 0);
	vector<char*> resultStr;
	string name;
	Mainstatus::strSplit(specialNode->getName(), "_", resultStr);
	if (resultStr.size()>1){
		name = resultStr.at(0);
		areaId = atoi(resultStr.at(resultStr.size()-1));
	}
	else {
		name = resultStr.at(0);
	}
	auto result = Special::SpecialTypeMap.find(name);
	if (result != Special::SpecialTypeMap.end()){
		itemType = result->second;
	}
	if (itemType == SpecialType::kuangKuang){
		specialNode->setColor(Color3B(atoi(resultStr.at(1)), atoi(resultStr.at(2)), atoi(resultStr.at(3))));
	}
	if (specialNode->getTag() >= 20000) {
		specialAtl = dynamic_cast<ActionTimeline*>(specialNode->getActionByTag(specialNode->getTag()));
		//specialAtl->gotoFrameAndPause(0);
	}
	else {//默认为零，不用更改type
		Sprite* specialSpr = dynamic_cast<Sprite*>(specialNode);
		specialSprs->push_back(specialSpr);
	}

	dealWithAction();
}

void Special::dealWithAction()
{
	if (itemType == SpecialType::kuangKuang){
		specialAtl->gotoFrameAndPause(0);
		for (int i = 0; i < 4; i++){
			Sprite* spr = dynamic_cast<Sprite*>(specialNode->getChildByTag(113)->getChildByTag(114+i));
			specialSprs->push_back(spr);
		}
	}
}

void Special::reset()
{
	if (itemType == SpecialType::kuangKuang){
		specialAtl->gotoFrameAndPause(0);
		specialNode->setVisible(true);
	}
	else if (itemType == SpecialType::qipao){
		specialNode->setVisible(true);
	}
}
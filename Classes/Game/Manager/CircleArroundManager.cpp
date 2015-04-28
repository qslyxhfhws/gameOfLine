/**********天鸣**********/
#include "CircleArroundManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <math.h>
#define TAG "CircleArroundManager"

CircleArrMgr::CircleArrMgr(Node* game_area, string file_name)
{
	circleArrounds = new vector < CircleArrMgr::CircleArround* >;
	getCircleInfo(file_name);
	specials = nullptr;

 	init(game_area);

}

CircleArrMgr::~CircleArrMgr()
{
	if (specials!=nullptr)
		for (int i = 0; i < specials->size(); i++){
			CC_SAFE_DELETE(specials->at(i));
		}
		CC_SAFE_DELETE(specials);

	for (int i = 0; i < circleArrounds->size(); i++){
		CC_SAFE_DELETE(circleArrounds->at(i));
	}
	CC_SAFE_DELETE(circleArrounds);
}

 void CircleArrMgr::init(Node* game_area){
	if (game_area->getChildByName("Node_circleArround") == nullptr)
		return;
	else if ((game_area->getChildByName("Node_circleArround")->getChildren()).size() == 0)
		return;

	Vector<Node*> tempSpecialObstacles = game_area->getChildByName("Node_circleArround")->getChildren();
	specials = new vector < Special* >;
	specials->clear();
	for (int i = 0; i < tempSpecialObstacles.size(); i++){
		Special* special = new Special(tempSpecialObstacles.at(i));
		specials->push_back(special);
	}
}

int CircleArrMgr::isImpact(Point targetPos){
	//特殊物品 
	int specialId = -1;
	if (specials == nullptr)
		return specialId;
	for (int i = 0; i < specials->size(); i++){
		if (!specials->at(i)->specialNode->isVisible())
			continue;
		if (specials->at(i)->itemType == Special::SpecialType::qipao)
			continue;
		for (int j = 0; j < specials->at(i)->specialSprs->size(); j++){
			Point locationInNode = specials->at(i)->specialSprs->at(j)->convertToNodeSpace(targetPos);
			Size s = Size(specials->at(i)->specialSprs->at(j)->getContentSize().width, specials->at(i)->specialSprs->at(j)->getContentSize().height);
			Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode)){
				if (isObstacleAlpah255(specials->at(i)->specialSprs->at(j), locationInNode)){
					specialId = i;
					break;
				}
			}
		}
	}
	return specialId;
}

bool CircleArrMgr::isObstacleAlpah255(Sprite* targetSpr, Point locationInNode)
{
	Point pivot_pos = targetSpr->getAnchorPointInPoints();
	float angle_radians = -1*targetSpr->getRotationX()*M_PI/180.f;
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
	if (c.a >= 200)
		return true;
	else
		return false;
	//return false;
}

void CircleArrMgr::kuangkuangIsHidden(int id, int touch_size, Color3B color3b)
{
	if (touch_size < 2)
		return;
	if (specials->at(id)->itemType == Special::SpecialType::kuangKuang){
		if (specials->at(id)->specialNode->getColor() == color3b){
			TextBMFont* numberTbf = dynamic_cast<TextBMFont*>(specials->at(id)->specialNode->getChildByName("number"));
			int num = atoi(numberTbf->getString().c_str()) - touch_size;
			if (num < 0) num == 0;

			char numch[16];
			sprintf(numch, "%d", num);
			numberTbf->setString(numch);

			if (num == 0){
				specials->at(id)->specialAtl->pause();
				specials->at(id)->specialNode->setVisible(false);
				//specials->erase(specials->begin() + id);
			}
		}
	}
}

void CircleArrMgr::getCircleInfo(string file_name)
{
	circleArrounds->clear();
	String filename = file_name+".json";
	rapidjson::Document doc;
	//判断文件是否存在  
	if (!FileUtils::getInstance()->isFileExist(filename.getCString()))
	{
		return;
	}
	//读取文件数据，初始化doc  
	string data = FileUtils::getInstance()->getStringFromFile(filename.getCString());
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//判断读取成功与否 和 是否为数组类型  
	if (doc.HasParseError() || !doc.IsArray())
	{
		return;
	}
	for (unsigned int i = 0; i<doc.Size(); i++)
	{
		//逐个提取数组元素（声明的变量必须为引用）  
		rapidjson::Value &v = doc[i];

		//判断各属性是否存在（可以支持中文（UTF8格式））  
		CircleArrMgr::CircleArround* circleInfo = new CircleArrMgr::CircleArround;
		//按属性提取数据  
		circleInfo->CircleTag = v["tag"].GetInt();
		circleInfo->kuangkuangTag = v["kuangkuangtag"].GetInt();
		circleInfo->qipaoTag = v["qipaotag"].GetInt();
		circleArrounds->push_back(circleInfo);
	}
}

Circle::ArroundObj* CircleArrMgr::getArroundObj(int circle_tag)
{ 
	Circle::ArroundObj* arround_obj = new Circle::ArroundObj;
	//查找这个球球旁边是否有东西 
	auto result = find_if(circleArrounds->cbegin(), circleArrounds->cend(), [circle_tag](CircleArrMgr::CircleArround* circle_info){
		return (circle_info->CircleTag == circle_tag);
	});
	if (result != circleArrounds->cend()){//球球旁边有东西 
		//框框 
		int kuangkuang_tag = (*result)->kuangkuangTag;
		if (kuangkuang_tag >= 0){//如果有框框 
			//查找相对应的node 
			auto findArround = find_if(specials->cbegin(), specials->cend(), [kuangkuang_tag](Special* special){
				return (special->specialNode->getTag() == kuangkuang_tag);
			});
			if (findArround != specials->cend())
				arround_obj->kuangkuangNode = (*findArround)->specialNode;

		}
		else {
			arround_obj->kuangkuangNode = nullptr;
		}
		//气泡  
		int qipao_tag = (*result)->qipaoTag;
		if (qipao_tag >= 0){//如果有框框 
			//查找相对应的node 
			auto findArround = find_if(specials->cbegin(), specials->cend(), [qipao_tag](Special* special){
				return (special->specialNode->getTag() == qipao_tag);
			});
			if (findArround != specials->cend())
				arround_obj->qipaoSpr = dynamic_cast<Sprite*>((*findArround)->specialNode);

		}
		else {
			arround_obj->qipaoSpr = nullptr;
		}
	}
	else {
		arround_obj = nullptr;
	}
	return arround_obj;
}

void CircleArrMgr::resetAll()
{
	if (specials == nullptr)
		return;
	for (int i = 0; i < specials->size(); i++){
		specials->at(i)->reset();
		if (specials->at(i)->itemType == Special::SpecialType::kuangKuang){
			TextBMFont* numberTbf = dynamic_cast<TextBMFont*>(specials->at(i)->specialNode->getChildByName("number"));
			numberTbf->setString("5");
		}
	}
}

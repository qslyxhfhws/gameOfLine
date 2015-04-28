/**********天鸣**********/
#include "GameAreaLayer.h"
#include "GameLayer.h"
#include "MainStatus.h"
#define TAG "GameAreaLayer"

const int GameArea::ZORDER_DRAW = 3;
bool GameArea::ActionIsPause = false;

GameArea* GameArea::create(GameUIMgr* game_ui_mgr, string file_name)
{
	GameArea* gameArea = new GameArea(game_ui_mgr, file_name);
	if (gameArea && gameArea->init()) 
	{
		gameArea->autorelease();
	}
	else
	{
		delete gameArea;
		gameArea = NULL;
	}
	return gameArea;
}

GameArea::GameArea(GameUIMgr* game_ui_mgr, string file_name)
{
	circleMgr = nullptr;
	obstacleMgr = nullptr;
	areaMgr = nullptr;
	gameUIMgr = game_ui_mgr;
	fileName = file_name;
}

GameArea::~GameArea()
{
	CC_SAFE_DELETE(circleMgr);
	CC_SAFE_DELETE(obstacleMgr);
	CC_SAFE_DELETE(areaMgr);
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool GameArea::init()
{
	if (!Layer::init()){
		return false;
	}
	initData();

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameArea::gameFail), "gameFail", NULL);

	gameNode = CSLoader::createNode(fileName+".csb");
	this->addChild(gameNode);

	//背景精灵
	bgSpr = dynamic_cast<Sprite*>(gameNode->getChildByName("Sprite_baidi"));
	bgSpr->setOpacity(255);

	colorLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSpr->getContentSize().width*bgSpr->getScaleX(), bgSpr->getContentSize().height*bgSpr->getScaleY());
	colorLayer->setAnchorPoint(Vec2(0, 0));
	colorLayer->setPosition(0,0);
	bgSpr->addChild(colorLayer);


	//球球 
	circleMgr = new CircleMgr(gameNode, fileName);
	circleMgr->getAreaQty(areaQty);

	//障碍物 
	obstacleMgr = new ObstacleMgr(gameNode);

	//判定 
	areaMgr = new AreaMgr(gameNode, areaQty, circleMgr, obstacleMgr);

	//draw
	draw = DrawNode::create();
	addChild(draw, GameArea::ZORDER_DRAW);

	this->pauseSchedulerAndActions();

	initTouch();

	return true;
}

void GameArea::initData(){
	winSize = Director::getInstance()->getVisibleSize();
	lastPos = Vec2(-1, -1);
	isCanTouch = true;
	isFirstGameOver = true;
}

void GameArea::initTouch()
{
	//跳过动画 
	auto layerTouchLis = EventListenerTouchOneByOne::create();
	layerTouchLis->setSwallowTouches(true);
	layerTouchLis->onTouchBegan = [&](Touch* touch, Event* event){
		if (!isCanTouch)
			return false;
		if (lastPos.x != -1 || lastPos.y != -1) //如果没touchend的话不进began,防止拉出两条线 
			return false;
		//初始化数值 
		draw->clear();
		circleMgr->lastTagNum = -1;
		isContinue = true;
		//判断是否在游戏区域
		Point location_in_node;
		bool is_draw_area;
		isDrawArea(touch, event, location_in_node, is_draw_area);
		if (is_draw_area){//判断触摸点是否在目标的范围内 
			lastPos = location_in_node;
			dealWithDraw(location_in_node);
			return true;
		}else{
			return false;
		}
	};
	layerTouchLis->onTouchMoved = [&](Touch* touch, Event* event){
		struct timeval tv;
		gettimeofday(&tv, NULL);
		long haomiao =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
		if (lasttime == haomiao){
			return;
		}

		if (!isCanTouch)
			return;
		if (!isContinue)
			return;
		//判断是否在游戏区域
		Point location_in_node;
		bool is_draw_area;
		isDrawArea(touch, event, location_in_node, is_draw_area);
		if (is_draw_area){
			dealWithDraw(location_in_node);
		}
		struct timeval tv2;
		gettimeofday(&tv2, NULL);
		lasttime = tv2.tv_sec * 1000 + tv2.tv_usec / 1000;
	};
	layerTouchLis->onTouchEnded = [&](Touch* touch, Event* event){
		if (!isCanTouch)
			return;
		if (!isContinue)
			return;
		//判断是否在游戏区域
		Point location_in_node;
		bool is_draw_area;
		isDrawArea(touch, event, location_in_node, is_draw_area);
		if (is_draw_area){
			dealWithDraw(location_in_node);
		}
		dealRemove();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(layerTouchLis, gameNode);

}

GameArea::DealPointResult GameArea::dealWithPoint(Point targetPos)
{
	DealPointResult deal_result = DealPointResult::DealPointResult_draw;
	//判断线段当前区域显示 
	AreaMgr::AreaResult result = areaMgr->JudgeArea(targetPos);
	if (result == AreaMgr::AreaResult_DealRemove){//如果有锁并不满足解锁条件  
		dealRemove();
		deal_result = DealPointResult::DealPointResult_break;
		return deal_result;
	}
	//判断当前是否碰到障碍物
	if (obstacleMgr->isImpact(targetPos)){
		dealRemove();
		deal_result = DealPointResult::DealPointResult_break;
		return deal_result;
	}
	//判断是否球球边框，返回true则同障碍物 
	int arroundId = circleMgr->circleArrMgr->isImpact(targetPos);
	if (arroundId>=0){
		circleMgr->dealArround(arroundId);
		dealRemove();
		deal_result = DealPointResult::DealPointResult_break;
		return deal_result;
	}
	//判断是否碰到球球 
	int beforeSize = circleMgr->getTouchCirclesSize();
	CircleMgr::AddOrCancel add_or_cancel = circleMgr->searchIsCircle(targetPos);
	if (add_or_cancel == CircleMgr::AddOrCancel::AddOrCancel_Add){
		dealBgColor();
	}
	else if (add_or_cancel == CircleMgr::AddOrCancel::AddOrCancel_Cancel){
		dealRemove();
		deal_result = DealPointResult::DealPointResult_break;
		return deal_result;
	}
	//如果还没画到球球，那么不画线 
	if (circleMgr->getTouchCirclesSize() == 0){
		deal_result = DealPointResult::DealPointResult_doNothing;
	}
	if (circleMgr->getTouchCirclesSize() == 1 && beforeSize == 0){
		circleMgr->changeAreaId(areaMgr->nowAreaId);
	}
	//isjixu = true;
	return deal_result;
}

//处理消除 
void GameArea::dealRemove()
{
	lastPos = Vec2(-1, -1);
	draw->clear();
	isContinue = false;

	bool is_gameover = circleMgr->removeTouchListAndIsGameOver();
	//isDraw = false;
	areaMgr->resetPanDing();	//重置判断区状态 
	dealBgColor();
	if (is_gameover){
		isCanTouch = false;
		Game* game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByTag(Mainstatus::TAG_GAME_ONE));
		game->dealGameResult(Game::GameResult::Result_Fail);
	}
	if (circleMgr->isAllRemove()){
		isCanTouch = false;
		Game* game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByTag(Mainstatus::TAG_GAME_ONE));
		game->dealGameResult(Game::GameResult::Result_Success);
	}
}

//处理背景色 
void GameArea::dealBgColor()
{
	int count = circleMgr->getTouchCirclesSize();
	if (count > 0){
		if (colorLayer->getOpacity() == 0){
			float color_r, color_g, color_b;
			circleMgr->changeLineColor(color_r, color_g, color_b);
			colorLayer->setColor(Color3B(color_r * 255.0, color_g * 255.0, color_b * 255.0));
		}
		float op = count*Mainstatus::onceOpacity;
		if (op > 0.2)
			op = 0.2;
		op = op * 255;
		colorLayer->setOpacity(op);
	}
	else {
		colorLayer->setOpacity(0);
	}
	//float colo
	//	circleMgr->getTouchCirclesSize()
	//bgSpr
}

//判断是否是游戏区域 
void GameArea::isDrawArea(Touch* touch, Event* event, Point &location_in_node, bool &is_draw_area)
{
	Rect rect = Mainstatus::getRectInTarget(bgSpr,nullptr);
	//游戏区域 
	//Size s = Size(bgSpr->getContentSize().width*bgSpr->getScaleX(), bgSpr->getContentSize().height*bgSpr->getScaleY());
	//Point pos = gameNode->convertToNodeSpace(Vec2(bgSpr->getPosition().x - bgSpr->getAnchorPoint().x*bgSpr->getContentSize().width, bgSpr->getPosition().y - bgSpr->getAnchorPoint().y*bgSpr->getContentSize().height));
	//Rect rect = Rect(pos.x, pos.y, s.width, s.height);

	auto target = static_cast<Node*>(event->getCurrentTarget());//获取的当前触摸的目标 
	location_in_node = target->convertToNodeSpace(touch->getLocation());

	if (rect.containsPoint(location_in_node))//判断触摸点是否在目标的范围内
		is_draw_area = true;
	else {
		is_draw_area = false;
	}

}

//画线 
void GameArea::dealWithDraw(Point locationInNode)
{
	float color_r = 1.0, color_g = 0.0, color_b = 0.0, color_a = 1.0;
	if (circleMgr->getTouchCirclesSize() > 0){
		circleMgr->changeLineColor(color_r, color_g, color_b);
		float dis = lastPos.distance(locationInNode);
		bool isGameOver = gameUIMgr->reduceByDis(dis);//有些许误差，应该不要紧 
		if (isGameOver){
			gameOver();
			return;
		}
	}
	if (locationInNode.x != lastPos.x || locationInNode.y != lastPos.y){//不一样
		float x = lastPos.x;
		float y = lastPos.y;
		float ZengJia = 1.0f;
		float biLv;
		if (fabs(lastPos.y - locationInNode.y) / fabs(locationInNode.x - lastPos.x)<1.0){//以x为准
			biLv = (ZengJia*fabs(locationInNode.y - lastPos.y)) / fabs(locationInNode.x - lastPos.x);
			if (locationInNode.x - lastPos.x<0) ZengJia = ZengJia*-1;
			if (locationInNode.y - lastPos.y<0) biLv = biLv*-1;
			if (locationInNode.x - lastPos.x<0)
				for (; x >= locationInNode.x;){
					DealPointResult deal_result = dealWithPoint(Vec2(x, y));
					if (deal_result == DealPointResult::DealPointResult_draw){
						circleMgr->changeLineColor(color_r, color_g, color_b);
						draw->drawDot(Vec2(x, y), 4, Color4F(color_r, color_g, color_b, color_a));
					}
					else if (deal_result == DealPointResult::DealPointResult_break){
						break;
					}
					y += biLv;
					x += ZengJia;
					lastPos = Vec2(x, y);
				}
			else
				for (; x <= locationInNode.x;){
					DealPointResult deal_result = dealWithPoint(Vec2(x, y));
					if (deal_result == DealPointResult::DealPointResult_draw){
						circleMgr->changeLineColor(color_r, color_g, color_b);
						draw->drawDot(Vec2(x, y), 4, Color4F(color_r, color_g, color_b, color_a));
					}
					else if (deal_result == DealPointResult::DealPointResult_break){
						break;
					}
					y += biLv;
					x += ZengJia;
					lastPos = Vec2(x, y);
				}
		}
		else {
			biLv = (ZengJia*fabs(locationInNode.x - lastPos.x)) / fabs(locationInNode.y - lastPos.y);
			if (locationInNode.y - lastPos.y<0) ZengJia = ZengJia*-1;
			if (locationInNode.x - lastPos.x<0) biLv = biLv*-1;
			if (locationInNode.y - lastPos.y<0)
				for (; y >= locationInNode.y;){
					DealPointResult deal_result = dealWithPoint(Vec2(x, y));
					if (deal_result == DealPointResult::DealPointResult_draw){
						circleMgr->changeLineColor(color_r, color_g, color_b);
						draw->drawDot(Vec2(x, y), 4, Color4F(color_r, color_g, color_b, color_a));
					}
					else if (deal_result == DealPointResult::DealPointResult_break){
						break;
					}
					x += biLv;
					y += ZengJia;
					lastPos = Vec2(x, y);
				}
			else
				for (; y <= locationInNode.y;){
					DealPointResult deal_result = dealWithPoint(Vec2(x, y));
					if (deal_result == DealPointResult::DealPointResult_draw){
						circleMgr->changeLineColor(color_r, color_g, color_b);
						draw->drawDot(Vec2(x, y), 4, Color4F(color_r, color_g, color_b, color_a));
					}
					else if (deal_result == DealPointResult::DealPointResult_break){
						break;
					}
					x += biLv;
					y += ZengJia;
					lastPos = Vec2(x, y);
				}
		}
	}
}

void GameArea::gameFail(Ref* ref)
{
	if (isFirstGameOver){
		isFirstGameOver = false;
		gameOver();
	}
}

void GameArea::gameOver()
{
	dealRemove();
	isCanTouch = false;
	draw->clear();
	obstacleMgr->stopallAct();
	Game* game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByTag(Mainstatus::TAG_GAME_ONE));
	game->dealGameResult(Game::GameResult::Result_Fail);
}

void GameArea::gameRest(bool is_btn_rest){
	circleMgr->resetAll();
	obstacleMgr->resetAll();
	areaMgr->resetAll();
	//if (!is_btn_rest)//以前的需求 
	gameUIMgr->resetAll();

}

void GameArea::onEnter()
{
	Layer::onEnter();
}

void GameArea::onExit()
{
	Layer::onExit();
}

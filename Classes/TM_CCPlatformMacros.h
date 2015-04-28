
#ifndef  __TM_CCPLATFORMMACROS_H_
#define  __TM_CCPLATFORMMACROS_H_

#define TM_CC_SAFE_RemoveFromParentAndCleanup_NULL(p, cleanup)\
	do {\
		if(p){\
			(p)->removeFromParentAndCleanup(cleanup);\
			(p) = nullptr;\
				}\
		} while(0)

#define TM_CC_SAFE_ActioinManagerEx_STOP(json, animation) \
	do { \
		ActionObject *action = ActionManagerEx::getInstance()->getActionByName(json, animation);\
		if(action) {\
				action->stop();\
				}\
		} while(0)

#define TM_CC_SAFE_ActioinManagerEx_STOP_REMOVE(json, animation)\
	do { \
		ActionObject *action = ActionManagerEx::getInstance()->getActionByName(json, animation);\
			if(action) {\
				action->stop();\
				ActionManagerEx::getInstance()->removeActionByName(json);\
						}\
		} while(0)

#define TM_CC_SAFE_ActioinManagerEx_REMOVE(json)\
	do {\
		ActionManagerEx::getInstance()->removeActionByName(json);\
		} while(0)

#define CREATE_EVENT_FRAME() \
static EventFrame* createEventFrame(int index, const char* name, bool tween) \
{ \
	EventFrame* frame = EventFrame::create(); \
	frame->setFrameIndex(index); \
	frame->setEvent(name); \
	if(tween) frame->setTween(tween); \
	return frame; \
}

#endif


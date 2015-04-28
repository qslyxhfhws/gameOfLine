/**********天鸣**********/
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace cocostudio::timeline;

#ifndef __TMTOOL_H__
#define __TMTOOL_H__
#define __TMTOOL_H_VER 20140811A
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#define  LOG_TAG    "HelloWorld"
//#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"",__VA_ARGS__)
//#define  LOGTAGD(TAG, ...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
//#define  LOGD(format, ...)  __android_log_print(ANDROID_LOG_DEBUG,"",format,##__VA_ARGS__)
#define  LOGD(TAG, format, ...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,format,##__VA_ARGS__)

#else
//#define LOGD(format,...) cocos2d::log(format, ##__VA_ARGS__)
//DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format"/n", __LINE__, ##__VA_ARGS__)
//#define  LOGD(...)  CCLOG(__VA_ARGS__)
//#define  LOGD(format, ...)  CCLOG("FUNCTION: "__FUNCTION__", LINE: %d: "format"", __LINE__, ##__VA_ARGS__)
//#define  LOGD(format, ...)  CCLOG("FILE: "__FILE__", FUNCTION: "__FUNCTION__", LINE: %d: "format"", __LINE__, ##__VA_ARGS__)
//#define  LOGD(format, ...)  CCLOG("FUNC: "__FUNCTION__", LINE: %d: "format"", __LINE__, ##__VA_ARGS__)
#define  LOGD(TAG, format, ...)  CCLOG(""TAG", FUNC: "__FUNCTION__", LINE: %d: "format"", __LINE__, ##__VA_ARGS__)
#endif
#endif


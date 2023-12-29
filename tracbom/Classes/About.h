#ifndef __ABOUT_H__
#define __ABOUT_H__

//#define _BETA
#include "cocos2d.h"
#include "ui\CocosGUI.h"
using namespace cocos2d::ui;

class About : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuReturnCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(About);
};

#endif
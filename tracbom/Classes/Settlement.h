#ifndef __SETTLEMENT_H__
#define __SETTLEMENT_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
using namespace cocos2d::ui;

class Settlement : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuReturnCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(Settlement);
};

#endif
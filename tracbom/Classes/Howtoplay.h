#ifndef __HOWTOPLAY_H__
#define __HOWTOPLAY_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
using namespace cocos2d::ui;

class HowtoPlay : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuReturnCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(HowtoPlay);
};

#endif
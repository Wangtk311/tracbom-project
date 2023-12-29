#ifndef __LAUNCHPAGE_H__
#define __LAUNCHPAGE_H__

#include "cocos2d.h"

class LaunchPage : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuNextCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(LaunchPage);
};

#endif
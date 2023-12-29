
#ifndef __SETDELAY_SCENE_H__
#define __SETDELAY_SCENE_H__

#include "cocos2d.h"

class SetDelay : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SetDelay);
};

#endif // __HELLOWORLD_SCENE_H__

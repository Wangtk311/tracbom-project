
#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"

class Play : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void update(float dt);
    // a selector callback
    void GamePre();//��ȡ�ļ�֮��׼������
    void CreateNote();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void reTry();
    void Back();
    void conTinue();
    std::vector<std::vector<float>>nTap;
    std::vector<std::vector<float>>nHold;
    // implement the "static create()" method manually
    CREATE_FUNC(Play);
    //Size visibleSize;
    //Vec2 origin ;
    


};

#endif // __PLAY_SCENE_H__

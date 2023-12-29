#pragma once

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Track.h"

class NoteHold : public cocos2d::Sprite
{
public:


    virtual bool init(Track* track, float jud,float end);

    // implement the "static create()" method manually
    static NoteHold* create(Track* track, float jud,float end)
    {
        NoteHold* pRet = new(std::nothrow) NoteHold();

        if (pRet && pRet->init(track, jud,end))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }

    int trackNum;

private:
    Sprite* headPic;
    Sprite* bodyPic;
    Sprite* tailPic;

    bool ifhold = false;
    Color3B color = Color3B::WHITE;
    float judTime;
    float endTime;
    int num = 0;
    int mode=-1;//-2,-1,0,1,2,3对应 按过已经判定，还没按,miss,bad,good,perfect
};

#endif // __HELLOWORLD_SCENE_H__

#ifndef __NOTETAP_H__
#define __NOTETAP_H__

#include "cocos2d.h"
#include "Track.h"
#include <vector>

USING_NS_CC;

class NoteTap : public Sprite
{
public:

    virtual bool init(Track* track, float jud);


    // implement the "static create()" method manually
    static NoteTap* create(Track* track, float jud)
    {
        NoteTap* pRet = new(std::nothrow) NoteTap();
        
        if (pRet && pRet->init(track,jud))
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
    void setjudTime(float delaytime);//用于设置延迟页面

private:
    Sprite* bodyPic;
    float judTime;



};

#endif // __NOTETAP_H__



#ifndef __TRACK_H__
#define __TRACK_H__

#include "cocos2d.h"
#include <vector>
USING_NS_CC;

class Track : public Sprite
{
public:

    virtual bool init();


    // implement the "static create()" method manually
    static Track* create(std::string num)
    { 
        Track* pRet = new(std::nothrow) Track(); 
        pRet->TrackNumber = num;
        if (pRet && pRet->init()) 
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


    Sprite* bodyPic;
    Sprite* bodyPic2;
    Sprite* headPic;
    void SetX(float dt);
    void SetY(float dt);
    void SetAngle(float dt);
    void SetWidth(float dt);
    void SetOpacity(float dt);
    void SetSpeed(float dt);


    std::string TrackNumber;
    float X;
    float Y;
    float width=250;
    float angle=0;
    float opacity=0;
    float speed=2300;
    float startX;
    float startY;
    float startW;
    float startA;
    float startO;//GLubyte
    float startS;
    std::vector <std::vector<float>> attrX;
    std::vector <std::vector<float>> attrY;
    std::vector <std::vector<float>> attrWidth;//实际是比例而不是宽度
    std::vector <std::vector<float>> attrAngle;
    std::vector <std::vector<float>> attrOpacity;
    std::vector <std::vector<float>> attrSpeed;
};

#endif // __TRACK_H__

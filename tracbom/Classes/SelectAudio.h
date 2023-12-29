#ifndef __SELECTAUDIO_H__
#define __SELECTAUDIO_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
using namespace cocos2d::ui;

class SelectAudio : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuReturnCallback(cocos2d::Ref* pSender);
    void onchangedSlider1(cocos2d::Ref* pSender, Slider::EventType type);
    void onchangedSlider2(cocos2d::Ref* pSender, Slider::EventType type);
    void menuKeyset1Callback(cocos2d::Ref* pSender);
    void menuKeyset2Callback(cocos2d::Ref* pSender);
    void menuKeyset3Callback(cocos2d::Ref* pSender);
    void menuKeyset4Callback(cocos2d::Ref* pSender);
    void menuSetdelayCallback(cocos2d::Ref* pSender);

    Text* label1;
    Text* label2;
    Text* keylabel1;
    Text* keylabel2;
    Text* keylabel3;
    Text* keylabel4;

    CREATE_FUNC(SelectAudio);
};

#endif
#ifndef __SELECTSONG_H__
#define __SELECTSONG_H__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

class SelectSong : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    Text* highscoretext;
    Text* musicname;

    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuAboutCallback(Ref* pSender);
    void menuAudioCallback(cocos2d::Ref* pSender);
    void menuPlayCallback(Ref* pSender);
    void menuHowtoplayCallback(Ref* pSender);
    void menuChangediffCallback(RadioButton* button, int index, RadioButtonGroup::EventType type);
    void menuChangeSongCallback(RadioButton* button, int index, RadioButtonGroup::EventType type);

    CREATE_FUNC(SelectSong);
};

#endif
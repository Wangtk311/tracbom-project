#include "Howtoplay.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include <cstring>
#include <fstream>
#include <Windows.h>

//#define _BETA

using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;

extern int bgmvol;



Scene* HowtoPlay::createScene()
{
    return HowtoPlay::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}

bool HowtoPlay::init()
{

    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto backLayer = Layer::create();
    this->addChild(backLayer);


    backLayer->setContentSize(visibleSize);

    auto backPicture = Sprite::create("backanddecos/SelectSong.png");
    if (backPicture == nullptr)
    {
        problemLoading("'backanddecos/SelectSong.png'");
    }
    else
    {

        backPicture->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));

        backLayer->addChild(backPicture);
    }

    auto howtoplayBack = Sprite::create("backanddecos/howtoplaypic.png");
    if (howtoplayBack == nullptr)
    {
        problemLoading("'backanddecos/howtoplaypic.png'");
    }
    else
    {
        howtoplayBack->setScale(0.5f);
        howtoplayBack->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));
        howtoplayBack->setOpacity(190);
        backLayer->addChild(howtoplayBack);
    }

    auto backButton = MenuItemImage::create(
        "buttonanddiv/exitbutton.png",
        "buttonanddiv/exitbuttonpressed.png",
        CC_CALLBACK_1(HowtoPlay::menuReturnCallback, this));

    if (backButton == nullptr ||
        backButton->getContentSize().width <= 0 ||
        backButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/exitbutton.png'");
    }
    else
    {
        float x = origin.x - 5;
        float y = origin.y + visibleSize.height - backButton->getContentSize().height / 1.6;
        backButton->setScale(0.5f);
        backButton->setAnchorPoint(Vec2(0, 0));
        backButton->setPosition(Vec2(x, y));
    }


    auto pageTitle = Label::createWithTTF("How to play tracbom", "fonts/genshin.ttf", 40);
    if (pageTitle == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        pageTitle->setAnchorPoint(Vec2(0, 0.5));
        pageTitle->setPosition(Vec2(origin.x + 0.55 * backButton->getContentSize().width, origin.y + visibleSize.height - pageTitle->getContentSize().height * 2));


        backLayer->addChild(pageTitle);
    }



    auto backText = Label::createWithTTF("BACK", "fonts/genshin.ttf", 40);
    if (backText == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        backText->setPosition(Vec2(origin.x + backText->getContentSize().width / 1.2 - 3, origin.y + visibleSize.height - backText->getContentSize().height * 2));


        backLayer->addChild(backText);
    }

    auto menu = Menu::create(backButton, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setOpacity(150);

    backLayer->addChild(menu);


    return true;
}


void HowtoPlay::menuReturnCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false);

    Director::getInstance()->popScene();

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0);
}
#include "About.h"
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



Scene* About::createScene()
{
    return About::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}


bool About::init()
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

#ifndef _BETA
    auto aboutBack = Sprite::create("backanddecos/about.png");
    if (aboutBack == nullptr)
    {
        problemLoading("'backanddecos/about.png'");
    }
    else
    {
        aboutBack->setScale(0.5f);
        aboutBack->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));
        aboutBack->setOpacity(190);
        backLayer->addChild(aboutBack);
    }
#else
    auto aboutBack = Sprite::create("backanddecos/aboutbeta.png");
    if (aboutBack == nullptr)
    {
        problemLoading("'backanddecos/aboutbeta.png'");
    }
    else
    {
        aboutBack->setScale(0.5f);
        aboutBack->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));
        aboutBack->setOpacity(190);
        backLayer->addChild(aboutBack);
    }
#endif

    auto backButton = MenuItemImage::create(
        "buttonanddiv/exitbutton.png",
        "buttonanddiv/exitbuttonpressed.png",
        CC_CALLBACK_1(About::menuReturnCallback, this));

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


    auto pageTitle = Label::createWithTTF("About tracbom", "fonts/genshin.ttf", 40);
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


void About::menuReturnCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    Director::getInstance()->popScene();

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0);
}
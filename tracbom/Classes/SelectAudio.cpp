#include "SelectAudio.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "SetdelayScene.h"
#include <cstring>
#include <fstream>
#include <Windows.h>

using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;

extern int bgmvol;
extern int notevol;
extern short track1key;
extern short track2key;
extern short track3key;
extern short track4key;


Scene* SelectAudio::createScene()
{
    return SelectAudio::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}


bool SelectAudio::init()
{

    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto backLayer = Layer::create();
    this->addChild(backLayer);



    ifstream soundsettingsFile;
    soundsettingsFile.open("soundsettings.ini", ios::_Nocreate);
    if (!soundsettingsFile.is_open())
    {
        problemLoading("'soundsettings.ini'");
    }
    else
    {
        int bgmvol_read = 0, notevol_read = 0;
        soundsettingsFile >> bgmvol_read >> notevol_read;
        bgmvol = bgmvol_read;
        notevol = notevol_read;
        soundsettingsFile.close();
    }




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

    auto audioBack = Sprite::create("backanddecos/SelectAudioBack.png");
    if (audioBack == nullptr)
    {
        problemLoading("'backanddecos/SelectAudioBack.png'");
    }
    else
    {
        audioBack->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));
        audioBack->setScale(0.5);
        audioBack->setOpacity(230);
        backLayer->addChild(audioBack);
    }

    auto backButton = MenuItemImage::create(
        "buttonanddiv/exitbutton.png",
        "buttonanddiv/exitbuttonpressed.png",
        CC_CALLBACK_1(SelectAudio::menuReturnCallback, this));

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


    auto pageTitle = Label::createWithTTF("General Setting", "fonts/genshin.ttf", 40);
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

    Slider* bgmSlider = Slider::create();
    bgmSlider->loadBarTexture("buttonanddiv/emptybar.png");
    bgmSlider->loadSlidBallTextures("buttonanddiv/slideblock.png", "buttonanddiv/slideblockselected.png", "buttonanddiv/slideblockselected.png");
    bgmSlider->loadProgressBarTexture("buttonanddiv/selectedbar.png");
    bgmSlider->setPercent(bgmvol);
    bgmSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 - visibleSize.width / 21, origin.y + 3 * visibleSize.height / 5 - 120));
    bgmSlider->addEventListener(CC_CALLBACK_2(SelectAudio::onchangedSlider1, this));
    backLayer->addChild(bgmSlider);

    string s1 = to_string(bgmvol);
    label1 = Text::create(s1, "fonts/genshin.ttf", 40);
    label1->setPosition(bgmSlider->getPosition() + Vec2(840, 0));
    backLayer->addChild(label1);


    Slider* noteSlider = Slider::create();
    noteSlider->loadBarTexture("buttonanddiv/emptybar.png");
    noteSlider->loadSlidBallTextures("buttonanddiv/slideblock.png", "buttonanddiv/slideblockselected.png", "buttonanddiv/slideblockselected.png");
    noteSlider->loadProgressBarTexture("buttonanddiv/selectedbar.png");
    noteSlider->setPercent(notevol);
    noteSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 - visibleSize.width / 21, origin.y + 2 * visibleSize.height / 5 - 90));
    noteSlider->addEventListener(CC_CALLBACK_2(SelectAudio::onchangedSlider2, this));
    backLayer->addChild(noteSlider);

    string s2 = to_string(notevol);
    label2 = Text::create(s2, "fonts/genshin.ttf", 40);
    label2->setPosition(noteSlider->getPosition() + Vec2(840, 0));
    backLayer->addChild(label2);







    auto keyset1 = MenuItemImage::create(
        "buttonanddiv/keyset.png",
        "buttonanddiv/keysetselected.png",
        CC_CALLBACK_1(SelectAudio::menuKeyset1Callback, this));
    if (keyset1 == nullptr ||
        keyset1->getContentSize().width <= 0 ||
        keyset1->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/keyset.png'");
    }
    else
    {
        keyset1->setAnchorPoint(Vec2(0, 0));
        keyset1->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18, origin.y + 4 * visibleSize.height / 5.9));
    }

    auto keyset2 = MenuItemImage::create(
        "buttonanddiv/keyset.png",
        "buttonanddiv/keysetselected.png",
        CC_CALLBACK_1(SelectAudio::menuKeyset2Callback, this));
    if (keyset2 == nullptr ||
        keyset2->getContentSize().width <= 0 ||
        keyset2->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/keyset.png'");
    }
    else
    {
        keyset2->setAnchorPoint(Vec2(0, 0));
        keyset2->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + keyset1->getContentSize().width + 20, origin.y + 4 * visibleSize.height / 5.9));
    }

    auto keyset3 = MenuItemImage::create(
        "buttonanddiv/keyset.png",
        "buttonanddiv/keysetselected.png",
        CC_CALLBACK_1(SelectAudio::menuKeyset3Callback, this));
    if (keyset3 == nullptr ||
        keyset3->getContentSize().width <= 0 ||
        keyset3->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/keyset.png'");
    }
    else
    {
        keyset3->setAnchorPoint(Vec2(0, 0));
        keyset3->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 2 * (keyset2->getContentSize().width + 20), origin.y + 4 * visibleSize.height / 5.9));
    }

    auto keyset4 = MenuItemImage::create(
        "buttonanddiv/keyset.png",
        "buttonanddiv/keysetselected.png",
        CC_CALLBACK_1(SelectAudio::menuKeyset4Callback, this));
    if (keyset4 == nullptr ||
        keyset4->getContentSize().width <= 0 ||
        keyset4->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/keyset.png'");
    }
    else
    {
        keyset4->setAnchorPoint(Vec2(0, 0));
        keyset4->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 3 * (keyset3->getContentSize().width + 20), origin.y + 4 * visibleSize.height / 5.9));
    }


    auto menu1 = Menu::create(backButton, keyset1, keyset2, keyset3, keyset4, NULL);
    menu1->setPosition(Vec2::ZERO);
    menu1->setOpacity(150);

    backLayer->addChild(menu1);


    char b1[2] = { (char)track1key };
    b1[0] = track1key;
    keylabel1 = Text::create(b1, "fonts/Arial.ttf", 40);
    keylabel1->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 67, origin.y + 4 * visibleSize.height / 5.9 + 23));
    backLayer->addChild(keylabel1);

    char b2[2] = { (char)track2key };
    b2[0] = track2key;
    keylabel2 = Text::create(b2, "fonts/Arial.ttf", 40);
    keylabel2->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 224, origin.y + 4 * visibleSize.height / 5.9 + 23));
    backLayer->addChild(keylabel2);

    char b3[2] = { (char)track3key };
    b3[0] = track3key;
    keylabel3 = Text::create(b3, "fonts/Arial.ttf", 40);
    keylabel3->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 380, origin.y + 4 * visibleSize.height / 5.9 + 23));
    backLayer->addChild(keylabel3);

    char b4[2] = { (char)track4key };
    b4[0] = track4key;
    keylabel4 = Text::create(b4, "fonts/Arial.ttf", 40);
    keylabel4->setPosition(Vec2(origin.x + visibleSize.width / 2 + visibleSize.width / 18 + 537, origin.y + 4 * visibleSize.height / 5.9 + 23));
    backLayer->addChild(keylabel4);




    auto delaysetButton = MenuItemImage::create(
        "buttonanddiv/keyset.png",
        "buttonanddiv/keysetselected.png",
        CC_CALLBACK_1(SelectAudio::menuSetdelayCallback, this));
    if (delaysetButton == nullptr ||
        delaysetButton->getContentSize().width <= 0 ||
        delaysetButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/keyset.png'");
    }
    else
    {
        delaysetButton->setAnchorPoint(Vec2(0, 0));
        delaysetButton->setPosition(Vec2(origin.x + visibleSize.width / 4 + 173, origin.y + 4 * visibleSize.height / 5.9));
    }

    auto menu2 = Menu::create(delaysetButton, NULL);
    menu2->setPosition(Vec2::ZERO);
    menu2->setOpacity(150);

    backLayer->addChild(menu2);


    auto delaysetText = Label::createWithTTF("SET", "fonts/Arial.ttf", 40);
    if (delaysetText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        delaysetText->setAnchorPoint(Vec2(0, 0));
        delaysetText->setPosition(Vec2(delaysetButton->getPosition().x + 29, delaysetButton->getPosition().y + 1));


        backLayer->addChild(delaysetText);
    }



    return true;
}


void SelectAudio::menuReturnCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    //SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    ofstream soundsettingsFile;
    soundsettingsFile.open("soundsettings.ini", ios::_Nocreate | ios::out | ios::trunc);
    if (!soundsettingsFile.is_open())
    {
        problemLoading("'soundsettings.ini'");
    }
    else
    {
        soundsettingsFile << bgmvol << "  " << notevol;
        soundsettingsFile.close();
    }

    ofstream keysettingsFile;
    keysettingsFile.open("keysettings.ini", ios::_Nocreate | ios::out | ios::trunc);
    if (!keysettingsFile.is_open())
    {
        problemLoading("'keysettings.ini'");
    }
    else
    {
        keysettingsFile << track1key << "  " << track2key << "  " << track3key << "  " << track4key;
        keysettingsFile.close();
    }

    Director::getInstance()->popScene();
    //SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/selectsongbackground.mp3");
    //SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/selectsongbackground.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0f);
}

void SelectAudio::onchangedSlider1(Ref* pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        label1->setString(StringUtils::format("%d", percent));
        bgmvol = percent;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0f);
    }
}

void SelectAudio::onchangedSlider2(Ref* pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        label2->setString(StringUtils::format("%d", percent));
        notevol = percent;
        SimpleAudioEngine::getInstance()->setEffectsVolume(notevol / 100.0f);
    }
}



void SelectAudio::menuKeyset1Callback(Ref* pSender)
{

    MSG msg;
    while (1) {
        GetMessageW(&msg, NULL, 0, 0);
        if (msg.message == WM_KEYDOWN) {
            if ((msg.wParam >= 48 && msg.wParam <= 57) || (msg.wParam >= 65 && msg.wParam <= 90) && (msg.wParam != track2key && msg.wParam != track3key && msg.wParam != track4key))
            {
                track1key = msg.wParam;
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
                SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                break;
            }
            else {
                SimpleAudioEngine::getInstance()->playEffect("sounds/error.wav", false, 1.0, 0.0, notevol / 100.0);
            }
        }
    }
    keylabel1->setString(StringUtils::format("%c", track1key));
}

void SelectAudio::menuKeyset2Callback(Ref* pSender)
{

    MSG msg;
    while (1) {
        GetMessageW(&msg, NULL, 0, 0);
        if (msg.message == WM_KEYDOWN) {
            if ((msg.wParam >= 48 && msg.wParam <= 57) || (msg.wParam >= 65 && msg.wParam <= 90) && (msg.wParam != track1key && msg.wParam != track3key && msg.wParam != track4key))
            {
                track2key = msg.wParam;
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
                SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                break;
            }
            else {
                SimpleAudioEngine::getInstance()->playEffect("sounds/error.wav", false, 1.0, 0.0, notevol / 100.0);
            }
        }
    }
    keylabel2->setString(StringUtils::format("%c", track2key));
}

void SelectAudio::menuKeyset3Callback(Ref* pSender)
{

    MSG msg;
    while (1) {
        GetMessageW(&msg, NULL, 0, 0);
        if (msg.message == WM_KEYDOWN) {
            if ((msg.wParam >= 48 && msg.wParam <= 57) || (msg.wParam >= 65 && msg.wParam <= 90) && (msg.wParam != track1key && msg.wParam != track2key && msg.wParam != track4key))
            {
                track3key = msg.wParam;
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
                SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                break;
            }
            else {
                SimpleAudioEngine::getInstance()->playEffect("sounds/error.wav", false, 1.0, 0.0, notevol / 100.0);
            }
        }
    }
    keylabel3->setString(StringUtils::format("%c", track3key));
}

void SelectAudio::menuKeyset4Callback(Ref* pSender)
{

    MSG msg;
    while (1) {
        GetMessageW(&msg, NULL, 0, 0);
        if (msg.message == WM_KEYDOWN) {
            if ((msg.wParam >= 48 && msg.wParam <= 57) || (msg.wParam >= 65 && msg.wParam <= 90) && (msg.wParam != track1key && msg.wParam != track2key && msg.wParam != track3key))
            {
                track4key = msg.wParam;
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
                SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
                SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
                break;
            }
            else {
                SimpleAudioEngine::getInstance()->preloadEffect("sounds/error.wav");
                SimpleAudioEngine::getInstance()->playEffect("sounds/error.wav", false, 1.0, 0.0, notevol / 100.0);
            }
        }
    }
    keylabel4->setString(StringUtils::format("%c", track4key));
}

void SelectAudio::menuSetdelayCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);

    auto scene = SetDelay::createScene();
    auto transition = TransitionFade::create(0.5f, scene);
    SimpleAudioEngine::getInstance()->setEffectsVolume(notevol / 100.0f);
    Director::getInstance()->pushScene(transition);
}
#include "LaunchPage.h"
#include "SelectSong.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

cocos2d::MenuItemImage* nextButton;

Scene* LaunchPage::createScene()
{
    return LaunchPage::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}


bool LaunchPage::init()
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

    

    auto cocos2dxLogo = Sprite::create("backanddecos/LaunchPage.png");
    if (cocos2dxLogo == nullptr)
    {
        problemLoading("'backanddecos/LaunchPage.png'");
    }
    else
    {
        cocos2dxLogo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));
        cocos2dxLogo->setOpacity(0);
        backLayer->addChild(cocos2dxLogo);
    }

    auto nameAndAuthor = Sprite::create("backanddecos/NameAndAuthor.png");
    if (nameAndAuthor == nullptr)
    {
        problemLoading("'backanddecos/NameAndAuthor.png'");
    }
    else
    {
        nameAndAuthor->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        nameAndAuthor->setOpacity(0);
        backLayer->addChild(nameAndAuthor);
    }

    auto poweredbyText = Label::createWithTTF("Powered by", "fonts/Genshin.ttf", 35);
    if (poweredbyText == nullptr)
    {
        problemLoading("'fonts/Genshin.ttf'");
    }
    else
    {
        poweredbyText->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2 + cocos2dxLogo->getContentSize().height / 2));

        poweredbyText->setOpacity(0);
        backLayer->addChild(poweredbyText);
    }

    auto clicktostartText = Label::createWithTTF("C L I C K   T O   S T A R T", "fonts/Arial.ttf", 45);
    if (clicktostartText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        clicktostartText->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2 - clicktostartText->getContentSize().height * 2));

        clicktostartText->setOpacity(0);
        backLayer->addChild(clicktostartText, 1);
    }

    auto tracbomLogo = Label::createWithTTF("tracbom", "fonts/Genshin.ttf", 170);
    if (tracbomLogo == nullptr)
    {
        problemLoading("'fonts/Genshin.ttf'");
    }
    else
    {
        tracbomLogo->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2 + tracbomLogo->getContentSize().height / 4 - 5));

        tracbomLogo->setOpacity(0);
        backLayer->addChild(tracbomLogo, 1);
    }
    
    nextButton = MenuItemImage::create(
        "backanddecos/start.png",
        "backanddecos/start.png",
        CC_CALLBACK_1(LaunchPage::menuNextCallback, this));

    if (nextButton == nullptr ||
        nextButton->getContentSize().width <= 0 ||
        nextButton->getContentSize().height <= 0)
    {
        problemLoading("'backanddecos/start.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2;
        nextButton->setPosition(Vec2(x, y));
        nextButton->setName("next");
    }
    auto menu = Menu::create(nextButton, NULL);
    menu->setPosition(Vec2::ZERO);

    menu->setOpacity(0);
    backLayer->addChild(menu);


    auto skipText = Label::createWithTTF("- Click to skip -", "fonts/Genshin.ttf", 20);
    if (skipText == nullptr)
    {
        problemLoading("'fonts/Genshin.ttf'");
    }
    else
    {
        skipText->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + skipText->getContentSize().height * 2));

        skipText->setOpacity(0);
        backLayer->addChild(skipText);
    }


    auto skipdelay1 = DelayTime::create(2.7f);
    auto skipfadeIn = FadeIn::create(1.0f);
    auto skipdelay2 = DelayTime::create(8.8f);
    auto skipfadeOut = FadeOut::create(0.5f);
    auto skipseq = Sequence::create(skipdelay1, skipfadeIn, skipdelay2, skipfadeOut, nullptr);
    skipText->runAction(Repeat::create(skipseq, 1));

    auto fadeIn1 = FadeIn::create(0.3f);
    auto fadeIn2 = FadeIn::create(0.3f);
    auto fadeOut1 = FadeOut::create(1.0f);
    auto fadeOut2 = FadeOut::create(1.0f);
    auto delay11 = DelayTime::create(0.6f);
    auto delay12 = DelayTime::create(3.6f);
    auto seq1 = Sequence::create(delay11, fadeIn1, delay12, fadeOut1, nullptr);
    auto delay21 = DelayTime::create(0.6f);
    auto delay22 = DelayTime::create(3.6f);
    auto seq2 = Sequence::create(delay21, fadeIn2, delay22, fadeOut2, nullptr);

    poweredbyText->runAction(Repeat::create(seq1, 1));

    cocos2dxLogo->runAction(Repeat::create(seq2, 1));

    auto delay31 = DelayTime::create(6.5f);
    auto fadeIn3 = FadeIn::create(2.5f);
    auto delay32 = DelayTime::create(2.0f);
    auto fadeOut3 = FadeOut::create(0.8f);
    auto seq3 = Sequence::create(delay31, fadeIn3, delay32, fadeOut3, nullptr);
    nameAndAuthor->runAction(Repeat::create(seq3, 1));

    auto delay4 = DelayTime::create(13.0f);
    auto fadeIn4 = FadeIn::create(1.4f);
    auto seq4 = Sequence::create(delay4, fadeIn4, nullptr);
    auto delay5 = DelayTime::create(13.0f);
    auto fadeIn5 = FadeIn::create(1.4f);
    auto seq5 = Sequence::create(delay5, fadeIn5, nullptr);
    auto delay6 = DelayTime::create(13.0f);
    auto fadeIn6 = FadeIn::create(1.4f);
    auto seq6 = Sequence::create(delay6, fadeIn6, nullptr);

    menu->runAction(Repeat::create(seq4, 1));
    clicktostartText->runAction(Repeat::create(seq5, 1));
    tracbomLogo->runAction(Repeat::create(seq6, 1));
    
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/launch.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/launch.mp3", false);    

    return true;
}


void LaunchPage::menuNextCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false);
    nextButton->setEnabled(false);
    Sleep(500);
    auto scene = SelectSong::createScene();
    auto scenetrans = TransitionFade::create(0.5f, scene);
	Director::getInstance()->replaceScene(scenetrans);
}

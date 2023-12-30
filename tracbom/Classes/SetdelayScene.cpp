
#include "SetdelayScene.h"
#include "SimpleAudioEngine.h"
#include "Track.h"
#include "NoteTap.h"
#include "ui/CocosGUI.h"
#include <fstream>



USING_NS_CC;
using namespace cocos2d::ui;
using namespace CocosDenshion;


extern int bgmvol;
extern int notevol;

extern float delaytime;
extern float Time;
float judtime = 1.0f;

bool first = 1;

//extern short track1key;
//extern int Note_strack[4];//轨道



Scene* SetDelay::createScene()
{
	return SetDelay::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SetDelay::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Time=-3.0f;
	judtime = 1.0f;

	first = 1;

	//预加载音乐和音效
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/setdelay4.mp3");
	audio->preloadEffect("sounds/tapEffect1.mp3");
	audio->setEffectsVolume(notevol / 100.0f);

	auto labTime = Label::createWithTTF("", "fonts/com.otf", 24);
	labTime->setPosition(Vec2(100, 100));
	this->addChild(labTime, 4);

	schedule([labTime, audio](float dt)->void {

		if (Time >= 0 && first) {
			log("play");
			audio->playBackgroundMusic("sounds/setdelay4.mp3");
			audio->setBackgroundMusicVolume(bgmvol / 100.0f);
			first = 0;
		}
		}, "PlayMusic");



	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto Backlayer = LayerColor::create(Color4B::BLACK);

	auto BackGround = Sprite::create("picture/background.png");
	BackGround->setScale(visibleSize.width / BackGround->getContentSize().width);
	BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	BackGround->setOpacity(50);
	Backlayer->addChild(BackGround);
	this->addChild(Backlayer, 0);


	auto track = Track::create("track");
	track->opacity = 255;
	track->bodyPic->setOpacity(track->opacity);
	track->headPic->setOpacity(track->opacity);
	track->bodyPic2->setOpacity(track->opacity);
	track->setAnchorPoint(Vec2(0.5f, 0));
	track->setPosition(Vec2(visibleSize.width / 2 + origin.x + 600, 200 + origin.y));
	track->setContentSize(Size(track->width, track->width * 5));
	this->addChild(track, 1, "track");
	
	std::string str = StringUtils::format("%dms", int(delaytime * 1000));
	auto delayLab = Label::createWithTTF(str, "fonts/combo.otf", 24);

	auto slider = Slider::create();
	slider->loadBarTexture("buttonanddiv/emptybar.png");
	slider->loadSlidBallTextures("buttonanddiv/slideblock.png", "buttonanddiv/slideblockselected.png", "buttonanddiv/slideblockselected.png");
	slider->loadProgressBarTexture("buttonanddiv/selectedbar.png");
	slider->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 2));
	slider->setPercent(delaytime * 100 + 50);
	slider->addEventListener([delayLab, slider](Ref* sender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			delaytime = (slider->getPercent() - 50) / 100.0f;

			//log("%fms", delaytime * 1000);
			delayLab->setString(StringUtils::format("%dms", int(delaytime * 1000)));
		}
		});
	slider->addChild(delayLab);
	delayLab->setPosition(Vec2(slider->getContentSize().width / 9 * 10, slider->getContentSize().height / 2));

	this->addChild(slider);


	Time = -3.0f;
	//生成音符
	schedule([this, track, audio](float dt)->void {
		//[0][0]为轨道号
		//[0][1]为生成时间到判定时间间隔
		//[0][2]为判定时间
		Time += dt;
		//log("Time: %f", Time);

		while (1.0f + Time >= judtime + delaytime) {
			auto tap = NoteTap::create(track, judtime + delaytime);
			//log("create!!!");
			track->addChild(tap);
			tap->setAnchorPoint(Vec2(0.5f, 0.5f));
			tap->trackNum = 1;
			judtime += 2.0f;
			//log("juidTime: %f", judtime);
		}
		
			

		}, "create");

	
	schedule([this,audio](float dt)->void {

		if (Time >= judtime + delaytime-2 && Time < judtime + delaytime + dt -2&&Time>0) {
			audio->playEffect("sounds/tapEffect1.mp3");
		}

	}, "Effect");
	

	// 键盘监听
	auto listener = EventListenerKeyboard::create();
	//按下按键时


	

	listener->onKeyPressed = [this, track, visibleSize, origin](EventKeyboard::KeyCode keyCode, Event* event) ->void {
		log("Pressed");
		auto line =Sprite::create("picture/line.png");
		log("Creat");
		this->addChild(line, 10, "line");
		line->setAnchorPoint(Vec2(0.5f, 0.5f));
		line->setPosition(Vec2(visibleSize.width / 2 + origin.x + 600, 200+2300*(judtime + delaytime -Time-2)));
		log("judtime-Time=%f", judtime - Time);
		auto fadeOut = FadeOut::create(1.5f); // 创建一个淡出动作，duration 是淡出效果持续的时间
		line->runAction(Sequence::create(fadeOut, /*CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, line)), */nullptr));// 让精灵执行淡出动作
		log("run");
		
		
		////log("key %d pressed", keyCode);
		//if (GetAsyncKeyState(track1key) & 0x8000 && Note_strack[0] != -1) {
		//	
		//	Note_strack[0] = 1;
		//	log("trac1Key，Note_strack[0]=%d", Note_strack[0]);
		//	//LightingL1->setOpacity(0);
		//}
		


	};
	//松开按键时
	listener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event)->void {
		////恢复按键锁定
		//if (!(GetAsyncKeyState(track1key) & 0x8000)) {
		//	Note_strack[0] = 0;

		//}

	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto backbutton = MenuItemImage::create(
		"buttonanddiv/exitbutton.png",
		"buttonanddiv/exitbuttonpressed.png", CC_CALLBACK_1(SetDelay::menuCloseCallback,this));


	backbutton->setScale(0.5f);
	backbutton->setAnchorPoint(Vec2(0, 0));
	backbutton->setPosition(Vec2(origin.x - 5, origin.y + visibleSize.height - backbutton->getContentSize().height / 1.6));
	auto menu = Menu::create(backbutton,NULL);

	menu->setPosition(Vec2(0, 0));
	menu->setOpacity(150);

	this->addChild(menu);

	auto backtext = Label::createWithTTF("BACK","fonts/genshin.ttf",40);
	backtext->setPosition(Vec2(origin.x + backtext->getContentSize().width / 1.2 - 3, origin.y + visibleSize.height - backtext->getContentSize().height * 2));
	this->addChild(backtext, 2);

	
	return true;
}




void SetDelay::menuCloseCallback(Ref* pSender)
{
	std::ofstream outputFile("delaysetting.ini");
	// 检查文件是否成功打开
	if (outputFile.is_open()) {

	}
	else {
		log("Error opening file");
		return ;
	}

	outputFile << delaytime;
	outputFile.close();

	Director::getInstance()->popScene();
	auto audio = SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/selectsongbackgroundnew.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0f);
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

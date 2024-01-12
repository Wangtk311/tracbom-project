
#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "Track.h"
#include "NoteTap.h"
#include "NoteHold.h"
//#include "Settlement.h"
//#include "SelectSong.h"
#include <iostream>
#include<fstream>
#include <sstream>
#include "winuser.h"
#include "Settlement.h"
#include "ui/CocosGUI.h"

using namespace CocosDenshion;
USING_NS_CC;

extern std::string MusicFilename;//歌曲名字
extern std::string SkinFilename ;//皮肤名字
extern std::string Diff ;//难度
extern short track1key ;
extern short track2key ;
extern short track3key ;
extern short track4key ;

extern float Score;//分数
extern int combo;//连击数
extern int pNum;//perfect数
extern int gNum;//good数
extern int bNum;//bad数
extern int mNum;//miss数

extern float delaytime;//延迟时间


extern int bgmvol;
extern int notevol;


float Time = -15.0f;//时间;
int Note_strack[4] = { 0 };//轨道
float perfectScore ;//perfect分数
float goodScore;//goodt分数

bool ifPause=0;//是否暂停了
bool firstPlay = 1;
int combonow = 0;

Vector<SpriteFrame*> animFrames;


MenuItemImage* retry;
MenuItemImage* cont;
MenuItemImage* back;




Scene* Play::createScene()
{
	return Play::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Play::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	Time = -3.0f;//时间;
	Note_strack[4] = { 0 };//轨道
	ifPause = 0;
	firstPlay = 1;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	std::string effectFilename = "sounds/tapEffect.mp3";
	std::string musicFilename = "Music/" + MusicFilename + "/" + MusicFilename + ".mp3";
	
	//预加载音乐和音效
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(musicFilename.c_str());
	//audio->setEffectsVolume(notevol / 100.0f);
	audio->preloadEffect(effectFilename.c_str());
	
	//audio->preloadBackgroundMusic(effectFilename.c_str());
	//audio->setBackgroundMusicVolume(0.0f);
	//audio->playBackgroundMusic(musicFilename.c_str());
	//audio->setBackgroundMusicVolume(0.0f);

	combo = 0;//连击数初始化
	combonow = 0;//连击数初始化
	Score = 0;//分数初始化
	pNum = 0;//perfect数初始化
	gNum = 0;//good数初始化
	bNum = 0;//bad数初始化
	mNum = 0;//miss数初始化


	

	Vector<SpriteFrame*> Tostarts(3);
	Tostarts.pushBack(SpriteFrame::create("picture/3.png", Rect(0, 0, 244, 244)));
	Tostarts.pushBack(SpriteFrame::create("picture/2.png", Rect(0, 0, 244, 244)));
	Tostarts.pushBack(SpriteFrame::create("picture/1.png", Rect(0, 0, 244, 244)));
	Animation* Tostart = Animation::createWithSpriteFrames(Tostarts, 1.0f);

	auto Tsani = Animate::create(Tostart);

	auto ts = Sprite::create();
	ts->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(ts, 1);
	ts->runAction(Sequence::create(Tsani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, ts)), NULL));

	



	/*auto labTime = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	labTime->setPosition(Vec2(100, 100));
	this->addChild(labTime, 1);*/
	auto lab = Label::createWithTTF("Press ESC to pause", "fonts/Deng.ttf", 25);
	lab->setAnchorPoint(Vec2(0, 1));
	lab->setPosition(Vec2(20+origin.x, visibleSize.height + origin.y-40));
	this->addChild(lab, 1);

	/*schedule([labTime, audio, musicFilename](float dt)->void {
		if (Time > -2.0 && firstStopPre) {
			audio->stopBackgroundMusic();
			firstStopPre = 0;
		}	
		}, "StopMusicforPre");*/
	//播放音乐
	schedule([ audio, musicFilename](float dt)->void {		
		if (Time >= 0&& firstPlay) {
			log("play");
			audio->playBackgroundMusic(musicFilename.c_str());
			audio->setBackgroundMusicVolume(bgmvol/100.0f);
			firstPlay = 0;
		}
		
		/*
			std::ostringstream ss;
			ss << Time;
			std::string s = ss.str();
			labTime->setString(s);
		*/
		}, "PlayMusic");





	this->scheduleUpdate();//更新时间


	//Background层
	auto Backlayer = LayerColor::create(Color4B::BLACK);
	std::string PicFilename = "Music/" + MusicFilename + "/" + MusicFilename + ".png";
	auto BackGround = Sprite::create(PicFilename);
	BackGround->setScale(visibleSize.width / BackGround->getContentSize().width);
	BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	BackGround->setOpacity(50);
	Backlayer->addChild(BackGround);
	this->addChild(Backlayer, 0);




	auto track1 = Track::create("track1");
	track1->setAnchorPoint(Vec2(0.5f, 0));
	track1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 600, 200 + origin.y));
	track1->setContentSize(Size(track1->width, track1->width * 5));
	this->addChild(track1, 1, "track1");


	auto track2 = Track::create("track2");
	track2->setAnchorPoint(Vec2(0.5f, 0));
	track2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 200, 200 + origin.y));
	track2->setContentSize(Size(track2->width, track2->width * 5));
	this->addChild(track2, 1, "track2");


	auto track3 = Track::create("track3");
	track3->setAnchorPoint(Vec2(0.5f, 0));
	track3->setPosition(Vec2(visibleSize.width / 2 + origin.x + 200, 200 + origin.y));
	track3->setContentSize(Size(track3->width, track3->width * 5));
	this->addChild(track3, 1, "track3");


	auto track4 = Track::create("track4");
	track4->setAnchorPoint(Vec2(0.5f, 0));
	track4->setPosition(Vec2(visibleSize.width / 2 + origin.x + 600, 200 + origin.y));
	track4->setContentSize(Size(track4->width, track4->width * 5));
	this->addChild(track4, 1, "track4");



	char sco[10];
	sprintf(sco, "%07d", Score);
	//显示分数

	char com[10];
	sprintf(com, "%d", combonow);
	//显示combo

	auto scoreLab = Label::createWithTTF(sco, "fonts/score.otf", 50);
	this->addChild(scoreLab, 1);
	scoreLab->setPosition(Vec2(visibleSize.width + origin.x - 180, visibleSize.height + origin.y - 70));

	auto comboLab = Label::createWithTTF("", "fonts/combo.otf", 70);
	this->addChild(comboLab, 1);
	comboLab->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y - 75));

	auto comLab = Label::createWithTTF("COMBO", "fonts/com.otf", 30);
	this->addChild(comLab, 1);
	comLab->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height + origin.y - 130));

	schedule([scoreLab, comboLab](float dt)->void {
		char sco[15];
		if (Score >= 999990)
			Score = 1000000;
		sprintf(sco, "%07d", int(Score));
		scoreLab->setString(sco);
		char com[10];
		sprintf(com, "%d", combonow);
		comboLab->setString(com);
		if (combonow > 0) {
			comboLab->setVisible(true);

		}
		else {
			comboLab->setVisible(false);

		}
		}, "ScoreUpdate"
	);



	


	//歌曲信息显示
	std::string musicnamepath = "Music/" + MusicFilename + "/" + MusicFilename + ".txt";
	//std::string musicname = FileUtils::getInstance()->getStringFromFile(musicnamepath);
	std::string musicname;
	

	std::ifstream file(musicnamepath); // 替换为你的文件路径
	float time = 0.0f;
	if (file.is_open()) {
		std::string line;
		std::string timeStr;
		

		while (getline(file, line)) {
			std::stringstream ss(line);
			std::string key;
			ss >> key;

			if (key == "Name:") {
				std::getline(ss, musicname);
				
			}
			else if (key == "Time:") {
				ss >> timeStr; // 读取并提取Time后面的内容
				time = stof(timeStr);// 将时间字符串转换为浮点数
			}
		}

		file.close();
	}

	auto musicName = Label::createWithTTF(musicname, "fonts/Deng.ttf", 30);
	musicName->setAnchorPoint(Vec2(0, 0));
	musicName->setPosition(Vec2(20 + origin.x, 20 + origin.y));
	this->addChild(musicName, 1);

	auto diffLab = Label::createWithTTF(Diff, "fonts/Deng.ttf", 30);
	diffLab->setAnchorPoint(Vec2(1, 0));
	diffLab->setPosition(Vec2(visibleSize.width-20 + origin.x, 20 + origin.y));
	this->addChild(diffLab, 1);

	//歌曲进度条
	float musicTime = 0;
	FileUtils::getInstance()->getStringFromFile("Music/" + MusicFilename + "/" + MusicFilename + ".txt");
	auto progress = LoadingBar::create("picture/progress.png");
	progress->setDirection(LoadingBar::Direction::LEFT);
	progress->setAnchorPoint(Vec2(0, 0.5f));
	progress->setScaleX(visibleSize.width / progress->getContentSize().width);
	progress->setPosition(Vec2(origin.x, visibleSize.height + origin.y - 3));
	this->addChild(progress, 1);

	schedule([progress,time](float dt)->void {
		progress->setPercent(Time / time * 100);
		}, "progress");



	GamePre();//铺面读取




	//生成音符
	schedule([this, track1, track2, track3, track4](float dt)->void {


		if (!ifPause) {
			//[0][0]为轨道号
			//[0][1]为生成时间到判定时间间隔
			//[0][2]为判定时间
			while (!nTap.empty() && nTap[0][1] + Time >= nTap[0][2]) {

				int trackNum = nTap[0][0];

				if (trackNum == 1) {
					auto tap = NoteTap::create(track1, nTap[0][2]+delaytime);

					track1->addChild(tap);
					tap->setAnchorPoint(Vec2(0.5f, 0.5f));
					tap->trackNum = trackNum;
				}
				else if (trackNum == 2) {
					auto tap = NoteTap::create(track2, nTap[0][2] + delaytime);

					track2->addChild(tap);
					tap->setAnchorPoint(Vec2(0.5f, 0.5f));
					tap->trackNum = trackNum;
				}
				else if (trackNum == 3) {
					auto tap = NoteTap::create(track3, nTap[0][2] + delaytime);

					track3->addChild(tap);
					tap->setAnchorPoint(Vec2(0.5f, 0.5f));
					tap->trackNum = trackNum;
				}
				else if (trackNum == 4) {
					auto tap = NoteTap::create(track4, nTap[0][2] + delaytime);

					track4->addChild(tap);

					tap->setAnchorPoint(Vec2(0.5f, 0.5f));
					tap->trackNum = trackNum;
				}
				else
					log("error");
				nTap.erase(nTap.begin());
			}
			while (!nHold.empty() && nHold[0][1] + Time >= nHold[0][2]) {

				int trackNum = nHold[0][0];
				//log("trackNum=%d", trackNum);			
				if (trackNum == 1) {
					auto hold = NoteHold::create(track1, nHold[0][2] + delaytime, nHold[0][3] + delaytime);
					track1->addChild(hold);
					hold->trackNum = trackNum;
					//hold->setPosition(Vec2(track1->width, track1->speed * (nHold[0][1])));
				}
				else if (trackNum == 2) {
					auto hold = NoteHold::create(track2, nHold[0][2] + delaytime, nHold[0][3] + delaytime);
					track2->addChild(hold);
					//hold->setAnchorPoint(Vec2(0.5f, 0.5f));
					hold->trackNum = trackNum;
					//log("trackNum=%d", trackNum);
					//log("trackNum=%d", hold->trackNum);
					//hold->setPosition(Vec2(track2->width, track2->speed * (nHold[0][1])));
				}
				else if (trackNum == 3) {
					auto hold = NoteHold::create(track3, nHold[0][2] + delaytime, nHold[0][3] + delaytime);
					track3->addChild(hold,5);
					hold->trackNum = trackNum;
					//hold->setPosition(Vec2(track3->width, track3->speed * (nHold[0][1])));
				}
				else if (trackNum == 4) {
					auto hold = NoteHold::create(track4, nHold[0][2] + delaytime, nHold[0][3] + delaytime);
					track4->addChild(hold);
					hold->trackNum = trackNum;
					//hold->setPosition(Vec2(track4->width, track4->speed * (nHold[0][1])));
				}
				else
					log("error");
				nHold.erase(nHold.begin());
			}
		}
		}, "CreateNote"
	);

	/*scheduleOnce([audio, musicFilename](float dt)->void {
		if (Time >= 0) {
			log("play");
			audio->playBackgroundMusic(musicFilename.c_str(), true);
		}

		*/

	//测试用
	/*auto LightingL1 = Sprite::create("CloseNormal.png");
	LightingL1->setAnchorPoint(Vec2(0, 0));
	LightingL1->setPosition(Vec2(visibleSize.width / 2 - 303, 0));
	LightingL1->setOpacity(255);
	this->addChild(LightingL1, 4);*/

	retry = MenuItemImage::create("picture/retry.png", "picture/retryselected.png",
		CC_CALLBACK_0(Play::reTry, this));
	retry->setName("retry");
	back = MenuItemImage::create("picture/back.png", "picture/backselected.png",
		CC_CALLBACK_0(Play::Back, this));
	back->setName("back");
	cont = MenuItemImage::create("picture/continue.png", "picture/continueselected.png",
		CC_CALLBACK_0(Play::conTinue, this));
	cont->setName("cont");


	/*retry->addChild(back, 1, "back");
	retry->addChild(cont, 1, "cont");
	back->setAnchorPoint(Vec2(1, 0));
	cont->setAnchorPoint(Vec2(0, 0));
	back->setPosition(Vec2(-retry->getContentSize().width / 2, 0));
	cont->setPosition(Vec2(retry->getContentSize().width / 2 * 3, 0));*/
	retry->setScale(0.2f);
	back->setScale(0.2f);
	cont->setScale(0.2f);
	//retry->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//retry->addChild(pauseLayer, 0);
	
	auto pauseMenu = Menu::create(back, retry,cont, NULL);
	pauseMenu->alignItemsHorizontallyWithPadding(retry->getContentSize().width/5);
	auto pauseLayer = LayerColor::create(Color4B::BLACK);
	pauseLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	pauseLayer->setContentSize(visibleSize);

	pauseLayer->addChild(pauseMenu,1,"pauseMneu");
	this->addChild(pauseLayer, 2, "pauseLayer");

	//pauseMenu->setContentSize(visibleSize);
	pauseLayer->setOpacity(0);
	pauseMenu->setOpacity(0);
	retry->setEnabled(false);
	back->setEnabled(false);
	cont->setEnabled(false);


	// 键盘监听
	auto listener = EventListenerKeyboard::create();
	//按下按键时
	
	listener->onKeyPressed =[](EventKeyboard::KeyCode keyCode, Event* event) ->void{
		/*因为EventKeyboard::KeyCode::KEY每帧只能检测一个按键，而音游对按键时机的要求较高
		比如，双押的同时按下
		所以就用GetAsyncKeyState()作为按下时处理*/
		//log("key %d pressed", keyCode);
		if (!ifPause) {
			if (GetAsyncKeyState(track1key) & 0x8000 && Note_strack[0] != -1) {
				Note_strack[0] = 1;
				log("trac1Key，Note_strack[0]=%d", Note_strack[0]);
				//LightingL1->setOpacity(0);
			}
			if (GetAsyncKeyState(track2key) & 0x8000 && Note_strack[1] != -1) {
				Note_strack[1] = 1;
				log("trac2Key，Note_strack[1]=%d", Note_strack[1]);
				//LightingL2->setOpacity(255);
			}
			if (GetAsyncKeyState(track3key) & 0x8000 && Note_strack[2] != -1) {
				Note_strack[2] = 1;
				log("trac3Key，Note_strack[2]=%d", Note_strack[2]);
				//LightingR2->setOpacity(255);
			}
			if (GetAsyncKeyState(track4key) & 0x8000 && Note_strack[3] != -1) {
				Note_strack[3] = 1;
				log("trac4Key，Note_strack[3]=%d", Note_strack[3]);
				//LightingR1->setOpacity(255);
			}
		}
	};
	//松开按键时
	listener->onKeyReleased =[pauseLayer, pauseMenu](EventKeyboard::KeyCode keyCode, Event* event)->void {
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
			if (!ifPause) {
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				ifPause = 1;
				pauseLayer->setOpacity(100);
				pauseMenu->setOpacity(255);
				retry->setEnabled(true);
				back->setEnabled(true);
				cont->setEnabled(true);


			}
		}
		/*if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && Play_End){
			
		}*/
		//恢复按键锁定
		if (!(GetAsyncKeyState(track1key) & 0x8000)){
			Note_strack[0] = 0;
			//灯光熄火
			//LightingL1->setOpacity(0);
		}
		if (!(GetAsyncKeyState(track2key) & 0x8000)){
			Note_strack[1] = 0;
			//LightingL2->setOpacity(0);
		}
		if (!(GetAsyncKeyState(track3key) & 0x8000)){
			Note_strack[2] = 0;
			//LightingR2->setOpacity(0);
		}
		if (!(GetAsyncKeyState(track4key) & 0x8000)){
			Note_strack[3] = 0;
			//LightingR1->setOpacity(0);
		}
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	/*auto  pauseLayer=this->getChildByName("pauseLayer");
	auto  retry = pauseLayer->getChildByName("retry");
	auto  back = retry->getChildByName("back");
	auto  cont = retry->getChildByName("cont");

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [retry, back, cont](Touch* t, Event* e)->bool {
		auto nPos = retry->convertTouchToNodeSpace(t);
		if (retry->getBoundingBox().containsPoint(nPos)) {
			return true;
		}
		else {
			auto nPos = retry->convertTouchToNodeSpace(t);
			if (back->getBoundingBox().containsPoint(nPos)) {}
		}
			return false;
		};

	touchListener->onTouchMoved = [](Touch* t, Event* e)->bool {
		};
	touchListener->onTouchEnded = [](Touch* t, Event* e)->bool {
		};*/

	//打击特效

	animFrames.clear();
	for (int i = 0; i <= 63; i++) {
		std::string skinFilename = "skin/" + SkinFilename + "/hit/" + std::to_string(i) + ".png";
		log("%s", skinFilename.c_str());

		animFrames.pushBack(SpriteFrame::create(skinFilename, Rect(0, 0, 1247, 1247)));
	}
	Animation* hitAnimation = Animation::createWithSpriteFrames(animFrames, 0.008f);
	auto hitAni = Animate::create(hitAnimation);

	auto hit = Sprite::create();
	hit->setScale(0.3f);
	hit->setOpacity(0);
	this->addChild(hit,-1);
	hit->runAction(Sequence::create(hitAni, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, hit)), NULL));


	/*animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/0.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/1.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/2.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/3.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/4.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/5.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/6.png", Rect(0, 0, 50, 50)));
	animFrames.pushBack(SpriteFrame::create("skin/skin_name/hit/7.png", Rect(0, 0, 50, 50)));*/

	// create the animation out of the frames
	




	schedule([this](float dt)->void {
		bool isPlaying = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
		if (!isPlaying&&Time>10&&!ifPause) {
			
			if (combonow > combo)
				combo = combonow;
			auto settlement = Settlement::createScene();
			//渐入
			auto transition = TransitionFade::create(1.0f, settlement);
			Director::getInstance()->replaceScene(transition);
			
			
			//auto settlement = Settlement::createScene();
			//auto transition = TransitionFade::create(1.0f, settlement);
			//Director::getInstance()->replaceScene(transition);
			// 在这里可以执行相应的操作
		}
		}, "MusicEnd");

	


	return true;
}

void Play::update(float dt) {
	if(!ifPause)
		Time += dt;
	
	//log("Time: %f" , Time); 
}

void Play::GamePre() {

	int noteTotal = 0;
	//log("GamePre in");
	std::string filename = "Music/" + MusicFilename + "/" + Diff + "/note.txt";
	std::string content = FileUtils::getInstance()->getStringFromFile(filename);
	if (content.empty()) {
		CCLOG("Failed to open file: %s", filename.c_str());
	}

	std::stringstream ss(content);
	std::string line;
	while (std::getline(ss, line)) {
		std::vector<float>data(4);
		std::istringstream iss(line);

		std::string token;
		int setType;
		std::getline(iss, token, ' ');
		setType = std::stof(token);
		//log("GamePre%d", setType);
		for (int i = 0; i < 4; i++) {
			std::getline(iss, token, ' ');

			data[i] = std::stof(token);
		}
		if (setType == 1) {
			nTap.push_back(data);
			
			++noteTotal;
		}
		else if (setType == 2) {
			nHold.push_back(data);
			//log("data:%f %f %f %f", nHold[0][0], nHold[0][1], nHold[0][2], nHold[0][3]);
			++noteTotal;
		}
		else
			log("error");
		
	}


	perfectScore=1000000.0/noteTotal;
	goodScore=perfectScore*0.75;

}

void Play::reTry() {
	
	//Director::getInstance()->popScene();
	auto playSce = Play::createScene();
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(playSce);
	//log("Time: %f" , Time); 
}

void Play::conTinue() {
	if (ifPause) {
		auto pauseLayer= this->getChildByName("pauseLayer");
		auto pauseMenu = pauseLayer->getChildByName("pauseMneu");


		pauseLayer->setOpacity(0);
		pauseMenu->setOpacity(0);
		retry->setEnabled(false);
		back->setEnabled(false);
		cont->setEnabled(false);

		
		auto visibleSize = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();

		Vector<SpriteFrame*> pauseTostarts(3);
		pauseTostarts.pushBack(SpriteFrame::create("picture/3.png", Rect(0, 0, 244, 244)));
		pauseTostarts.pushBack(SpriteFrame::create("picture/2.png", Rect(0, 0, 244, 244)));
		pauseTostarts.pushBack(SpriteFrame::create("picture/1.png", Rect(0, 0, 244, 244)));
		Animation* pauseTostart = Animation::createWithSpriteFrames(pauseTostarts, 1.0f);

		auto pTsani = Animate::create(pauseTostart);

		auto pts = Sprite::create();
		pts->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(pts, 1);
		pts->runAction(Sequence::create(pTsani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, pts)), NULL));



		schedule([](float dt)->void {
			static float pauseTime = 0;			
				pauseTime += dt;
			if (pauseTime >= 3) {
				ifPause = 0;
				pauseTime = 0;
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
			
			}, "pauseTime"
		);
		schedule([=](float dt)->void {
			if (!ifPause)
				unschedule("pauseTime");
			}, "pauseFlag"
		);
		
	}
}

void Play::Back()
{
	Director::getInstance()->popScene();
	//Close the cocos2d-x game scene and quit the application

	//Director::getInstance()->popScene();
	//auto selectSong = SelectSong::createScene();
	//Director::getInstance()->replaceScene(selectSong);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/selectsongbackgroundnew.mp3",true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0f);

}

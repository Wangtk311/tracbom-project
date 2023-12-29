#include "SelectSong.h"
#include "About.h"
#include "SelectAudio.h"
#include "Settlement.h"
#include "PlayScene.h"
#include "Howtoplay.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <vector>

//#define _BETA

using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

extern int bgmvol;
extern int notevol;
extern short track1key;
extern short track2key;
extern short track3key;
extern short track4key;
extern float delaytime;
extern std::string MusicFilename;
extern std::string Diff;

int scores[3] = { 0, 0, 0 };
int scorenum = 0;
char hscore[10] = "0000000";
vector<string> musicLib;
vector<string> musicLibChinese;
Texture2D* musicpic_texture;
Sprite* musicPicture;
int musicNum = 0;
int selectedmusicindex = 0;
cocos2d::MenuItemImage* exitButton;

Scene* SelectSong::createScene()
{
    return SelectSong::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}


bool SelectSong::init()
{
    musicLib.erase(musicLib.begin(), musicLib.end());
    musicLibChinese.erase(musicLibChinese.begin(), musicLibChinese.end());

    ifstream musiclistFile;
    musiclistFile.open("musiclist.txt", ios::_Nocreate);
    if (!musiclistFile.is_open())
    {
		problemLoading("'musiclist.txt'");
	}
	else
	{
		string tempMusicname;
		while (getline(musiclistFile, tempMusicname))
		{
			musicLib.push_back(tempMusicname);
		}
		musiclistFile.close();
	}
    static int flag = 0;
    if (flag == 0) {
        MusicFilename = musicLib[0];
        Diff = "EZ";
        flag = 1;
        selectedmusicindex = 0;
    }
    

    for (size_t i = 0; i < musicLib.size(); i++)
    {
		string tempMusicnamePath = "Music/" + musicLib[i] + "/" + musicLib[i] + ".txt";
        std::string tempMusicnameChinese;
        ifstream file(tempMusicnamePath);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string key;
                ss >> key;

                if (key == "Name:") {
                    getline(ss, tempMusicnameChinese);
                    if (tempMusicnameChinese.size() >= 37) {
                        tempMusicnameChinese = tempMusicnameChinese.substr(0, 37);
                        tempMusicnameChinese += "...";
                    }
                }

            }
            file.close();
        }
		musicLibChinese.push_back(tempMusicnameChinese);
	}
    
    musicNum = musicLib.size();

    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto backLayer = Layer::create();
    this->addChild(backLayer);

    backLayer->setContentSize(visibleSize);

    ifstream soundsettingsFile;
    soundsettingsFile.open("soundsettings.ini", ios::_Nocreate);
    if (!soundsettingsFile.is_open())
    {
		problemLoading("'soundsettings.ini'");
	}
    else
    {
		soundsettingsFile >> bgmvol >> notevol;
		soundsettingsFile.close();
    }

    ifstream keysettingsFile;
    keysettingsFile.open("keysettings.ini", ios::_Nocreate);
    if (!keysettingsFile.is_open())
    {
        problemLoading("'keysettings.ini'");
    }
    else
    {
		int track1key_read = 0x00, track2key_read = 0x00, track3key_read = 0x00, track4key_read = 0x00;
		keysettingsFile >> track1key_read >> track2key_read >> track3key_read >> track4key_read;
		track1key = track1key_read;
		track2key = track2key_read;
		track3key = track3key_read;
		track4key = track4key_read;
		keysettingsFile.close();
	}

    fstream highscoreFile;
    string highscorePath = "musicscore/" + MusicFilename + ".txt";
    highscoreFile.open(highscorePath, ios::_Nocreate);
    if (!highscoreFile.is_open())
    {
		highscoreFile.open(highscorePath, ios::out);
        highscoreFile << "0 0 0";
        highscoreFile.close();
        highscoreFile.open(highscorePath, ios::_Nocreate);
        highscoreFile >> scores[0] >> scores[1] >> scores[2];
        highscoreFile.close();
	}
	else
	{
		highscoreFile >> scores[0] >> scores[1] >> scores[2];
		highscoreFile.close();
	}

    ifstream delaysettingFile;
    delaysettingFile.open("delaysetting.ini", ios::_Nocreate);
    if (!delaysettingFile.is_open())
    {
		problemLoading("'delaysetting.ini'");
	}
    else
    {
        delaysettingFile >> delaytime;
		delaysettingFile.close();
	}

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

    

    exitButton = MenuItemImage::create(
        "buttonanddiv/exitbutton.png",
        "buttonanddiv/exitbuttonpressed.png",
        CC_CALLBACK_1(SelectSong::menuCloseCallback, this));

    if (exitButton == nullptr ||
        exitButton->getContentSize().width <= 0 ||
        exitButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/exitbutton.png'");
    }
    else
    {
        float x = origin.x - 5;
        float y = origin.y + visibleSize.height - exitButton->getContentSize().height / 1.6;
        exitButton->setScale(0.5f);
        exitButton->setAnchorPoint(Vec2(0, 0));
        exitButton->setPosition(Vec2(x, y));
    }

    auto exitMenu = Menu::create(exitButton, NULL);
    exitMenu->setPosition(Vec2::ZERO);
    exitMenu->setOpacity(150);

    backLayer->addChild(exitMenu);
#ifndef _BETA
    auto tracbomLogo = Label::createWithTTF("tracbom", "fonts/genshin.ttf", 40);
    if (tracbomLogo == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        tracbomLogo->setAnchorPoint(Vec2(0, 0.5));
        tracbomLogo->setPosition(Vec2(origin.x + 0.55 * exitButton->getContentSize().width, origin.y + visibleSize.height - tracbomLogo->getContentSize().height * 2));


        backLayer->addChild(tracbomLogo);
    }
#else
    auto tracbomLogo = Label::createWithTTF("tracbom (Beta Version)", "fonts/genshin.ttf", 40);
    if (tracbomLogo == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        tracbomLogo->setAnchorPoint(Vec2(0, 0.5));
        tracbomLogo->setPosition(Vec2(origin.x + 0.55 * exitButton->getContentSize().width, origin.y + visibleSize.height - tracbomLogo->getContentSize().height * 2));


        backLayer->addChild(tracbomLogo);
    }

    auto betaLogo = Sprite::create("backanddecos/betalogo.png");
    if (betaLogo == nullptr)
    {
		problemLoading("'backanddecos/betalogo.png'");
	}
    else
    {
		betaLogo->setAnchorPoint(Vec2(0.5, 0.5));
		betaLogo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100));
		betaLogo->setScale(0.7f);
		backLayer->addChild(betaLogo);
    }
#endif

    auto exitText = Label::createWithTTF("EXIT", "fonts/genshin.ttf", 40);
    if (exitText == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        exitText->setPosition(Vec2(origin.x + exitText->getContentSize().width - 3, origin.y + visibleSize.height - exitText->getContentSize().height * 2));


        backLayer->addChild(exitText);
    }


    auto audioButton = MenuItemImage::create(
        "buttonanddiv/audiobutton.png",
        "buttonanddiv/audiobuttonpressed.png",
        CC_CALLBACK_1(SelectSong::menuAudioCallback, this));

    if (audioButton == nullptr ||
        audioButton->getContentSize().width <= 0 ||
        audioButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/audiobutton.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - audioButton->getContentSize().width / 2 + 5;
        float y = origin.y + visibleSize.height - audioButton->getContentSize().height / 1.6;
        audioButton->setScale(0.5f);
        audioButton->setAnchorPoint(Vec2(0, 0));
        audioButton->setPosition(Vec2(x, y));
    }

    auto audioMenu = Menu::create(audioButton, NULL);
    audioMenu->setPosition(Vec2::ZERO);
    audioMenu->setOpacity(150);

    backLayer->addChild(audioMenu);

    auto audioIcon = Sprite::create("backanddecos/SetAudio.png");
    if (audioIcon == nullptr)
    {
		problemLoading("'backanddecos/SetAudio.png'");
	}
    else
    {
		float x = origin.x + visibleSize.width - audioIcon->getContentSize().width - audioButton->getContentSize().height / 14.0;
		float y = origin.y + visibleSize.height - audioButton->getContentSize().height / 2.68;
		audioIcon->setPosition(Vec2(x, y));
	}
    backLayer->addChild(audioIcon);


    auto aboutButton = MenuItemImage::create(
        "buttonanddiv/aboutbutton.png",
        "buttonanddiv/aboutbuttonpressed.png",
        CC_CALLBACK_1(SelectSong::menuAboutCallback, this));

    if (aboutButton == nullptr ||
        aboutButton->getContentSize().width <= 0 ||
        aboutButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/aboutbutton.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - aboutButton->getContentSize().width / 2 - 180;
        float y = origin.y + visibleSize.height - aboutButton->getContentSize().height / 1.6;
        aboutButton->setScale(0.5f);
        aboutButton->setAnchorPoint(Vec2(0, 0));
        aboutButton->setPosition(Vec2(x, y));
    }

    auto aboutMenu = Menu::create(aboutButton, NULL);
    aboutMenu->setPosition(Vec2::ZERO);
    aboutMenu->setOpacity(150);

    backLayer->addChild(aboutMenu);


    auto aboutIcon = Sprite::create("backanddecos/Abouticons.png");
    if (aboutIcon == nullptr)
    {
        problemLoading("'backanddecos/Abouticons.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - aboutIcon->getContentSize().width * 5.3;
        float y = origin.y + visibleSize.height - aboutIcon->getContentSize().height * 1.56;
        aboutIcon->setPosition(Vec2(x, y));
    }
    backLayer->addChild(aboutIcon);


    auto playButton = MenuItemImage::create(
        "buttonanddiv/playbutton.png",
        "buttonanddiv/playbuttonpressed.png",
        CC_CALLBACK_1(SelectSong::menuPlayCallback, this));

    if (playButton == nullptr ||
        playButton->getContentSize().width <= 0 ||
        playButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/playbutton.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - playButton->getContentSize().width / 2;
        float y = origin.y + playButton->getContentSize().height / 2 - 157;
        playButton->setScale(0.5f);
        playButton->setAnchorPoint(Vec2(0, 0));
        playButton->setPosition(Vec2(x, y));
    }

    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(Vec2::ZERO);
    playMenu->setOpacity(150);

    backLayer->addChild(playMenu);

    auto howtoplayIcon = Sprite::create("backanddecos/howtoplay.png");
    if (howtoplayIcon == nullptr)
    {
        problemLoading("'backanddecos/howtoplay.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - howtoplayIcon->getContentSize().width * 5.3 - 223;
        float y = origin.y + visibleSize.height - howtoplayIcon->getContentSize().height * 1.56;
        howtoplayIcon->setPosition(Vec2(x, y));
    }
    backLayer->addChild(howtoplayIcon);


    auto howtoplayButton = MenuItemImage::create(
        "buttonanddiv/aboutbutton.png",
        "buttonanddiv/aboutbuttonpressed.png",
        CC_CALLBACK_1(SelectSong::menuHowtoplayCallback, this));

    if (howtoplayButton == nullptr ||
        howtoplayButton->getContentSize().width <= 0 ||
        howtoplayButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/aboutbutton.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - howtoplayButton->getContentSize().width / 2 - 403;
        float y = origin.y + visibleSize.height - howtoplayButton->getContentSize().height / 1.6;
        howtoplayButton->setScale(0.5f);
        howtoplayButton->setAnchorPoint(Vec2(0, 0));
        howtoplayButton->setPosition(Vec2(x, y));
    }

    auto howtoplayMenu = Menu::create(howtoplayButton, NULL);
    howtoplayMenu->setPosition(Vec2::ZERO);
    howtoplayMenu->setOpacity(150);

    backLayer->addChild(howtoplayMenu);

    auto playText = Label::createWithTTF("PLAY  >", "fonts/genshin.ttf", 60);
    if (playText == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        playText->setPosition(Vec2(origin.x + visibleSize.width - playButton->getContentSize().width / 2 + 172, origin.y + playButton->getContentSize().height / 2 - 67));


        backLayer->addChild(playText);
    }

    auto musicMenuBack = Sprite::create("backanddecos/musicmenu.png");
    if (musicMenuBack == nullptr)
    {
		problemLoading("'backanddecos/musicmenu.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 4 - 87;
		float y = origin.y + visibleSize.height / 2 - 94;
		musicMenuBack->setPosition(Vec2(x, y));
        musicMenuBack->setScale(0.88f);
        musicMenuBack->setOpacity(180);
        backLayer->addChild(musicMenuBack);
	}

    auto musicPicBack = Sprite::create("backanddecos/musicpic.png");
    if (musicPicBack == nullptr)
    {
        problemLoading("'backanddecos/musicpic.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 3 / 4 - 127;
        float y = origin.y + visibleSize.height / 2 + 104;
        musicPicBack->setPosition(Vec2(x, y));
        musicPicBack->setScale(0.44f);
        musicPicBack->setOpacity(180);
        backLayer->addChild(musicPicBack);
    }

    string musicPicPath = "Music/" + MusicFilename + "/"+ MusicFilename + ".png";
    musicPicture = Sprite::create(musicPicPath);
    musicpic_texture = musicPicture->getTexture();
    if (musicPicture == nullptr)
    {
		problemLoading(musicPicPath.c_str());
	}
    else
    {
        musicPicture->setContentSize(Size(musicPicBack->getContentSize().width * 0.44, musicPicBack->getContentSize().height * 0.44));
		musicPicture->setPosition(musicPicBack->getPosition());
		backLayer->addChild(musicPicture);
	}

    auto musicNameBack = Sprite::create("backanddecos/musicname.png");
    if (musicNameBack == nullptr)
    {
		problemLoading("'backanddecos/musicname.png'");
	}
    else
    {
		float x = origin.x + visibleSize.width * 3 / 4 - 127;
		float y = origin.y + visibleSize.height / 3 - 75;
        musicNameBack->setAnchorPoint(Vec2(0.5, 1.0));
        musicNameBack->setPosition(Vec2(x, y));
        musicNameBack->setScale(0.44f);
        musicNameBack->setOpacity(180);
		backLayer->addChild(musicNameBack);
	}

    string musicNamePath = "Music/" + MusicFilename + "/" + MusicFilename + ".txt";
    //auto musicNameChinese = FileUtils::getInstance()->getStringFromFile(musicNamePath);
    std::string musicNameChinese;
    ifstream file(musicNamePath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string key;
            ss >> key;

            if (key == "Name:") {
                getline(ss, musicNameChinese);
            }

        }

        file.close();
    }

    musicname = Text::create(musicNameChinese, "fonts/Deng.ttf", 60);
    if (musicname == nullptr)
    {
        problemLoading("'fonts/Deng.ttf'");
    }
    else
    {
		musicname->setAnchorPoint(Vec2(0, 0.5));
		musicname->setPosition(Vec2(origin.x + visibleSize.width * 3 / 4 - 781, origin.y + visibleSize.height / 3 - 155));
		musicname->setOpacity(230);
		backLayer->addChild(musicname);
	}
    
    auto diffSelectBack = Sprite::create("backanddecos/diffselect.png");
    if (diffSelectBack == nullptr)
    {
		problemLoading("'backanddecos/diffselect.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width * 3 / 4 - 69;
		float y = origin.y + visibleSize.height / 3 - 273;
		diffSelectBack->setAnchorPoint(Vec2(0.5, 1.0));
		diffSelectBack->setPosition(Vec2(x, y));
		diffSelectBack->setScale(0.44f);
		diffSelectBack->setOpacity(180);
		backLayer->addChild(diffSelectBack);
	}
    
    auto highscoreBack = Sprite::create("backanddecos/highscore.png");
    if (highscoreBack == nullptr)
	{
        problemLoading("'backanddecos/highscore.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 3 / 4 - 589;
        float y = origin.y + visibleSize.height / 3 - 273;
        highscoreBack->setAnchorPoint(Vec2(0.5, 1.0));
        highscoreBack->setPosition(Vec2(x, y));
        highscoreBack->setScale(0.44f);
        highscoreBack->setOpacity(180);
        backLayer->addChild(highscoreBack);
    }

    if (Diff == "EZ")
    {
        
        sprintf(hscore, "%07d", scores[0]);
    }
    else if (Diff == "HD") {
        
        sprintf(hscore, "%07d", scores[1]);
    }
    else if (Diff == "IN") {
        
        sprintf(hscore, "%07d", scores[2]);
    }

    highscoretext = Text::create(hscore, "fonts/Arial.ttf", 80);
    if (highscoretext == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        highscoretext->setAnchorPoint(Vec2(0, 0.5));
        highscoretext->setPosition(Vec2(origin.x + visibleSize.width * 3 / 4 - 750, origin.y + visibleSize.height / 3 - 373));
        highscoretext->setOpacity(220);
        backLayer->addChild(highscoretext);
    }

    RadioButtonGroup* diffGroup = RadioButtonGroup::create();
    this->addChild(diffGroup);

    float startx = origin.x + visibleSize.width * 3 / 4 - 205;
    float starty = origin.y + visibleSize.height / 3 - 376;

    auto ez = RadioButton::create("buttonanddiv/ez.png", "buttonanddiv/ezprsd.png");
    ez->setPosition(Vec2(startx, starty));
    ez->setScale(0.8f);
    ez->setOpacity(180);
    diffGroup->addRadioButton(ez);
    diffGroup->addEventListener(CC_CALLBACK_3(SelectSong::menuChangediffCallback, this));
    backLayer->addChild(ez);

    auto hd = RadioButton::create("buttonanddiv/hd.png", "buttonanddiv/hdprsd.png");
    hd->setPosition(Vec2(startx + hd->getContentSize().width, starty));
    hd->setScale(0.8f);
    hd->setOpacity(180);
    diffGroup->addRadioButton(hd);
    diffGroup->addEventListener(CC_CALLBACK_3(SelectSong::menuChangediffCallback, this));
    backLayer->addChild(hd);

    auto in = RadioButton::create("buttonanddiv/in.png", "buttonanddiv/inprsd.png");
    in->setPosition(Vec2(startx + in->getContentSize().width * 2, starty));
    in->setScale(0.8f);
    in->setOpacity(180);
    diffGroup->addRadioButton(in);
    diffGroup->addEventListener(CC_CALLBACK_3(SelectSong::menuChangediffCallback, this));
    backLayer->addChild(in);

    if (Diff == "EZ")
        diffGroup->setSelectedButton(ez);
    else if (Diff == "HD")
        diffGroup->setSelectedButton(hd);
    else if (Diff == "IN")
        diffGroup->setSelectedButton(in);








    ListView* musicSelectBack = ListView::create();
    musicSelectBack->setBackGroundImage("backanddecos/musicmenu.png");
    musicSelectBack->setBackGroundImageScale9Enabled(true);
    musicSelectBack->setAnchorPoint(Vec2(0.5, 0.5));
    musicSelectBack->setContentSize(musicMenuBack->getContentSize());
    musicSelectBack->setScale(0.88f);
    musicSelectBack->setOpacity(140);
    float x = origin.x + visibleSize.width / 4 - 87;
    float y = origin.y + visibleSize.height / 2 - 94;
    musicSelectBack->setPosition(Vec2(x, y));
    musicSelectBack->setBounceEnabled(true);
    backLayer->addChild(musicSelectBack);

    Layout* musicSelectContent = Layout::create();
    musicSelectContent->setContentSize(Size(musicSelectBack->getContentSize().width, 60 + 200 * musicNum));
    musicSelectContent->setAnchorPoint(Vec2(0.5, 1));
    musicSelectContent->setPosition(Vec2(0, musicSelectBack->getContentSize().height / 2 - 50));

    RadioButtonGroup* musicGroup = RadioButtonGroup::create();
    musicGroup->setPosition(Vec2(0, 0));
    musicSelectContent->addChild(musicGroup);

    static int i = 0;
    for (i = 0; i < musicNum; i++) {
        auto button = RadioButton::create("buttonanddiv/music.png", "buttonanddiv/musicpressed.png");
        button->setAnchorPoint(Vec2(0.5, 0.5));
        button->setScale(0.5);
        button->setPosition(Vec2(musicSelectBack->getContentSize().width / 2, musicSelectContent->getContentSize().height - 130 - 200 * i));
        musicSelectContent->addChild(button);
        button->setOpacity(180);

        auto buttontext = Text::create(musicLibChinese[i], "fonts/Deng.ttf", 100);
        if (buttontext == nullptr)
		{
			problemLoading("'fonts/Deng.ttf'");
		}
		else
		{
			buttontext->setAnchorPoint(Vec2(0, 0.5));
			buttontext->setPosition(Vec2(90, button->getContentSize().height / 2));
			buttontext->setOpacity(220);
			button->addChild(buttontext);
		}

        musicGroup->addRadioButton(button);
        musicGroup->addEventListener(CC_CALLBACK_3(SelectSong::menuChangeSongCallback, this));
    }

    musicSelectBack->addChild(musicSelectContent);

    musicGroup->setSelectedButton(selectedmusicindex);

    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/selectsongbackgroundnew.mp3");
    SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/selectsongbackgroundnew.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->setEffectsVolume(notevol / 100.0f);

    return true;
}

void SelectSong::menuAudioCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    //SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = SelectAudio::createScene();
    Director::getInstance()->pushScene(scene);
}

void SelectSong::menuAboutCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    auto scene = About::createScene();
    Director::getInstance()->pushScene(scene);
}

void SelectSong::menuHowtoplayCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
	SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    auto scene = HowtoPlay::createScene();
	Director::getInstance()->pushScene(scene);
}

void SelectSong::menuPlayCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    /////////////onlyfortest/////////////
    auto scene = Play::createScene();
    //auto scenetrans = TransitionFade::create(0.5f, scene);
    Director::getInstance()->pushScene(scene);
    //Director::getInstance()->pushScene(scenetrans);
    /////////////////////////////////////
}

void SelectSong::menuChangediffCallback(RadioButton* button, int index, RadioButtonGroup::EventType type)
{
    fstream highscoreFile;
    highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::_Nocreate);
    if (!highscoreFile.is_open())
    {
        highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::out);
        highscoreFile << "0 0 0";
        highscoreFile.close();
        highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::_Nocreate);
        highscoreFile >> scores[0] >> scores[1] >> scores[2];
        highscoreFile.close();
    }
    else
	{
		highscoreFile >> scores[0] >> scores[1] >> scores[2];
		highscoreFile.close();
	}

    if (type == RadioButtonGroup::EventType::SELECT_CHANGED)
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
        SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
        SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
        SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
		switch (index)
		{
		case 0:
			Diff = "EZ";

            sprintf(hscore, "%07d", scores[0]);
            highscoretext->setString(hscore);
			break;
		case 1:
			Diff = "HD";

            sprintf(hscore, "%07d", scores[1]);
            highscoretext->setString(hscore);
			break;
		case 2:
			Diff = "IN";

            sprintf(hscore, "%07d", scores[2]);
            highscoretext->setString(hscore);
			break;
		default:
			break;
		}
	}
}

void SelectSong::menuCloseCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/exit.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/exit.mp3", false, 1.0, 0.0, notevol / 100.0);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    exitButton->setEnabled(false);
    Sleep(1600);
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void SelectSong::menuChangeSongCallback(RadioButton* button, int index, RadioButtonGroup::EventType type)
{
    if (type == RadioButtonGroup::EventType::SELECT_CHANGED)
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
        SimpleAudioEngine::getInstance()->preloadEffect("sounds/buttontap.mp3");
		SimpleAudioEngine::getInstance()->playEffect("sounds/buttontap.mp3", false, 1.0, 0.0, notevol / 100.0);
        SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
        MusicFilename = musicLib[index];
        string musicNamePath = "Music/" + MusicFilename + "/" + MusicFilename + ".txt";
        //auto musicNameChinese = FileUtils::getInstance()->getStringFromFile(musicNamePath);
        std::string musicNameChinese;
        ifstream file(musicNamePath);
        if (file.is_open()) {
			string line;
            while (getline(file, line)) {
				stringstream ss(line);
				string key;
				ss >> key;

                if (key == "Name:") {
					getline(ss, musicNameChinese);
                    if (musicNameChinese.size() >= 50) {
						musicNameChinese = musicNameChinese.substr(0, 50);
						musicNameChinese += "...";
					}
				}

			}

			file.close();
		}

        musicname->setString(musicNameChinese);
        fstream highscoreFile;
        highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::_Nocreate);
        if (!highscoreFile.is_open())
        {
			highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::out);
			highscoreFile << "0 0 0";
			highscoreFile.close();
			highscoreFile.open("musicscore/" + MusicFilename + ".txt", ios::_Nocreate);
			highscoreFile >> scores[0] >> scores[1] >> scores[2];
			highscoreFile.close();
		}
        else
        {
			highscoreFile >> scores[0] >> scores[1] >> scores[2];
			highscoreFile.close();
		}

        if (Diff == "EZ") {

            sprintf(hscore, "%07d", scores[0]);
        }
        else if (Diff == "HD") {
            
            sprintf(hscore, "%07d", scores[1]);
        }
        else if (Diff == "IN") {
            
            sprintf(hscore, "%07d", scores[2]);
        }
        highscoretext->setString(hscore);
        auto tempmusicpic = Sprite::create("Music/" + MusicFilename + "/" + MusicFilename + ".png");
        musicpic_texture = tempmusicpic->getTexture();
        musicPicture->setTexture(musicpic_texture);

        selectedmusicindex = index;
	}
}
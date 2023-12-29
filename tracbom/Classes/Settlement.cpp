#include "Settlement.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "SelectSong.h"
#include <cstring>
#include <fstream>
#include <Windows.h>

using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;

extern int bgmvol;
extern int notevol;
extern float Score;
extern int combo;
extern int pNum;
extern int gNum;
extern int bNum;
extern int mNum;
extern std::string MusicFilename;
extern std::string Diff;

extern int scores[3];

Scene* Settlement::createScene()
{
    return Settlement::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}


bool Settlement::init()
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

    ifstream scoreFile;
    string scoreFilename = "musicscore/" + MusicFilename + ".txt";
    scoreFile.open(scoreFilename, ios::_Nocreate);
    if (!scoreFile)
    {
		MessageBox("Cannot open scores file!", "Error");
		exit(1);
	}
    else
    {
		scoreFile >> scores[0] >> scores[1] >> scores[2];
        scoreFile.close();
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

    auto settlementBackPicture = Sprite::create("backanddecos/settlement.png");
    if (settlementBackPicture == nullptr)
    {
        problemLoading("'backanddecos/settlement.png'");
    }
    else
    {
        settlementBackPicture->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        settlementBackPicture->setScale(0.8f);
        settlementBackPicture->setOpacity(230);
        backLayer->addChild(settlementBackPicture);
    }


    auto ranka = Sprite::create("ranking/a.png");
    if (ranka == nullptr)
    {
		problemLoading("'ranking/a.png'");
	}
    else
    {
        ranka->setScale(0.63f);
        ranka->setPosition(Vec2(1520, 530));
        ranka->setOpacity(0);
		backLayer->addChild(ranka);
	}

    auto rankb = Sprite::create("ranking/b.png");
    if (rankb == nullptr)
    {
        problemLoading("'ranking/b.png'");
    }
    else
    {
		rankb->setScale(0.63f);
		rankb->setPosition(Vec2(1520, 530));
		rankb->setOpacity(0);
		backLayer->addChild(rankb);
	}

    auto rankc = Sprite::create("ranking/c.png");
    if (rankc == nullptr)
    {
		problemLoading("'ranking/c.png'");
	}
    else
    {
        rankc->setScale(0.63f);
        rankc->setPosition(Vec2(1520, 530));
        rankc->setOpacity(0);
        backLayer->addChild(rankc);
    }

    auto rankf = Sprite::create("ranking/f.png");
    if (rankf == nullptr)
    {
		problemLoading("'ranking/f.png'");
	}
    else
    {
		rankf->setScale(0.63f);
		rankf->setPosition(Vec2(1520, 530));
		rankf->setOpacity(0);
		backLayer->addChild(rankf);
	}

    auto ranks = Sprite::create("ranking/s.png");
    if (ranks == nullptr)
    {
        problemLoading("'ranking/s.png'");
    }
    else
    {
		ranks->setScale(0.63f);
		ranks->setPosition(Vec2(1520, 530));
		ranks->setOpacity(0);
		backLayer->addChild(ranks);
	}

    auto rankv = Sprite::create("ranking/v.png");
    if (rankv == nullptr)
    {
		problemLoading("'ranking/v.png'");
	}
    else
    {
        rankv->setScale(0.63f);
        rankv->setPosition(Vec2(1520, 530));
        rankv->setOpacity(0);
        backLayer->addChild(rankv);
    }

    auto rankm = Sprite::create("ranking/m.png");
    if (rankm == nullptr)
    {
        problemLoading("'ranking/m.png'");
    }
    else
    {
		rankm->setScale(0.63f);
		rankm->setPosition(Vec2(1520, 530));
		rankm->setOpacity(0);
		backLayer->addChild(rankm);
	}

    if (Score == 1000000)
    {
        auto rankmdelay = DelayTime::create(2.7f);
        auto rankmfadein = FadeIn::create(0.6f);
        auto rankmbigger = ScaleTo::create(0.6f, 2.0f);
        auto rankmparallel = Spawn::create(rankmfadein, rankmbigger, NULL);
        auto rankmdelay2 = DelayTime::create(0.6f);
        auto rankmsmaller = ScaleTo::create(0.2f, 0.63f);
        auto rankmseq = Sequence::create(rankmdelay, rankmparallel, rankmdelay2, rankmsmaller, NULL);
        rankm->runAction(rankmseq);
	}
    else if (mNum == 0 && bNum == 0)
    {
        auto rankvdelay = DelayTime::create(2.7f);
        auto rankvfadein = FadeIn::create(0.6f);
        auto rankvbigger = ScaleTo::create(0.6f, 2.0f);
        auto rankvparallel = Spawn::create(rankvfadein, rankvbigger, NULL);
        auto rankvdelay2 = DelayTime::create(0.6f);
        auto rankvsmaller = ScaleTo::create(0.2f, 0.63f);
        auto rankvseq = Sequence::create(rankvdelay, rankvparallel, rankvdelay2, rankvsmaller, NULL);
        rankv->runAction(rankvseq);
    }
    else if ((mNum + bNum <= 6) && Score >= 920000)
    {
        auto ranksdelay = DelayTime::create(2.7f);
		auto ranksfadein = FadeIn::create(0.6f);
		auto ranksbigger = ScaleTo::create(0.6f, 2.0f);
		auto ranksparallel = Spawn::create(ranksfadein, ranksbigger, NULL);
        auto ranksdelay2 = DelayTime::create(0.6f);
		auto rankssmaller = ScaleTo::create(0.2f, 0.63f);
		auto ranksseq = Sequence::create(ranksdelay, ranksparallel, ranksdelay2, rankssmaller, NULL);
        ranks->runAction(ranksseq);
	}
    else if (Score >= 880000)
    {
        auto rankadelay = DelayTime::create(2.7f);
        auto rankafadein = FadeIn::create(0.6f);
        auto rankabigger = ScaleTo::create(0.6f, 2.0f);
        auto rankaparallel = Spawn::create(rankafadein, rankabigger, NULL);
        auto rankadelay2 = DelayTime::create(0.6f);
        auto rankasmaller = ScaleTo::create(0.2f, 0.63f);
        auto rankaseq = Sequence::create(rankadelay, rankaparallel, rankadelay2, rankasmaller, NULL);
        ranka->runAction(rankaseq);
	}
    else if (Score >= 800000)
    {
        auto rankbdelay = DelayTime::create(2.7f);
		auto rankbfadein = FadeIn::create(0.6f);
		auto rankbbigger = ScaleTo::create(0.6f, 2.0f);
		auto rankbparallel = Spawn::create(rankbfadein, rankbbigger, NULL);
        auto rankbdelay2 = DelayTime::create(0.6f);
		auto rankbsmaller = ScaleTo::create(0.2f, 0.63f);
		auto rankbseq = Sequence::create(rankbdelay, rankbparallel, rankbdelay2, rankbsmaller, NULL);
        rankb->runAction(rankbseq);
	}
    else if (Score >= 700000)
    {
        auto rankcdelay = DelayTime::create(2.7f);
        auto rankcfadein = FadeIn::create(0.6f);
        auto rankcbigger = ScaleTo::create(0.6f, 2.0f);
        auto rankcparallel = Spawn::create(rankcfadein, rankcbigger, NULL);
        auto rankcdelay2 = DelayTime::create(0.6f);
        auto rankcsmaller = ScaleTo::create(0.2f, 0.63f);
        auto rankcseq = Sequence::create(rankcdelay, rankcparallel, rankcdelay2, rankcsmaller, NULL);
        rankc->runAction(rankcseq);
	}
    else
    {
        auto rankfdelay = DelayTime::create(2.7f);
		auto rankffadein = FadeIn::create(0.6f);
		auto rankfbigger = ScaleTo::create(0.6f, 2.0f);
		auto rankfparallel = Spawn::create(rankffadein, rankfbigger, NULL);
        auto rankfdelay2 = DelayTime::create(0.6f);
		auto rankfsmaller = ScaleTo::create(0.2f, 0.63f);
		auto rankfseq = Sequence::create(rankfdelay, rankfparallel, rankfdelay2, rankfsmaller, NULL);
        rankf->runAction(rankfseq);
	}

    char scoreLabel[10];
    sprintf(scoreLabel, "%07d", static_cast<int>(Score));

    string pnumLabel = to_string(pNum);
    string gnumLabel = to_string(gNum);
    string bnumLabel = to_string(bNum);
    string mnumLabel = to_string(mNum);
    string comboLabel = to_string(combo);

    auto scoreText = Label::createWithTTF(scoreLabel, "fonts/genshin.ttf", 170);
    if (scoreText == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        scoreText->setAnchorPoint(Vec2(0, 0.5));
        scoreText->setPosition(Vec2(1443, 960));
        scoreText->setOpacity(0);
        backLayer->addChild(scoreText);
    }

    auto pnumText = Label::createWithTTF(pnumLabel, "fonts/Arial.ttf", 50);
    if (pnumText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        pnumText->setAnchorPoint(Vec2(0, 0.5));
        pnumText->setPosition(Vec2(1456, 750));
        pnumText->setOpacity(0);
        backLayer->addChild(pnumText);
    }

    auto gnumText = Label::createWithTTF(gnumLabel, "fonts/Arial.ttf", 50);
    if (gnumText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        gnumText->setAnchorPoint(Vec2(0, 0.5));
        gnumText->setPosition(Vec2(1686, 750));
        gnumText->setOpacity(0);
        backLayer->addChild(gnumText);
    }

    auto bnumText = Label::createWithTTF(bnumLabel, "fonts/Arial.ttf", 50);
    if (bnumText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        bnumText->setAnchorPoint(Vec2(0, 0.5));
        bnumText->setPosition(Vec2(1923, 750));
        bnumText->setOpacity(0);
        backLayer->addChild(bnumText);
    }

    auto mnumText = Label::createWithTTF(mnumLabel, "fonts/Arial.ttf", 50);
    if (mnumText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        mnumText->setAnchorPoint(Vec2(0, 0.5));
        mnumText->setPosition(Vec2(2138, 750));
        mnumText->setOpacity(0);
        backLayer->addChild(mnumText);
    }

    auto comboText = Label::createWithTTF(comboLabel, "fonts/Arial.ttf", 50);
    if (comboText == nullptr)
    {
        problemLoading("'fonts/Arial.ttf'");
    }
    else
    {
        comboText->setAnchorPoint(Vec2(0, 0.5));
        comboText->setPosition(Vec2(1689, 482));
        comboText->setOpacity(0);
        backLayer->addChild(comboText);
    }









    auto backButton = MenuItemImage::create(
        "buttonanddiv/playbutton.png",
        "buttonanddiv/playbuttonpressed.png",
        CC_CALLBACK_1(Settlement::menuReturnCallback, this));

    if (backButton == nullptr ||
        backButton->getContentSize().width <= 0 ||
        backButton->getContentSize().height <= 0)
    {
        problemLoading("'buttonanddiv/playbutton.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - 610;
        float y = origin.y + 415;
        backButton->setScale(0.5f);
        backButton->setAnchorPoint(Vec2(0, 0));
        backButton->setPosition(Vec2(x, y));
        backButton->setOpacity(0);
    }

    auto backText = Label::createWithTTF("NEXT  >", "fonts/genshin.ttf", 60);
    if (backText == nullptr)
    {
        problemLoading("'fonts/genshin.ttf'");
    }
    else
    {
        backText->setPosition(Vec2(backButton->getPositionX() + backButton->getContentSize().width / 4, backButton->getPositionY() + backButton->getContentSize().height / 4 - 2));

        backText->setOpacity(0);
        backLayer->addChild(backText);
    }


    auto menu = Menu::create(backButton, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setOpacity(50);

    backLayer->addChild(menu);

    string picturesrc = "Music/" + MusicFilename + "/" + MusicFilename + ".png";
    auto picture = Sprite::create(picturesrc);
    if (picture == nullptr)
    {
		problemLoading("'musicback'");
	}
    else
    {
        picture->setContentSize(Size(0.38*2560, 0.38*1440));
		picture->setPosition(Vec2(735, 686));
        picture->setOpacity(0);
		backLayer->addChild(picture);
	}

    string musicnamepath = "Music/" + MusicFilename + "/" + MusicFilename + ".txt";
    //auto musicnamechinese = FileUtils::getInstance()->getStringFromFile(musicnamepath);
    std::string tempmusicnamechinese;
    ifstream file(musicnamepath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string key;
            ss >> key;

            if (key == "Name:") {
                getline(ss, tempmusicnamechinese);
                if (tempmusicnamechinese.size() >= 35) {
                    tempmusicnamechinese = tempmusicnamechinese.substr(0, 35);
					tempmusicnamechinese += "...";
                }
            }

        }

        file.close();
    }
    auto musicnameText = Label::createWithTTF(tempmusicnamechinese, "fonts/Deng.ttf", 51);
    if (musicnameText == nullptr)
    {
		problemLoading("'fonts/Deng.ttf'");
	}
    else
    {
		musicnameText->setAnchorPoint(Vec2(0, 0.5));
		musicnameText->setPosition(Vec2(440, 1017));
        musicnameText->setOpacity(200);
		backLayer->addChild(musicnameText);
	}

    auto picturedelay = DelayTime::create(1.0f);
    auto picturefadein = FadeIn::create(1.0f);
    auto pictureseq = Sequence::create(picturedelay, picturefadein, NULL);
    picture->runAction(pictureseq);

    auto scoredelay = DelayTime::create(1.0f);
    auto scorefadein = FadeIn::create(0.3f);
    auto scorebigger = ScaleTo::create(0.3f, 1.1f);
    auto scoreparallel = Spawn::create(scorefadein, scorebigger, NULL);
    auto scoresmaller = ScaleTo::create(0.3f, 1.0f);
    auto scoreseq = Sequence::create(scoredelay, scoreparallel, scoresmaller, NULL);
    scoreText->runAction(scoreseq);

    auto pNumdelay = DelayTime::create(1.0f);
    auto pNumfadein = FadeIn::create(1.0f);
    auto pNumseq = Sequence::create(pNumdelay, pNumfadein, NULL);
    pnumText->runAction(pNumseq);

    auto gNumdelay = DelayTime::create(1.0f);
    auto gNumfadein = FadeIn::create(1.0f);
    auto gNumseq = Sequence::create(gNumdelay, gNumfadein, NULL);
    gnumText->runAction(gNumseq);

    auto bNumdelay = DelayTime::create(1.0f);
    auto bNumfadein = FadeIn::create(1.0f);
    auto bNumseq = Sequence::create(bNumdelay, bNumfadein, NULL);
    bnumText->runAction(bNumseq);

    auto mNumdelay = DelayTime::create(1.0f);
    auto mNumfadein = FadeIn::create(1.0f);
    auto mNumseq = Sequence::create(mNumdelay, mNumfadein, NULL);
    mnumText->runAction(mNumseq);

    auto combodelay = DelayTime::create(1.0f);
    auto combofadein = FadeIn::create(1.0f);
    auto comboseq = Sequence::create(combodelay, combofadein, NULL);
    comboText->runAction(comboseq);

    auto Buttondisappear = ScaleTo::create(0.01f, 0.01f);
    auto Buttondelay = DelayTime::create(3.9f);
    auto Buttonappear = ScaleTo::create(0.3f, 0.5f);
    auto Buttonfadein = FadeIn::create(0.3f);
    auto Buttonparallel = Spawn::create(Buttonappear, Buttonfadein, NULL);
    auto Buttonseq = Sequence::create(Buttondisappear, Buttondelay, Buttonparallel, NULL);
    backButton->runAction(Buttonseq);

    auto backTextdisappear = ScaleTo::create(0.01f, 0.01f);
    auto backTextdelay = DelayTime::create(3.9f);
    auto backTextappear = ScaleTo::create(0.3f, 1.0f);
    auto backTextfadein = FadeIn::create(0.3f);
    auto backTextparallel = Spawn::create(backTextappear, backTextfadein, NULL);
    auto backTextseq = Sequence::create(backTextdisappear, backTextdelay, backTextparallel, NULL);
    backText->runAction(backTextseq);


    SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/selectsongbackgroundnew.mp3", true);
    return true;
}


void Settlement::menuReturnCallback(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/Pick.mp3");
    SimpleAudioEngine::getInstance()->playEffect("sounds/Pick.mp3", false);
    SimpleAudioEngine::getInstance()->setEffectsVolume(bgmvol / 100.0f);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    if (Diff == "EZ")
    {
        if (Score > scores[0])
        {
			scores[0] = Score;
		}
	}
    else if (Diff == "HD")
    {
        if (Score > scores[1])
        {
            scores[1] = Score;
        }
    }
    else if (Diff == "IN")
    {
        if (Score > scores[2])
        {
			scores[2] = Score;
		}
    }
    
    ofstream scoreFile;
    string scoreFilename = "musicscore/" + MusicFilename + ".txt";
    scoreFile.open(scoreFilename, ios::out | ios::trunc);
    if (!scoreFile)
    {
        MessageBox("Cannot open scores file!", "Error");
        exit(1);
    }
    else
    {
		scoreFile << scores[0] << " " << scores[1] << " " << scores[2];
        scoreFile.close();
	}

    auto selectSong = SelectSong::createScene();
    auto trans = TransitionFade::create(0.5f, selectSong);
    Director::getInstance()->replaceScene(trans);

    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("selectsongbackgroundnew.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgmvol / 100.0);
}
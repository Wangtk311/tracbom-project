
#include "NoteTap.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

extern std::string MusicFilename;//歌曲名字
extern std::string Diff;//难度
extern float Time;//时间
extern int Note_strack[4];//轨道

extern Vector<SpriteFrame*> animFrames;

extern float perfectScore;//perfect分数
extern float goodScore;//goodt分数

extern float Score;//分数
extern int combo;//连击数
 
extern int pNum;//perfect数
extern int gNum;//good数
extern int bNum;//bad数
extern int mNum;//miss数

extern int notevol;

extern int combonow;


// on "init" you need to initialize your instance
bool NoteTap::init(Track* track, float jud)
{
	//////////////////////////////
	// 1. super init first
	if (!Sprite::init())
	{
		return false;
	}
	judTime = jud;//设置判定时间
	this->setContentSize(Size(track->width, track->width / 10));
	this->setPosition(Vec2(track->width / 2, track->speed * (judTime - Time)));
	bodyPic = Sprite::create("skin/skin_name/tap.png");
	bodyPic->setAnchorPoint(Vec2(0.5, 0.0));
	bodyPic->setPosition(Vec2(track->width / 2, 0));

	bodyPic->setScale(track->width / bodyPic->getContentSize().width * 0.77f);

	this->addChild(bodyPic);

	//log("create success");

	//音符位置更新
	schedule([this, track](float dt)->void {
		this->setPosition(Vec2(track->width / 2, track->speed * (judTime - Time)));
		//og("Y:%f", track->speed * (judTime - Time));

		}, "tapUpdate"
	);
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sounds/tapEffect1.mp3");
	audio->preloadEffect("sounds/tapEffect2.mp3");
	audio->preloadEffect("sounds/tapEffect3.mp3");
	audio->preloadEffect("sounds/tapEffect4.mp3");
	
	audio->setEffectsVolume(notevol / 100.0f);
	

	//音符打击判定
	schedule([this, track, audio](float dt)->void {

		if (judTime < Time) {
			bodyPic->setOpacity(0);
		}

		//log("Note_strack[0]=%d trackNum=%d ", Note_strack[0], trackNum);
		//log("Note_strack[1]=%d trackNum=%d ", Note_strack[1], trackNum);
		//log("Note_strack[2]=%d trackNum=%d ", Note_strack[2], trackNum);
		//log("Note_strack[3]=%d trackNum=%d ", Note_strack[3], trackNum);
		if (Note_strack[trackNum - 1] == 1) {
			//按键锁定
			Note_strack[trackNum - 1] = -1;

			Animation* hitAnimation = Animation::createWithSpriteFrames(animFrames, 0.008f);
			auto hitAni = Animate::create(hitAnimation);
			auto hit = Sprite::create();

			hit->setScale(track->width / 1247);
			hit->setAnchorPoint(Vec2(0.5, 0.5));
			hit->setPosition(Vec2(track->width / 2, 0));
			this->getParent()->addChild(hit, 2);

			

			if (judTime + 0.150f < Time) {
				this->removeFromParent();
				++mNum;
				if(combo<combonow)
					combo = combonow;
				combonow = 0;
			}

			//bad
			else if (abs(judTime - Time) <= 0.200f && abs(judTime - Time) > 0.150f) {
				//log("bad judtime-Time %f ", judTime - Time);
				this->removeFromParent();
				++bNum;
				if (combo < combonow)
					combo = combonow;
				combonow = 0;
			}
			//good
			else if (abs(judTime - Time) <= 0.150f && abs(judTime - Time) > 0.080f) {
				if (trackNum == 1) {
					//audio->preloadEffect("sounds/tapEffect1.mp3");
					audio->playEffect("sounds/tapEffect1.mp3");
				}
				else if (trackNum == 2) {
					audio->playEffect("sounds/tapEffect2.mp3");
				}
				else if (trackNum == 3) {
					audio->playEffect("sounds/tapEffect3.mp3");
				}
				else if (trackNum == 4) {
					audio->playEffect("sounds/tapEffect4.mp3");
				}
				//audio->playEffect(effectFilename.c_str());

				//log("good judtime-Time %f ", judTime - Time);
				hit->setColor(Color3B(34, 162, 195));
				hit->runAction(Sequence::create(hitAni, CallFunc::create(CC_CALLBACK_0(Track::removeFromParent, hit)), NULL));
				this->removeFromParent();
				++gNum;
				++combonow;
				Score += goodScore;
			}
			//perfect
			else if (abs(judTime - Time) <= 0.080f) {
				//audio->setEffectsVolume(notevol / 100.0f);
				if (trackNum == 1) {
					//audio->preloadEffect("sounds/tapEffect1.mp3");
					audio->playEffect("sounds/tapEffect1.mp3");
				}
				else if (trackNum == 2) {
					audio->playEffect("sounds/tapEffect2.mp3");
				}
				else if (trackNum == 3) {
					audio->playEffect("sounds/tapEffect3.mp3");
				}
				else if (trackNum == 4) {
					audio->playEffect("sounds/tapEffect4.mp3");
				}
				//audio->playEffect(effectFilename.c_str());
				
				//log("perfect judtime-Time %f ", judTime - Time);
				hit->setColor(Color3B(251, 218, 65));

				hit->runAction(Sequence::create(hitAni, CallFunc::create(CC_CALLBACK_0(Track::removeFromParent, hit)), NULL));
				this->removeFromParent();
				++pNum;
				++combonow;
				Score += perfectScore;
			}


		}
		else if (judTime + 0.150f < Time) {
			
				this->removeFromParent();
				++mNum;
				if (combo < combonow)
					combo = combonow;
				combonow = 0;
			


		}





		}, "tapHit"
	);




	return true;
}


//用于设置延迟页面
void NoteTap::setjudTime(float delaytime) {
	static float delaylast=delaytime;
	judTime -= delaylast;
	judTime += delaytime;
	delaylast = delaytime;
}

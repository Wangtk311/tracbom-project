
#include "NoteHold.h"
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
bool NoteHold::init(Track* track, float jud, float end)
{
	//////////////////////////////
	// 1. super init first
	if (!Sprite::init())
	{
		return false;
	}
	//log("TrackNum=%d", trackNum);
	//log("judTime=%f", jud);
	//log("endTime=%f", end);
	judTime = jud;//设置判定时间
	endTime = end;
	this->setContentSize(Size(track->width, track->speed * (endTime - judTime + 2)));

	this->setAnchorPoint(Vec2(0.5f, 0));
	this->setPosition(Vec2(track->width / 2, track->speed * (judTime - Time)));






	headPic = Sprite::create("skin/skin_name/tap.png");
	headPic->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(headPic, 2);
	headPic->setPosition(Vec2(track->width / 2, 0));
	headPic->setScale(track->width / headPic->getContentSize().width * 0.77f);


	bodyPic = Sprite::create("skin/skin_name/hold.png");
	bodyPic->setAnchorPoint(Vec2(0.5f, 0));
	bodyPic->setScaleX(track->width / headPic->getContentSize().width * 0.77f);
	bodyPic->setScaleY(track->speed * (endTime - judTime) / bodyPic->getContentSize().height);
	//bodyPic->setContentSize(Size(track->width, track->speed * (endTime - judTime)));
	this->addChild(bodyPic, 1);
	bodyPic->setPosition(Vec2(track->width / 2, 0));


	tailPic = Sprite::create("skin/skin_name/holdTail.png");
	tailPic->setAnchorPoint(Vec2(0.5f, 0));
	tailPic->setScale(track->width / headPic->getContentSize().width * 0.77f);
	tailPic->setPosition(Vec2(track->width / 2, track->speed * (endTime - judTime)));
	this->addChild(tailPic, 1);


	// 音符位置更新
	schedule([this, track](float dt)->void {
		if (Time <= judTime) {
			this->setPosition(Vec2(track->width / 2, track->speed * (judTime - Time)));
			//og("Y:%f", track->speed * (judTime - Time));
		}
		else {
			headPic->setOpacity(0);
			bodyPic->setScaleY(track->speed * (endTime - Time) / bodyPic->getContentSize().height);
			tailPic->setPosition(Vec2(track->width / 2, track->speed * (endTime - Time)));
		}
		}, "holdUpdate"
	);

	//行不同
	////音符显示裁剪
	//schedule([this, track](float dt)->void {
	//	if (Time >= judTime) {
	//		this->setTextureRect(Rect(0, track->speed*(Time-judTime), track->width, this->getContentSize().height));
	//	}

	//	}, "set"
	//);

	//音符裁剪
	//schedule([this, track](float dt)->void {
	//	if (Time >= judTime) {
	//		this->setTextureRect(Rect(0, track->speed * (Time - judTime), track->width, this->getContentSize().height));
	//	}

	//	}, "set"
	//);

		//log("create success");



		auto audio = SimpleAudioEngine::getInstance();
		audio->preloadEffect("sounds/tapEffect1.mp3");
		audio->preloadEffect("sounds/tapEffect2.mp3");
		audio->preloadEffect("sounds/tapEffect3.mp3");
		audio->preloadEffect("sounds/tapEffect4.mp3");
		audio->setEffectsVolume(notevol / 100.0f);

		//音符打击判定
		schedule([this, track, audio](float dt)->void {

			Animation* hitAnimation = Animation::createWithSpriteFrames(animFrames, 0.008f);
			auto hitAni = Animate::create(hitAnimation);


			//log("Note_strack[0]=%d trackNum=%d ", Note_strack[0], trackNum);
			//log("Note_strack[1]=%d trackNum=%d ", Note_strack[1], trackNum);
			//log("Note_strack[2]=%d trackNum=%d ", Note_strack[2], trackNum);
			//log("Note_strack[3]=%d trackNum=%d ", Note_strack[3], trackNum);
			if (Note_strack[trackNum - 1] == 1 && mode != -2) {
				//按键锁定
				Note_strack[trackNum - 1] = -1;
				ifhold = true;


				//miss
				if (Time - judTime > 0.150f) {
					mode = 0;
					++mNum;
					if (combo < combonow)
						combo = combonow;
					combonow = 0;
					bodyPic->setOpacity(50);//调透明度
					headPic->setOpacity(0);
					tailPic->setOpacity(50);
					//没按倒
				}


				//good
				if (abs(judTime - Time) <= 0.150f && abs(judTime - Time) > 0.080f) {
					if (trackNum == 1) {
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
					
					color = Color3B(34, 162, 195);
					mode = 2;
					//hit->setColor(Color3B(34, 162, 195));

				}
				//perfect
				else if (abs(judTime - Time) <= 0.080f) {
					if (trackNum == 1) {
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
					color = Color3B(251, 218, 65);
					mode = 3;
					//hit->setColor(Color3B(251, 218, 65));


				}


			}


			if (ifhold && num % 20 == 0 && mode > 1) {
				log("repeat!!!!!!!!");
				auto hit = Sprite::create();
				hit->setScale(track->width / 1247);
				hit->setAnchorPoint(Vec2(0.5, 0.5));
				hit->setPosition(Vec2(track->width / 2, 0));
				this->getParent()->addChild(hit, 2, "hit");
				hit->setColor(color);
				hit->runAction(Sequence::create(hitAni, CallFunc::create(CC_CALLBACK_0(Track::removeFromParent, hit)), NULL));

				num++;
				//good
				//if (hit->getColor() == Color3B(34, 162, 195)) {}
				//if (abs(judTime - Time) <= 0.200f && abs(judTime - Time) > 0.100f) {
				//	audio->playEffect(effectFilename.c_str());
				//	log("good judtime-Time %f ", judTime - Time);
				//	hit->setColor(Color3B(34, 162, 195));
				//	
				//	this->removeFromParent();
				//	++gNum;
				//	++combonow;
				//	Score += goodScore;
				//}
				////perfect
				//else if (abs(judTime - Time) <= 0.100f) {
				//	audio->playEffect(effectFilename.c_str());
				//	log("perfect judtime-Time %f ", judTime - Time);
				//	hit->setColor(Color3B(251, 218, 65));

				//	hit->runAction(Sequence::create(hitAni, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, hit)), NULL));
				//	this->removeFromParent();
				//	++pNum;
				//	++combonow;
				//	Score += perfectScore;
				//}

			}
			else if (ifhold && num % 20 != 0 && mode > 1) {
				num++;
			}

			//log("endTime-judtime=%d", endTime - judTime);
			if (Note_strack[trackNum - 1] == 0) {
				ifhold = false;
			}

			//if (!ifhold && Time + 0.200f < endTime && mode!=-1) {
			//	mode = 1;//bad

			//}

			if (mode == -1 && judTime + 0.150f < Time) {//miss
				mode = 0;
				++mNum;
				if (combo < combonow)
					combo = combonow;
				combonow = 0;
				bodyPic->setOpacity(50);
				headPic->setOpacity(0);
				tailPic->setOpacity(50);
			}

			if (!ifhold && mode > 0) {
				if (Time + 0.150f < endTime) {
					mode = 1;//bad
					++bNum;
					if (combo < combonow)
						combo = combonow;
					combonow = 0;
					mode = -2;
					bodyPic->setOpacity(50);
					headPic->setOpacity(0);
					tailPic->setOpacity(50);
				}

				if (Time + 0.150f >= endTime && mode > 1) {
					if (mode == 2) {
						++gNum;
						++combonow;
						Score += goodScore;
						mode = -2;
						//this->removeFromParent();
					}
					else if (mode == 3) {
						++pNum;
						++combonow;
						Score += perfectScore;
						mode = -2;
						//this->removeFromParent();
					}
				}

			}
			else if (ifhold) {
				if (Time + 0.200f >= endTime && mode > 1) {
					if (mode == 2) {
						++gNum;
						++combonow;
						Score += goodScore;
						mode = -2;
						//this->removeFromParent();
					}
					else if (mode == 3) {
						++pNum;
						++combonow;
						Score += perfectScore;
						mode = -2;
						//this->removeFromParent();
					}
				}
			}

			if (endTime <= Time) {

				this->removeFromParent();

			}


			}, "holdHit"
		);




		return true;
}




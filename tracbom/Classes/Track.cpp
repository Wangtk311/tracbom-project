#include<math.h>
#include "Track.h"
extern bool ifPause ;//是否暂停了

USING_NS_CC;

extern std::string MusicFilename;//歌曲名字
extern std::string Diff;//难度
extern float Time;//时间
extern float delaytime;//延迟时间
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Track::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Sprite::init())
	{
		return false;
	}



	bodyPic = Sprite::create("skin/skin_name/track.png");
	bodyPic->setAnchorPoint(Vec2(0.5, 0));
	bodyPic->setPosition(Vec2(this->width / 2, 0));
	bodyPic->setContentSize(Size(width, width*5));
	//bodyPic->setOpacity(50);
	this->addChild(bodyPic,1);
	bodyPic2 = Sprite::create("skin/skin_name/track2.png");
	bodyPic2->setAnchorPoint(Vec2(0.5, 0));
	bodyPic2->setPosition(Vec2(this->width / 2, 0));
	//bodyPic2->setOpacity(50);
	bodyPic2->setScale(width / bodyPic2->getContentSize().width);
	this->addChild(bodyPic2,2);

	headPic = Sprite::create("skin/skin_name/track_head.png");
	headPic->setAnchorPoint(Vec2(0.5, 1));
	headPic->setPosition(Vec2(this->width / 2, 0));
	//headPic->setOpacity(50);
	headPic->setScale(width/headPic->getContentSize().width);
	this->addChild(headPic,1);

	bodyPic->setOpacity(opacity);
	headPic->setOpacity(opacity);
	bodyPic2->setOpacity(opacity);
	//log("create success");

	std::string filename = "Music/" + MusicFilename + "/" + Diff + "/" + TrackNumber + ".txt";


	std::string content = FileUtils::getInstance()->getStringFromFile(filename);
	if (content.empty()) {
		CCLOG("Failed to open file: %s", filename.c_str());
	}

	auto origin = Director::getInstance()->getVisibleOrigin();

	std::stringstream ss(content);
	std::string line;
	while (std::getline(ss, line)) {
		std::vector<float>data(4);
		std::istringstream iss(line);

		std::string token;
		int setType;
		std::getline(iss, token, ' ');
		setType = std::stof(token);
		//log("%d", setType);
		for (int i = 0; i < 4; i++) {
			std::getline(iss, token, ' ');
			
			data[i] = std::stof(token);
			if (setType==1&&i == 3) {
				
				data[i] += origin.x;
			}
			if (setType == 2 && i == 3) {

				data[i] += origin.y;
			}

			if (i == 0 || i == 1) {
				data[i] += delaytime;
			}
		}
		if(setType==1)
			attrX.push_back(data);
		else if(setType == 2)
			attrY.push_back(data);
		else if(setType==3)
			attrWidth.push_back(data);
		else if(setType==4)
			attrAngle.push_back(data);
		else if(setType==5)
			attrOpacity.push_back(data);
		else if(setType==6)
			attrSpeed.push_back(data);
		else
			log("error");

	}

	/*for (int j = 0; j < 1; j++) {
		log("%dcol: ", j);
		for (int i = 0; i < 4; i++) {
			log("%f", attrX[j][i]);
		}
	}
	for (int j = 0; j < 1; j++) {
		log("%dcol: ", j);
		for (int i = 0; i < 4; i++) {
			log("%f", attrY[j][i]);
		}
	}
	for (int j = 0; j < 1; j++) {
		log("%dcol: ", j);
		for (int i = 0; i < 4; i++) {
			log("%f", attrAngle[j][i]);
		}
	}*/



	auto visibleSize = Director::getInstance()->getVisibleSize();
	

	this->schedule(CC_CALLBACK_1(Track::SetX, this), "SetX");
	this->schedule(CC_CALLBACK_1(Track::SetY, this), "SetY");
	this->schedule(CC_CALLBACK_1(Track::SetAngle, this), "SetAngle");
	this->schedule(CC_CALLBACK_1(Track::SetWidth, this), "SetWidth");
	this->schedule(CC_CALLBACK_1(Track::SetOpacity, this), "SetOpacity");
	this->schedule(CC_CALLBACK_1(Track::SetSpeed, this), "SetSpeed");
	return true;
}

//缓动曲线
double easeOutQuart(double x) {
	return 1 - pow(1 - x, 4);
}

double easeInQuart(double x) {
	return x * x * x * x;
}

double easeInOutQuart(double x) {
	if (x < 0.5) {
		return 8 * x * x * x * x;
	}
	else {
		double t = -2 * x + 2;
		return 1 - pow(t, 4) / 2;
	}
}

double easeInBack(double x) {
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

double easeOutBack(double x) {
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

double easeInOutBack(double x) {
	const double c1 = 1.70158;
	const double c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

double easeInCirc(double x) {
	return 1 - sqrt(1 - pow(x, 2));
}

double easeOutCirc(double x) {
	return sqrt(1 - pow(x - 1, 2));
}

double easeInOutCirc(double x) {
	return x < 0.5
		? (1 - sqrt(1 - pow(2 * x, 2))) / 2
		: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}

double easeInElastic(double x) {
	const double c4 = (2 * M_PI) / 3;

	if (x == 0) {
		return 0;
	}
	else if (x == 1) {
		return 1;
	}
	else {
		return -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
	}
}

double easeOutElastic(double x) {
	const double c4 = (2 * M_PI) / 3;

	if (x == 0) {
		return 0;
	}
	else if (x == 1) {
		return 1;
	}
	else {
		return pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
	}
}

double easeInOutElastic(double x) {
	const double c5 = (2 * M_PI) / 4.5;

	if (x == 0) {
		return 0;
	}
	else if (x == 1) {
		return 1;
	}
	else if (x < 0.5) {
		return -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2;
	}
	else {
		return (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
	}
}

double easeOutBounce(double x) {
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

double easeInBounce(double x) {
	return 1 - easeOutBounce(1 - x);
}

double easeInOutBounce(double x) {
	if (x < 0.5) {
		return (1 - easeOutBounce(1 - 2 * x)) / 2;
	}
	else {
		return (1 + easeOutBounce(2 * x - 1)) / 2;
	}
}

double easeInQuad(double x) {
	return x * x;
}

double easeOutQuad(double x) {
	return 1 - (1 - x) * (1 - x);
}

double easeInOutQuad(double x) {
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

double easeInExpo(double x) {
	return x == 0 ? 0 : pow(2, 10 * x - 10);
}

double easeOutExpo(double x) {
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

double easeInOutExpo(double x) {
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? pow(2, 20 * x - 10) / 2 : (2 - pow(2, -20 * x + 10)) / 2;
}


void Track::SetX(float dt) {
	if (!ifPause) {
		if (!attrX.empty() && Time >= attrX[0][0]) {
			if (Time < attrX[0][0] + dt)
				startX = this->getPositionX();
			if (Time > attrX[0][1]) {
				if(attrX[0][3]==20|| attrX[0][3] == 21)
					this->setPositionX(startX);
				else
					this->setPositionX(attrX[0][2]);
				attrX.erase(attrX.begin());
				startX = this->getPositionX();
			}
			else {
				if (attrX[0][3] == 1) {
					this->setPositionX(startX + (Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0]) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 2) {
					this->setPositionX(startX+ easeOutQuart((Time- attrX[0][0])/(attrX[0][1] - attrX[0][0]))* (attrX[0][2] - startX));

				}
				else if (attrX[0][3] == 3) {
					this->setPositionX(startX + easeInQuart((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 4) {
					this->setPositionX(startX + easeInOutQuart((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 5) {
					this->setPositionX(startX + easeInCirc((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 6) {
					this->setPositionX(startX + easeOutCirc((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 7) {
					this->setPositionX(startX + easeInOutCirc((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 8) {
					this->setPositionX(startX + easeInElastic((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 9) {
					this->setPositionX(startX + easeOutElastic((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 10) {
					this->setPositionX(startX + easeInOutElastic((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 11) {
					this->setPositionX(startX + easeOutBounce((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 12) {
					this->setPositionX(startX + easeInBounce((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 13) {
					this->setPositionX(startX + easeInOutBounce((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 14) {
					this->setPositionX(startX + easeInQuad((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 15) {
					this->setPositionX(startX + easeOutQuad((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 16) {
					this->setPositionX(startX + easeInOutQuad((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 17) {
					this->setPositionX(startX + easeInExpo((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 18) {
					this->setPositionX(startX + easeOutExpo((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 19) {
					this->setPositionX(startX + easeInOutExpo((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])) * (attrX[0][2] - startX));
				}
				else if (attrX[0][3] == 20) {
					this->setPositionX(startX + sin((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0])*6.2831853) * attrX[0][2]);
				}
				else if (attrX[0][3] == 21) {
					this->setPositionX(startX + sin((Time - attrX[0][0]) / (attrX[0][1] - attrX[0][0]) * 3.1415926) * attrX[0][2]);
				}
				else {

					MessageBox("谱面动效编写错误", "error");
				}
			}

		}
	}
	//log("set");

}

void Track::SetY(float dt) {

	//log("set2");
	//[0][0]为运动开始时间
	//[0][1]为运动结束时间
	//[0][2]为运动目标位置
	if (!ifPause) {
		if (!attrY.empty() && Time >= attrY[0][0]) {
			if (Time < attrY[0][0] + dt)
				startY = this->getPositionY();
			if (Time >= attrY[0][1]) {
				if (attrY[0][3] == 20|| attrY[0][3] == 21)
					this->setPositionY(startY);
				else
					this->setPositionY(attrY[0][2]);
				attrY.erase(attrY.begin());
				startY = this->getPositionY();
			}
			else {
				if (attrY[0][3] == 1) {
					this->setPositionY(startY + (Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0]) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 2) {
					this->setPositionY(startY + easeOutQuart((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 3) {
					this->setPositionY(startY + easeInQuart((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 4) {
					this->setPositionY(startY + easeInOutQuart((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 5) {
					this->setPositionY(startY + easeInCirc((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 6) {
					this->setPositionY(startY + easeOutCirc((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 7) {
					this->setPositionY(startY + easeInOutCirc((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 8) {
					this->setPositionY(startY + easeInElastic((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 9) {
					this->setPositionY(startY + easeOutElastic((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 10) {
					this->setPositionY(startY + easeInOutElastic((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 11) {
					this->setPositionY(startY + easeOutBounce((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 12) {
					this->setPositionY(startY + easeInBounce((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 13) {
					this->setPositionY(startY + easeInOutBounce((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 14) {
					this->setPositionY(startY + easeInQuad((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 15) {
					this->setPositionY(startY + easeOutQuad((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 16) {
					this->setPositionY(startY + easeInOutQuad((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 17) {
					this->setPositionY(startY + easeInExpo((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 18) {
					this->setPositionY(startY + easeOutExpo((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 19) {
					this->setPositionY(startY + easeInOutExpo((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0])) * (attrY[0][2] - startY));
				}
				else if (attrY[0][3] == 20) {
					this->setPositionY(startY + sin((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0]) * 6.2831853) * attrY[0][2]);
				}
				else if (attrY[0][3] == 21) {
					this->setPositionY(startY + sin((Time - attrY[0][0]) / (attrY[0][1] - attrY[0][0]) * 3.1415926) * attrY[0][2]);
				}
				else {
					MessageBox("谱面动效编写错误", "error");
				}
			}
		}
	}
}

void Track::SetAngle(float dt) {
	//log("attrAngle[0][0]  attrAngle[0][1] %f %f ", attrAngle[0][0], attrAngle[0][1]);
	if (!ifPause) {
		if (!attrAngle.empty() && Time >= attrAngle[0][0]) {
			if (Time < attrAngle[0][0] + dt)
				startA = this->getRotation();
			if (Time >= attrAngle[0][1]) {
				if (attrAngle[0][3] == 20|| attrAngle[0][3] == 21)
					this->setRotation(startA);
				else
					this->setRotation(attrAngle[0][2]);
				attrAngle.erase(attrAngle.begin());
				startA = this->getRotation();
			}
			else {

				if (attrAngle[0][3] == 1) {
					this->setRotation(startA + (Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0]) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 2) {
					this->setRotation(startA + easeOutQuart((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 3) {
					this->setRotation(startA + easeInQuart((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 4) {
					this->setRotation(startA + easeInOutQuart((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 5) {
					this->setRotation(startA + easeInCirc((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 6) {
					this->setRotation(startA + easeOutCirc((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 7) {
					this->setRotation(startA + easeInOutCirc((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 8) {
					this->setRotation(startA + easeInElastic((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 9) {
					this->setRotation(startA + easeOutElastic((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 10) {
					this->setRotation(startA + easeInOutElastic((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 11) {
					this->setRotation(startA + easeOutBounce((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 12) {
					this->setRotation(startA + easeInBounce((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 13) {
					this->setRotation(startA + easeInOutBounce((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 14) {
					this->setRotation(startA + easeInQuad((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 15) {
					this->setRotation(startA + easeOutQuad((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 16) {
					this->setRotation(startA + easeInOutQuad((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 17) {
					this->setRotation(startA + easeInExpo((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 18) {
					this->setRotation(startA + easeOutExpo((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 19) {
					this->setRotation(startA + easeInOutExpo((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0])) * (attrAngle[0][2] - startA));
				}
				else if (attrAngle[0][3] == 20) {
					this->setRotation(startA + sin((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0]) * 6.2831853) * attrAngle[0][2]);
				}
				else if (attrAngle[0][3] == 21) {
					this->setRotation(startA + sin((Time - attrAngle[0][0]) / (attrAngle[0][1] - attrAngle[0][0]) * 3.1415926) * attrAngle[0][2]);
				}
				else {

					MessageBox("谱面动效编写错误", "error");
				}
			}
		}
	}
}

void Track::SetWidth(float dt) {

	//log("attrAngle[0][0]  attrAngle[0][1] %f %f ", attrWidth[0][0], attrWidth[0][1]);
	if (!ifPause) {
		if (!attrWidth.empty() && Time >= attrWidth[0][0]) {
			if (Time < attrWidth[0][0] + dt)
				startW = this->getScale();

			if (Time >= attrWidth[0][1]) {
				if (attrWidth[0][3] == 20 || attrWidth[0][3] == 21)
					this->setScale(startW);
				else
					this->setScale(attrWidth[0][2]);
				attrWidth.erase(attrWidth.begin());
				startW = this->getScale();
			}
			else {
				if (attrWidth[0][3] == 1) {
					this->setScale(startW + (Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0]) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 2) {
					this->setScale(startW + easeOutQuart((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));

				}
				else if (attrWidth[0][3] == 3) {
					this->setScale(startW + easeInQuart((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 4) {
					this->setScale(startW + easeInOutQuart((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 5) {
					this->setScale(startW + easeInCirc((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 6) {
					this->setScale(startW + easeOutCirc((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 7) {
					this->setScale(startW + easeInOutCirc((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 8) {
					this->setScale(startW + easeInElastic((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 9) {
					this->setScale(startW + easeOutElastic((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 10) {
					this->setScale(startW + easeInOutElastic((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 11) {
					this->setScale(startW + easeOutBounce((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 12) {
					this->setScale(startW + easeInBounce((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 13) {
					this->setScale(startW + easeInOutBounce((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 14) {
					this->setScale(startW + easeInQuad((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 15) {
					this->setScale(startW + easeOutQuad((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 16) {
					this->setScale(startW + easeInOutQuad((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 17) {
					this->setScale(startW + easeInExpo((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 18) {
					this->setScale(startW + easeOutExpo((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 19) {
					this->setScale(startW + easeInOutExpo((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0])) * (attrWidth[0][2] - startW));
				}
				else if (attrWidth[0][3] == 20) {
					this->setScale(startW + sin((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0]) * 6.2831853) * attrWidth[0][2]);
				}
				else if (attrWidth[0][3] == 21) {
					this->setScale(startW + sin((Time - attrWidth[0][0]) / (attrWidth[0][1] - attrWidth[0][0]) * 3.1415926) * attrWidth[0][2]);
				}
				else {

					MessageBox("谱面动效编写错误", "error");
				}
			}

		}
	}
}

void Track::SetOpacity(float dt) {
	//log("attrOpacity[0][0]  attrOpacity[0][1] %f %f ", attrOpacity[0][0], attrOpacity[0][1]);
	if (!ifPause) {	
		if (!attrOpacity.empty() && Time >= attrOpacity[0][0]) {
			if (Time < attrOpacity[0][0] + dt)
				startO = bodyPic->getOpacity();

			if (Time >= attrOpacity[0][1]) {
				if (attrOpacity[0][3] == 20||attrOpacity[0][3] == 21) {
					bodyPic->setOpacity(startO);
					headPic->setOpacity(startO);
					bodyPic2->setOpacity(startO);
				}
				else {
					bodyPic->setOpacity(attrOpacity[0][2]);
					headPic->setOpacity(attrOpacity[0][2]);
					bodyPic2->setOpacity(attrOpacity[0][2]);
				}
				attrOpacity.erase(attrOpacity.begin());
				startO = bodyPic->getOpacity();
			}
			else {
				if (attrOpacity[0][3] == 1) {
					bodyPic->setOpacity(startO + (Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + (Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + (Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 2) {
					bodyPic->setOpacity(startO + easeOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));

				}
				else if (attrOpacity[0][3] == 3) {
					bodyPic->setOpacity(startO + easeInQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 4) {
					bodyPic->setOpacity(startO + easeInOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutQuart((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 5) {
					bodyPic->setOpacity(startO + easeInCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 6) {
					bodyPic->setOpacity(startO + easeOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 7) {
					bodyPic->setOpacity(startO + easeInOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutCirc((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 8) {
					bodyPic->setOpacity(startO + easeInElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 9) {
					bodyPic->setOpacity(startO + easeOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 10) {
					bodyPic->setOpacity(startO + easeInOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutElastic((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 11) {
					bodyPic->setOpacity(startO + easeOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 12) {
					bodyPic->setOpacity(startO + easeInBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 13) {
					bodyPic->setOpacity(startO + easeInOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutBounce((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 14) {
					bodyPic->setOpacity(startO + easeInQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 15) {
					bodyPic->setOpacity(startO + easeOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 16) {
					bodyPic->setOpacity(startO + easeInOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutQuad((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 17) {
					bodyPic->setOpacity(startO + easeInExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 18) {
					bodyPic->setOpacity(startO + easeOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 19) {
					bodyPic->setOpacity(startO + easeInOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					headPic->setOpacity(startO + easeInOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
					bodyPic2->setOpacity(startO + easeInOutExpo((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0])) * (attrOpacity[0][2] - startO));
				}
				else if (attrOpacity[0][3] == 20) {
					bodyPic->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 6.2831853) * attrOpacity[0][2]);
					headPic->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 6.2831853) * attrOpacity[0][2]);
					bodyPic2->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 6.2831853) * attrOpacity[0][2]);
				}
				else if (attrOpacity[0][3] == 21) {
					bodyPic->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 3.1415926) * attrOpacity[0][2]);
					headPic->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 3.1415926) * attrOpacity[0][2]);
					bodyPic2->setOpacity(startO + sin((Time - attrOpacity[0][0]) / (attrOpacity[0][1] - attrOpacity[0][0]) * 3.1415926) * attrOpacity[0][2]);
					}
				else {
					
					MessageBox("谱面动效编写错误", "error");
				}
			}

		}
	}

}
void Track::SetSpeed(float dt) {
	//log("attrOpacity[0][0]  attrOpacity[0][1] %f %f ", attrSpeed[0][0], attrSpeed[0][1]);
	if (!ifPause) {
		if (!attrSpeed.empty() && Time >= attrSpeed[0][0]) {
			if (Time < attrSpeed[0][0] + dt)
				startS = speed;

			if (Time >= attrSpeed[0][1]) {
				if (attrSpeed[0][3] == 20||attrSpeed[0][3] == 21)
					speed = startS;
				else
					speed =attrSpeed[0][2];
				attrSpeed.erase(attrSpeed.begin());
				startS = speed;
			}
			else {
				if (attrSpeed[0][3] == 1) {
					speed = startS + (Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0]) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 2) {
					speed = startS + easeOutQuart((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 3) {
					speed = startS + easeInQuart((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 4) {
					speed = startS + easeInOutQuart((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 5) {
					speed = startS + easeInCirc((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 6) {
					speed = startS + easeOutCirc((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 7) {
					speed = startS + easeInOutCirc((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 8) {
					speed = startS + easeInElastic((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 9) {
					speed = startS + easeOutElastic((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 10) {
					speed = startS + easeInOutElastic((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 11) {
					speed = startS + easeOutBounce((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 12) {
					speed = startS + easeInBounce((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 13) {
					speed = startS + easeInOutBounce((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 14) {
					speed = startS + easeInQuad((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 15) {
					speed = startS + easeOutQuad((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 16) {
					speed = startS + easeInOutQuad((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 17) {
					speed = startS + easeInExpo((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 18) {
					speed = startS + easeOutExpo((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 19) {
					speed = startS + easeInOutExpo((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0])) * (attrSpeed[0][2] - startS);
				}
				else if (attrSpeed[0][3] == 20) {
					speed = startS + sin((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0]) * 6.2831853) * attrSpeed[0][2];
				}
				else if (attrSpeed[0][3] == 21) {
					speed = startS + sin((Time - attrSpeed[0][0]) / (attrSpeed[0][1] - attrSpeed[0][0]) * 3.1415926) * attrSpeed[0][2];
				}
				else {
					MessageBox("谱面动效编写错误", "error");
				}
			}

		}
	}
}

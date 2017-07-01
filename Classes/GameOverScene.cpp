#include "GameOverScene.h"//bringing in the game scene
#include "SimpleAudioEngine.h"//iporting the audio engine
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Level2.h"
#include "BonusLevel.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"
#include <iostream>

int Level1score;
int Level2score;
int BonusLevelscore;
int TScore;

USING_NS_CC;

// namespace for audio engine 
//using namespace CocosDenshion;
// namespace for cocos2d
using namespace cocos2d;

//std::string name2 = "Please enter your Name:";
//ui::EditBox* Box;


//Music for the Mainmenu
#define COCOS2D_DEBUG 1

#define MENU_SFX "GameOverMusic.mp3"

Scene* GameOverScene::createScene()//create scene
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()//initing the game so the scene can be made 
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}//code like this for consisinsty 
	

	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

	GameScene Level1;
	Level1score = Level1.getScore();
	CCLOG("Score: init: %i", Level1score);

	Level2 Level2;
	Level2score = Level2.getScore();
	CCLOG("Score: init: %i", Level2score);

	BonusLevel BonusLevel;
	BonusLevelscore = BonusLevel.getScore();
	CCLOG("Score: init: %i", BonusLevelscore);

	GameOverScene Total;
	TScore = Level1score + Level2score + BonusLevelscore;
	CCLOG("Score: init: %i", TScore);

	CCLOG("Score: %d", TScore);
	auto nameLabel = Label::createWithTTF("Your Score: " + std::to_string(TScore), "fonts/Marker Felt.ttf", 24);
	nameLabel->setColor(Color3B::YELLOW);
	nameLabel->setPosition(Vec2(origin.x + winSize.width / 2, 200));
	this->addChild(nameLabel, 1);

	auto MainMenubutton = MenuItemImage::create("lose.png", "lose.png", CC_CALLBACK_1(GameOverScene::GoToMainMenuScene, this));
	MainMenubutton->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));

	auto MainMenuButton = Menu::create(MainMenubutton, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);


	//	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_SFX);
	/*
	auto EBox2 = ui::EditBox::create(Size(200, 40), ".png");
	EBox2->setPlaceHolder("Enter Name:");
	EBox2->setFontSize(20);
	EBox2->setMaxLength(10);
	EBox2->setPosition(Vec2(origin.x + winSize.width / 2, 150));
	this->addChild(EBox2, 1);

	Box = EBox2;
	*/
	return true;
}//end is init()

void GameOverScene::GoToMainMenuScene(Ref *sender)//go to game scene method implementation 
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));

}
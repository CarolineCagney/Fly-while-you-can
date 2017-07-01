//Here all the needed .h files are included
#include "LoseScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "GameScene.h"
#include "Level2.h"
#include "BonusLevel.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include "SimpleAudioEngine.h"

int LevelOnescore;
int LevelTwoscore;
int TheBonusLevelscore;
int TotalScore;

//std::string name = "Please enter your Name:";
//ui::EditBox* Box2;

USING_NS_CC;

// namespace for audio engine 
using namespace CocosDenshion;
// namespace for cocos2d
using namespace cocos2d;

//Music for the Mainmenu
#define MENU_SFX "LoseMusic.mp3"

Scene* LoseScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = LoseScene::create();
	

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoseScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 
	
	GameScene Level1;
	LevelOnescore = Level1.getScore();
	CCLOG("Score: init: %i", LevelOnescore);

	Level2 Level2;
	LevelTwoscore = Level2.getScore();
	CCLOG("Score: init: %i", LevelTwoscore);

	BonusLevel BonusLevel;
	TheBonusLevelscore = BonusLevel.getScore();
	CCLOG("Score: init: %i", TheBonusLevelscore);

	LoseScene Total;
	TotalScore = LevelOnescore + LevelTwoscore + TheBonusLevelscore;
	CCLOG("Score: init: %i", TotalScore);

	CCLOG("Score: %d", TotalScore);
	auto nameLabel = Label::createWithTTF("Your Score: " + std::to_string(TotalScore), "fonts/Marker Felt.ttf", 24);
	nameLabel->setColor(Color3B::YELLOW);
	nameLabel->setPosition(Vec2(origin.x + winSize.width / 2, 200));
	this->addChild(nameLabel, 1);

	auto MainMenubutton = MenuItemImage::create("lose.png", "lose.png", CC_CALLBACK_1(LoseScene::GoToMainMenuScene, this));
	MainMenubutton->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));

	auto MainMenuButton = Menu::create(MainMenubutton, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);
	
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_SFX);
	/*
	auto EBox = ui::EditBox::create(Size(200, 40), ".png");
	EBox->setPlaceHolder("Enter Name:");
	EBox->setFontSize(20);
	EBox->setMaxLength(10);
	EBox->setPosition(Vec2(origin.x + winSize.width / 2, 150));
	this->addChild(EBox, 1);

	Box2 = EBox;
	*/

    return true;
}

void LoseScene::GoToMainMenuScene(Ref *sender)//go to game scene method implementation 
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));

}
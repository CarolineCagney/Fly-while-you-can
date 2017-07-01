#include "MainMenuScene.h"
#include "StoryBook.h"
#include "Definitions.h"
#include "Story1.h"
#include "BonusLevel.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion; // namespace for audio engine 
using namespace cocos2d;

#define MENU_SFX "MenuSceneMusic.mp3"//sound for the splash screen


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_SFX);//menu sound 

	Size visibleSize = Director::getInstance()->getVisibleSize();//init size
	Vec2 origin = Director::getInstance()->getVisibleOrigin();// init the origin 

	auto backgroundSprite = Sprite::create("background3.png");// main menu image 
	backgroundSprite->setPosition(Point(visibleSize.width / 3 + origin.x, visibleSize.height / 3 + origin.y));
	this->addChild(backgroundSprite);

	auto titleSprite = Sprite::create();//image
	titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(titleSprite);

	//button mapping to other screens

	//when click play it goes to the game scene
	auto NewGame = MenuItemImage::create("newGame.png", "newGame.png", CC_CALLBACK_1(MainMenuScene::GoToStory1Scene, this));
	NewGame->setPosition(Point(visibleSize.width / 5 + origin.x, visibleSize.height / 5 + origin.y));

	//options
	auto options = MenuItemImage::create("Storybook.png", "Storybook.png", CC_CALLBACK_1(MainMenuScene::GoToStoryBook, this));
	options->setPosition(Point(visibleSize.width / 2.5 + origin.x, visibleSize.height / 5 + origin.y));

	auto BonusLevel = MenuItemImage::create("BonusLevel.png", "BonusLevel.png", CC_CALLBACK_1(MainMenuScene::GoToBonusLevel, this));
	BonusLevel->setPosition(Point(visibleSize.width / 1.66 + origin.x, visibleSize.height / 5 + origin.y));

	auto closeItem = MenuItemImage::create("quit.png", "quit.png", CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
	closeItem->setPosition(Point(visibleSize.width / 1.30 + origin.x, visibleSize.height / 5 + origin.y));

	const float ScoreFontSize = 65;

	GameName = CCLabelTTF::create("Fly While You Can!", "fonts/arial", ScoreFontSize);
	GameName->setPosition(Point(visibleSize.width / 2.5 + origin.x, visibleSize.height / 1.7 + origin.y));
	this->addChild(GameName);

	auto NewGameButton = Menu::create(NewGame, NULL);
	NewGameButton->setPosition(Point::ZERO);
	this->addChild(NewGameButton);

	auto BonusLevelButton = Menu::create(BonusLevel, NULL);
	BonusLevelButton->setPosition(Point::ZERO);
	this->addChild(BonusLevelButton);

	auto optionsMenu = Menu::create(options, NULL);
	optionsMenu->setPosition(Point::ZERO);
	this->addChild(optionsMenu);

	auto exit = Menu::create(closeItem, NULL);
	exit->setPosition(Point::ZERO);
	this->addChild(exit);

	return true;
}

void MainMenuScene::GoToStory1Scene(Ref *sender)//go to game scene method implementation 
{
	auto scene = Story1::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::GoToStoryBook(Ref *sender)//go to game scene method implementation 
{
	auto scene = StoryBook::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::GoToBonusLevel(Ref *sender)//go to game scene method implementation 
{
	auto scene = BonusLevel::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}
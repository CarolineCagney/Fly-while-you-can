//Here all the needed .h files are included
#include "Story1.h"
#include "GameScene.h"
#include "Definitions.h"

// namespace for cocos2d
using namespace cocos2d;

USING_NS_CC;

#define COCOS2D_DEBUG 1

Scene* Story1::createScene()//create scene
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();//creating the scene with added physcis engine 
	auto layer = Story1::create();//creating the game layer 
								  // add layer as a child to scene
	scene->addChild(layer);//adding the layer to the scene 

						   // return the scene
	return scene;//returning the scene so it can be made 
}

// on "init" you need to initialize your instance
bool Story1::init()//initing the game so the scene can be made 
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}//code like this for consisinsty 

	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

	auto MainMenu = MenuItemImage::create("Story1background.png", "Story1background.png", CC_CALLBACK_1(Story1::GoToGameSceneScene, this));
	MainMenu->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));

	auto MainMenuButton = Menu::create(MainMenu, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);

}//end is init()


void Story1::GoToGameSceneScene(Ref *sender)//go to game scene method implementation 
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
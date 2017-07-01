//Here all the needed .h files are included
#include "Story2.h"
#include "Level2.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// namespace for audio engine 
using namespace CocosDenshion;
// namespace for cocos2d
using namespace cocos2d;

//Music for the Mainmenu
#define MENU_SFX "MenuSceneMusic.mp3"

Scene* Story2::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = Story2::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Story2::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

	auto Level2Image = MenuItemImage::create("Story2background.png", "Story2background.png", CC_CALLBACK_1(Story2::GoToLevel2, this));
	Level2Image->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));

	auto Level2 = Menu::create(Level2Image, NULL);
	Level2->setPosition(Point::ZERO);
	this->addChild(Level2);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_SFX);
	
    return true;
}

void Story2::GoToLevel2(Ref *sender)//go to game scene method implementation 
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(MENU_SFX);
	CCLOG("HELLO");
	auto scene = Level2::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));

}
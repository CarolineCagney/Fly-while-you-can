#include "StoryBook.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* StoryBook::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = StoryBook::create();
	

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StoryBook::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

	auto MainMenubutton = MenuItemImage::create("Storybook1.png", "Storybook1.png", CC_CALLBACK_1(StoryBook::GoToMainMenuScene, this));
	MainMenubutton->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));

	auto MainMenuButton = Menu::create(MainMenubutton, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);
	
    return true;
}

void StoryBook::GoToMainMenuScene(Ref *sender)//go to game scene method implementation 
{
	CCLOG("HELLO");
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));

}
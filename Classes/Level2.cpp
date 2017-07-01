//All the .h files needed for this scene are included
#include "Level2.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "LoseScene.h"
#include "GameOverScene.h"
#include <sstream>
#include <cstdlib>

int score2;

// namespace for audio engine 
using namespace CocosDenshion;
// namespace for cocos2d
using namespace cocos2d;

//Music for the Mainmenu
#define GAME_SOUND "GameScene.mp3"
#define SOUND_EFFECT "L2soundeffect.mp3"
#define DEATH_EFFECT "L1soundeffect.mp3"

cocos2d::PhysicsWorld * sceneWorld;

USING_NS_CC;

#define COCOS2D_DEBUG 1

enum class PhysicsCategory
{
	None = 0,
	cross = (1 << 0),    // 1
	Projectile = (1 << 1), // 2

};

Scene* Level2::createScene()//create scene
{
	// 'scene' is an autorelease object
	//creating the scene with the physcis engine added
	auto scene = Scene::createWithPhysics();
	//Here gravity is set
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	// 'layer' is an autorelease object
	//Here the game layer is created 
	auto layer = Level2::create();
	//Here the Physics world seen is swt
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	//Here the physics layer is being added to the scene 
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Level2::init()//initing the game so the scene can be made 
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}//code like this for consisinsty 

	auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
	auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

	auto backgroundSprite = Sprite::create("background1.png");
	//Setting the position for the main menu background image
	backgroundSprite->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));
	//Adding the background image sprite to the scene
	this->addChild(backgroundSprite);

	this->schedule(schedule_selector(Level2::addCross), 1);
	this->schedule(schedule_selector(Level2::addGarlic), 1);
	this->schedule(schedule_selector(Level2::addBlood), 0.20);

	//	this->addChild(_tileMap);
	_player = Sprite::create("player.png");

	_player->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));

	auto spriteBody = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(2);
	spriteBody->setContactTestBitmask(true);

	_player->setPhysicsBody(spriteBody);

	this->addChild(_player);
	
	const float ScoreFontSize = 24;
	const float  ScorePostitionX = 24;
	const float ScorePostitionY = 12;
	score2 = 0;

	__String *tempScore = __String::createWithFormat("%i", score2);

	scoreLabel = Label::create(tempScore->getCString(), "fonts/Marker felt.ttf", winSize.height* SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::RED);
	scoreLabel->setAnchorPoint(ccp(0, 1));
	scoreLabel->setPosition(winSize.width / 2 + origin.x, winSize.height * SCORE_FONT_SIZE);

	this->addChild(scoreLabel, 1000);


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level2::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();

	auto eventListener = EventListenerKeyboard::create();

	timer = Label::createWithTTF("Time", "fonts/arial.ttf", 16);
	timer->setColor(ccc3(0, 0, 0));
	timer->setAnchorPoint(ccp(0, 1));
	timer->setPosition(Point(winSize.width / 5 + origin.x, winSize.height / 1.1 + origin.y));
	this->addChild(timer);

	time = 60;

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:

			goLeft = true;
			CCLOG("LEFT");
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:

			goRight = true;
			CCLOG("Right");
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:

			goUp = true;
			CCLOG("Up");
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:

			goDown = true;
			CCLOG("Down");
			break;
		}
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

		return true;// returnign that all is ok as is a bool(booean class)
	};
	CCLOG(" FORGETTI");
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		switch (keyCode) {

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			goLeft = false;
			CCLOG("LEFT");
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			goRight = false;
			CCLOG("RIGHT");
			break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			goUp = false;
			CCLOG("UP");
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			goDown = false;
			CCLOG("DOWN");
			break;
		}

	};

	auto eventListenertouchyfeely = EventListenerTouchOneByOne::create();
	eventListenertouchyfeely->onTouchBegan = [=](Touch * touch, Event *unused_event)
	{
		if (_player->getPosition().y > touch->getLocation().y)
		{
			goDown = true;
		}
		else
		{
			goUp = true;
		}
		if (_player->getPosition().x > touch->getLocation().x)
		{
			goLeft = true;
		}
		else
		{
			Vec2 loc = _player->getPosition();
			goRight = true;
		}

		return true;
	};

	eventListenertouchyfeely->onTouchEnded = [=](Touch * touch, Event *unused_event)
	{
		goDown = false;
		goUp = false;
		goLeft = false;
		goRight = false;
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenertouchyfeely, _player);

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, _player);
	CCLOG("NEVER PEPPY");
	auto MainMenu = MenuItemImage::create("MainMenu.png", "MainMenu.png", CC_CALLBACK_1(Level2::GoToMainMenuScene, this));
	MainMenu->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 1.1 + origin.y));

	auto MainMenuButton = Menu::create(MainMenu, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);

	auto Home = Sprite::create("Home.png");
	Home->setPosition(Point(7 * winSize.width / 8 + origin.x, winSize.height / 6 + origin.y));

	this->addChild(Home);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(GAME_SOUND);

}//end is init()

void Level2::addCross(float dt)
{
	auto cross = Sprite::create("cross1.png");//making the enemy 

											  //giving the monster some attributes 
	auto crossSize = cross->getContentSize();
	auto spriteBody = PhysicsBody::createBox(cross->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(1);
	spriteBody->setContactTestBitmask(true);

	cross->setPhysicsBody(spriteBody);// adding monster to the physics engine so it can be colided 
									  // 1
									  // giving the monster some movement and coordnates
	auto crossContentSize = cross->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minX = crossContentSize.width / 2;
	int maxX = selfContentSize.width - crossContentSize.width / 2;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	cross->setPosition(Vec2(randomX, -crossContentSize.height));
	this->addChild(cross);//adding enemy to the layer 

						  // 2
	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = (rand() % rangeDuration) + minDuration;

	// 3
	//moving and taking off when collided 
	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, selfContentSize.height + crossContentSize.height));//
	auto actionRemove = RemoveSelf::create();
	cross->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

void Level2::addGarlic(float dt)
{
	auto garlic = Sprite::create("garlic.png");//making the enemy 

											   //giving the garlic some attributes 
	auto garlicSize = garlic->getContentSize();
	auto spriteBody = PhysicsBody::createBox(garlic->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(3);
	spriteBody->setContactTestBitmask(true);

	garlic->setPhysicsBody(spriteBody);// adding monster to the physics engine so it can be colided 
									   // 1
									   // giving the monster some movement and coordnates
	auto garlicContentSize = garlic->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minX = garlicContentSize.width / 2;
	int maxX = selfContentSize.width - garlicContentSize.width / 2;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	garlic->setPosition(Vec2(randomX, -garlicContentSize.height));
	this->addChild(garlic);//adding enemy to the layer 

						   // 2
	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = (rand() % rangeDuration) + minDuration;

	// 3
	//moving and taking off when collided 
	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, selfContentSize.height + garlicContentSize.height));//
	auto actionRemove = RemoveSelf::create();
	garlic->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

void Level2::addBlood(float dt)
{
	//making the enemy 
	auto blood = Sprite::create("blood.png");

	//giving the monster some attributes 
	auto bloodSize = blood->getContentSize();
	auto spriteBody = PhysicsBody::createBox(blood->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(4);
	spriteBody->setContactTestBitmask(true);

	blood->setPhysicsBody(spriteBody);// adding monster to the physics engine so it can be colided 

									  // giving the monster some movement and coordnates
	auto bloodContentSize = blood->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minX = bloodContentSize.width / 2;
	int maxX = selfContentSize.width - bloodContentSize.width / 2;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	//Here the stakes position is set
	blood->setPosition(Vec2(randomX, -bloodContentSize.height));
	//Here the blood is added to the layer 
	this->addChild(blood);

	// 2
	int minDuration = 1.0;
	int maxDuration = 2.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = (rand() % rangeDuration) + minDuration;
	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, selfContentSize.height + bloodContentSize.height));//
	auto actionRemove = RemoveSelf::create();
	blood->runAction(Sequence::create(actionMove, actionRemove, nullptr));

}


bool Level2::onTouchBegan(Touch * touch, Event *unused_event)
{
	// 2
	//setting up the vecs and what they are doing 
	Vec2 touchLocation = touch->getLocation();
	Vec2 offset = touchLocation - _player->getPosition();

	// 3
	if (offset.x < 0) //offset is the area at which the "bullet" will fire 
	{
		return true;
	}

	// 4
	auto projectile = Sprite::create("player.png");//making the projectile 
	projectile->setPosition(_player->getPosition());
	this->addChild(projectile);//adding it to the layer 

							   //setting the phycis of the projectile 
	auto projectileSize = projectile->getContentSize();
	auto physicsBody = PhysicsBody::createCircle(projectileSize.width / 2);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
	physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
	physicsBody->setContactTestBitmask((int)PhysicsCategory::cross);
	projectile->setPhysicsBody(physicsBody);

	// 5
	offset.normalize();
	auto shootAmount = offset * 1000;

	// 6
	auto realDest = shootAmount + projectile->getPosition();

	// 7
	auto actionMove = MoveTo::create(2.0f, realDest);
	auto actionRemove = RemoveSelf::create();
	projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));

	// sound plays once the player clicks the screen 
	//	SimpleAudioEngine::getInstance()->playEffect(TOWER_SHOOTING_SFX);//tower shooting sound 
	return true;
}
bool Level2::onContactBegin(PhysicsContact &contact)
{

	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	/*Checking if the player and crosses have collided

	*/

	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() || 2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		SimpleAudioEngine::getInstance()->playEffect(SOUND_EFFECT);
		auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
		auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

		_player->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));

		CCLOG("COLLISION HAS OCCURED");

	}

	if ((2 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() || 3 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()))
	{
		SimpleAudioEngine::getInstance()->playEffect(SOUND_EFFECT);
		auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
		auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

		_player->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));

		CCLOG("COLLISION HAS OCCURED");
	}
	if ((2 == a->getCollisionBitmask() && 4 == b->getCollisionBitmask() || 4 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()))
	{
		//	SimpleAudioEngine::getInstance()->playEffect(SOUND_EFFECT);
		auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
		auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 



		score2++;

		__String * tempScore = __String::createWithFormat("%d", score2);
		scoreLabel->setString(tempScore->getCString());

		CCLOG("COLLISION HAS OCCURED");
	}
	return true;
}

void Level2::SetIsScored()
{
	scored = true;
}

bool Level2::GetIsScored()
{
	return scored;
}

int Level2::getScore()
{
	CCLOG("Score: %d", score2);
	return score2;
}

void Level2::menuCloseCallback(Ref* pSender)// setting up the close button "quit"
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0); // origallny in the code 
#endif

}

void Level2::GoToMainMenuScene(Ref *sender)//go to game scene method implementation 
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_SOUND);
	CCLOG("HELLO");
	this->unschedule(schedule_selector(Level2::addCross));
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Level2::GoToLoseScene(Ref *sender)//go to game scene method implementation 
{
	SimpleAudioEngine::getInstance()->playEffect(DEATH_EFFECT);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_SOUND);
	this->unschedule(schedule_selector(Level2::addCross));
	auto scene = LoseScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Level2::GoToGameOverScene(Ref *sender)//go to game scene method implementation 
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_SOUND);
	this->unschedule(schedule_selector(Level2::addCross));
	auto scene = GameOverScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
void Level2::update(float delta)
{
	time -= delta;
	if (time <= 0) {
		time = 10000000;
		GoToLoseScene(NULL);
	}


	auto winSize = Director::getInstance()->getVisibleSize();

	if (_player->getPosition().x > winSize.width * 0.9)
	{
		_player->setPosition(Vec2(0, 0));
		GoToGameOverScene(NULL);
	}
	std::string t;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	t = std::to_string(time);

#else CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	std::ostringstream t1;
	t1 << time;
	t = t1.str();
#endif

	timer->setString(t);

	if (goLeft)
	{
		if (_player->getPosition().x > 0)
		{
			Vec2 loc = _player->getPosition();
			_player->setPosition(loc.x -= 2, loc.y);
		}
	}

	if (goRight)
	{
		Vec2 loc = _player->getPosition();
		_player->setPosition(loc.x += 2, loc.y);
	}

	if (goUp)
	{
		if (_player->getPosition().y < winSize.height)
		{
			Vec2 loc = _player->getPosition();
			_player->setPosition(loc.x, loc.y += 2);
		}
	}

	if (goDown)
	{
		if (_player->getPosition().y > 0)
		{
			Vec2 loc = _player->getPosition();
			_player->setPosition(loc.x, loc.y -= 2);
		}
	}
}
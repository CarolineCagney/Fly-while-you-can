//Here all the needed .h files are included
// Here sstream is included to get this game to work on android
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "Level2.h"
#include "LoseScene.h"
#include "Story2.h"
#include <sstream>

int score;

// namespace for audio engine 
using namespace CocosDenshion;
// cocos2d namespace 
using namespace cocos2d;

#define GAME_MUSIC "GameScene.mp3"
#define DEATH_EFFECT "L1soundeffect.mp3"

//Getting the physics world so that this scene is able to use physics
cocos2d::PhysicsWorld *World;

USING_NS_CC;

#define COCOS2D_DEBUG 1

enum class PhysicsCategory
{
	None = 0,
	stake = (1 << 0),    // 1
	Projectile = (1 << 1), // 2

};

//Here the Game scene is created
Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	//Here the game scene is being created with the physcis engine being added
	auto scene = Scene::createWithPhysics();

	//setting the gravity to fall in whaterver way via x/y coordnate 
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	// 'layer' is an autorelease object
	//Here game layer for game scene is created
	auto layer = GameScene::create();
	//Here the physics layer is being set 
	layer->setPhysics(scene->getPhysicsWorld());
	//Here the physics layer is being added to the layer as a child to scene
	scene->addChild(layer);

	//Here the scene is being returned
	//The scene is being returned so that the scene can be made 
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()//initing the game so the scene can be made 
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}//code like this for consisinsty 

	 //Here the origin is being set up
	auto origin = Director::getInstance()->getVisibleOrigin();
	//Here the window size or the visible size is being set up here
	auto winSize = Director::getInstance()->getVisibleSize();

	auto backgroundSprite = Sprite::create("background4.png");
	//Setting the position for the main menu background image
	backgroundSprite->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));
	//Adding the background image sprite to the scene
	this->addChild(backgroundSprite);

	//Here the number of stakes added to the scene per second is set
	this->schedule(schedule_selector(GameScene::addStake), 0.35);
	this->schedule(schedule_selector(GameScene::addBlood), 0.20);

	//Here the image sprite for the player is being created here
	_player = Sprite::create("player.png");
	//Here the position for the player image is being set
	_player->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));
	//Here the Physicsbody for the player is being created
	auto spriteBody = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(2);
	spriteBody->setContactTestBitmask(true);

	_player->setPhysicsBody(spriteBody);

	this->addChild(_player);

	const float ScoreFontSize = 24;
	const float  ScorePostitionX = 24;
	const float ScorePostitionY = 12;
	score = 0;

	__String *tempScore = __String::createWithFormat("%i", score);

	scoreLabel = Label::create(tempScore->getCString(), "fonts/Marker felt.ttf", winSize.height* SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::RED);
	scoreLabel->setAnchorPoint(ccp(0, 1));
	scoreLabel->setPosition(winSize.width / 2 + origin.x, winSize.height * SCORE_FONT_SIZE);

	this->addChild(scoreLabel, 1000);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	time = 60;

	this->scheduleUpdate();

	auto eventListener = EventListenerKeyboard::create();

	timer = Label::createWithTTF("Time", "fonts/arial.ttf", 16);
	timer->setAnchorPoint(ccp(0, 1));
	timer->setPosition(Point(winSize.width / 5 + origin.x, winSize.height / 1.1 + origin.y));
	this->addChild(timer);

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
	};
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

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

	auto MainMenu = MenuItemImage::create("MainMenu.png", "MainMenu.png", CC_CALLBACK_1(GameScene::GoToMainMenuScene, this));
	MainMenu->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 1.1 + origin.y));

	auto MainMenuButton = Menu::create(MainMenu, NULL);
	MainMenuButton->setPosition(Point::ZERO);
	this->addChild(MainMenuButton);

	auto NextScene = Sprite::create("sign.png");
	NextScene->setPosition(Point(7 * winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));

	this->addChild(NextScene);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_MUSIC);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(GAME_MUSIC);
}//end is init()

void GameScene::addStake(float dt)
{
	//making the enemy 
	auto stake = Sprite::create("stake.png");

	//giving the monster some attributes 
	auto stakeSize = stake->getContentSize();
	auto spriteBody = PhysicsBody::createBox(stake->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(1);
	spriteBody->setContactTestBitmask(true);

	stake->setPhysicsBody(spriteBody);// adding monster to the physics engine so it can be colided 

									  // giving the monster some movement and coordnates
	auto stakeContentSize = stake->getContentSize();
	auto selfContentSize = this->getContentSize();
	int minX = stakeContentSize.width / 2;
	int maxX = selfContentSize.width - stakeContentSize.width / 2;
	int rangeX = maxX - minX;
	int randomX = (rand() % rangeX) + minX;

	//Here the stakes position is set
	stake->setPosition(Vec2(randomX, -stakeContentSize.height));
	//Here the stake is added to the layer 
	this->addChild(stake);

	// 2
	int minDuration = 1.0;
	int maxDuration = 2.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = (rand() % rangeDuration) + minDuration;
	auto actionMove = MoveTo::create(randomDuration, Vec2(randomX, selfContentSize.height + stakeContentSize.height));//
	auto actionRemove = RemoveSelf::create();
	stake->runAction(Sequence::create(actionMove, actionRemove, nullptr));

}


void GameScene::addBlood(float dt)
{
	//making the enemy 
	auto blood = Sprite::create("blood.png");

	//giving the monster some attributes 
	auto bloodSize = blood->getContentSize();
	auto spriteBody = PhysicsBody::createBox(blood->getContentSize(), PhysicsMaterial(0, 1, 0));
	spriteBody->setCollisionBitmask(3);
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
	blood->setTag(12);
}

bool GameScene::onTouchBegan(Touch * touch, Event *unused_event)
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
	physicsBody->setContactTestBitmask((int)PhysicsCategory::stake);
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
bool GameScene::onContactBegin(PhysicsContact &contact)
{

	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	/*Checking if the player and stakes have collided

	*/
	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() || 2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		SimpleAudioEngine::getInstance()->playEffect(DEATH_EFFECT);
		auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
		auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 

		_player->setPosition(Point(winSize.width / 8 + origin.x, winSize.height / 3 + origin.y));

		CCLOG("COLLISION HAS OCCURED");
	}
	
		if ((2 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() || 3 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()))
	{
	//	SimpleAudioEngine::getInstance()->playEffect(SOUND_EFFECT);
		auto origin = Director::getInstance()->getVisibleOrigin();//setting up the origin 
		auto winSize = Director::getInstance()->getVisibleSize();// as well as the window size or the visible size as well 		

	//	removeChildByTag(12);//remove the drop of blood from the scene
		removeChildByName("12");
		score++;
		
		__String * tempScore = __String::createWithFormat("%d", score);
		scoreLabel->setString(tempScore->getCString());		

		CCLOG("COLLISION HAS OCCURED");
	}
	return true;
}

//Here when the house is pressed in the game the player is  brought to the MainMenu scene
void GameScene::GoToMainMenuScene(Ref *sender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_MUSIC);
	this->unschedule(schedule_selector(GameScene::addStake));
	//Here the main menu scene is created 
	auto scene = MainMenuScene::createScene();
	//Here the scene is being replaced and the scenes transistion time is set here 
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

//When the player wins the level, they are brought to the second story scene
void GameScene::GoToStory2(Ref *sender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_MUSIC);
	//Here the second story scene is created 
	auto scene = Story2::createScene();
	//Here the scene is being replaced and the scenes transistion time is set here 
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

//When the player loses the level, they are brought to the lose scene

void GameScene::GoToLoseScene(Ref *sender)//go to game scene method implementation 
{
	SimpleAudioEngine::getInstance()->playEffect(DEATH_EFFECT);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(GAME_MUSIC);
	this->unschedule(schedule_selector(GameScene::addStake));
	auto scene = LoseScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameScene::SetIsScored()
{
	scored = true;
}

bool GameScene::GetIsScored()
{
	return scored;
}

int GameScene::getScore()
{
	CCLOG("Score: %d", score);
	return score;
}

void GameScene::menuCloseCallback(Ref* pSender)// setting up the close button "quit"
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0); // origallny in the code 
#endif

}


void GameScene::update(float delta)
{
	time -= delta;
	if (time <= 0) {
		time = 100000 - 00;
		GoToLoseScene(NULL);
	}

	auto winSize = Director::getInstance()->getVisibleSize();

	if (_player->getPosition().x > winSize.width * 0.9)
	{
		_player->setPosition(Vec2(0, 0));
		GoToStory2(NULL);
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
			_player->setPosition(loc.x -= 3, loc.y);
		}
	}

	if (goRight)
	{
		Vec2 loc = _player->getPosition();
		_player->setPosition(loc.x += 3, loc.y);
	}

	if (goUp)
	{
		if (_player->getPosition().y < winSize.height)
		{
			Vec2 loc = _player->getPosition();
			_player->setPosition(loc.x, loc.y += 3);
		}
	}

	if (goDown)
	{
		if (_player->getPosition().y > 0)
		{
			Vec2 loc = _player->getPosition();
			_player->setPosition(loc.x, loc.y -= 3);
		}
	}
}
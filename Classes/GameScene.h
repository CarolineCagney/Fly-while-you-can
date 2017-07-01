#ifndef __GAME_SCENE_H__ // used so that hte header will only be used once per compile
#define __GAME_SCENE_H__

#include "cocos2d.h"// brining in cocos funcality 
#include "Box2D/Box2D.h"

using namespace cocos2d; // namespace for cococs so you dont have to use cocos2d:: every time 

class GameScene : public cocos2d::Layer
{

private:
	Sprite* _player;
	Label * scoreLabel;

	PhysicsWorld *World;
	void setPhysics(PhysicsWorld * world) { World = world; };

public:
	
	static cocos2d::Scene* createScene();

	virtual bool init();// creating the bool init calss 

	void addStake(float dt);
	void addBlood(float dt);

	float time;
	virtual void update(float delta);

	bool onTouchBegan(Touch * touch, Event *unused_event);
	
	//bool onContactBegan(PhysicsContact &contact);

	bool onContactBegin(cocos2d::PhysicsContact &contact);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	cocos2d::Label *timer;

	//	cocos2d::Sprite *player;

	bool goLeft = false;
	bool goRight = false;
	bool goDown = false;
	bool goUp = false;

	bool scored;
	void SetIsScored();
	bool GetIsScored();
	int getScore();

	//	void GoToMainMenuScene(Ref *sender);
	void GoToMainMenuScene(Ref *sender);
	void GoToStory2(Ref *sender);
	void GoToLoseScene(Ref *sender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
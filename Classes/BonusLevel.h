//#ifndef __BONUS_LEVEL_H__ // used so that hte header will only be used once per compile
#define __BONUS_LEVEL_H__

#include "cocos2d.h"// brining in cocos funcality 
#include "Box2D/Box2D.h"

using namespace cocos2d; // namespace for cococs so you dont have to use cocos2d:: every time 

class BonusLevel : public cocos2d::Layer
{

private:
	Sprite* _player;
	Label * scoreLabel;

	PhysicsWorld * sceneWorld2;
	void setPhysicsWorld(PhysicsWorld * world) { sceneWorld2 = world; };

public:

	static cocos2d::Scene* createScene();

	virtual bool init();// creating the bool init calss 

	void addCross(float dt);// creating crosses for the scene
	void addGarlic(float dt);// creating garlic for the scene
	void addStake(float dt);// creating garlic for the scene
	void addBlood(float dt);// creating blood for the scene
	
	float time;
	virtual void update(float delta);

	bool onTouchBegan(Touch * touch, Event *unused_event);

	bool onContactBegin(cocos2d::PhysicsContact &contact);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	cocos2d::Label *timer;

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
	void GoToLoseScene(Ref *sender);
	void GoToGameOverScene(Ref *sender);

	// implement the "static create()" method manually
	CREATE_FUNC(BonusLevel);
};

//#endif // __BONUS_LEVEL_H__

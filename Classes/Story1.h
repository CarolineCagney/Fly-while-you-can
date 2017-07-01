#include "cocos2d.h"// brining in cocos funcality 
#include "Box2D/Box2D.h"

using namespace cocos2d; // namespace for cococs so you dont have to use cocos2d:: every time 

class Story1 : public cocos2d::Layer
{

public:

	static cocos2d::Scene* createScene();

	virtual bool init();// creating the bool init calss 

	void GoToGameSceneScene(Ref *sender);

	// implement the "static create()" method manually
	CREATE_FUNC(Story1);
};
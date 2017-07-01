#ifndef __GameOverScene_H__
#define __GameOverScene_H__

#include "cocos2d.h"// brining in cocos funcality 
//#include "Box2D/Box2D.h"

using namespace cocos2d; // namespace for cococs so you dont have to use cocos2d:: every time 

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

private:

	void GoToMainMenuScene(Ref *sender);

#endif // __GameOverScene_H__

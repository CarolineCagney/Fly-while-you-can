#ifndef __LOSE_H__
#define __LOSE_H__

#include "cocos2d.h"

using namespace cocos2d;

class LoseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(LoseScene);

private:
	
	void GoToMainMenuScene(Ref *sender);
};

#endif // __LOSE_H__

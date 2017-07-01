#ifndef __StoryBook_H__
#define __StoryBook_H__

#include "cocos2d.h"

using namespace cocos2d;

class StoryBook : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(StoryBook);

private:
	
	void GoToMainMenuScene(Ref *sender);
};

#endif // __StoryBook_H__

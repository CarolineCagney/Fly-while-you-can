#ifndef __Story2_H__
#define __Story2_H__

#include "cocos2d.h"

using namespace cocos2d;

class Story2 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(Story2);

private:
	
	void GoToLevel2(Ref *sender);
};

#endif // __Story2_H__

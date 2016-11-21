#pragma once

#include "cocos2d.h"

class HelloWorld;

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool init(HelloWorld* pGame);

private:
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
};
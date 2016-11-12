#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
  
    if ( !Layer::init() )
    {
        return false;
    }
	touchInfo = Label::create();
	touchInfo->setPosition(100, 100);
	playerCharacter = Sprite::create();
	Sprite* characterSpriteSheet = Sprite::create("Assets/Graphics/Player/player-move.png");
	Vector<SpriteFrame*> wizardFrames;
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(0, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(64, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(128, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(192, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(256, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(320, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(384, 0, 64, 64)));
	wizardFrames.pushBack(SpriteFrame::createWithTexture(characterSpriteSheet->getTexture(), Rect(448, 0, 64, 64)));
	Animation* animation = Animation::createWithSpriteFrames(wizardFrames, 0.1);
	Animate* animate = Animate::create(animation);
	playerCharacter->runAction(RepeatForever::create(animate));
	playerCharacter->setAnchorPoint(Vec2(0.5, 0.5));
	playerCharacter->setPosition(200, 300);
	Camera* defaultCam = Camera::getDefaultCamera();
	defaultCam->setPosition(playerCharacter->getPosition());

	auto rootNode = CSLoader::createNode("MainScene.csb");
	rootNode->addChild(playerCharacter);
	addChild(rootNode);


	// Input init
	auto toucheListener = EventListenerTouchOneByOne::create();
	auto mouseListener = EventListenerMouse::create();
	toucheListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::OnTouchEnded, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::OnMouseDown, this);

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(toucheListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

void HelloWorld::OnTouchEnded(Touch* touch, Event* event)
{
	Vec2 touchPos = convertToWorldSpace(touch->getLocation());
	touchPos.y = this->getContentSize().height - touchPos.y;
	float distanceToTravel = playerCharacter->getPosition().distance(touchPos);
	auto move = MoveTo::create(distanceToTravel / 15, touchPos);
	playerCharacter->runAction(move);
}
void HelloWorld::OnMouseDown(cocos2d::Event* event)
{
	if (playerCharacter->numberOfRunningActions() < 5)
	{
		// Invert y because it is different between world space and screen space
		EventMouse* e = (EventMouse*)event;
		Vec2 clickPos = e->getLocation();
		clickPos.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - clickPos.y;
		
		// Move character
		Vec2 clickInWorld = convertToWorldSpace(clickPos);
		float distanceToTravel = playerCharacter->getPosition().distance(clickInWorld);
		auto move = MoveTo::create(distanceToTravel / 50, clickInWorld);
		float rotateAngle = Vec2::angle(GetSpriteHeading(playerCharacter),
			clickInWorld - playerCharacter->getPosition()) * 180/ 3.14159265359;
		playerCharacter->runAction(move);
		playerCharacter->setRotation(rotateAngle);

		// Move camera
		Camera* cam = Camera::getDefaultCamera();
		Action* moveCamera = MoveTo::create(distanceToTravel / 50, Vec3(clickInWorld.x, clickInWorld.y,
			cam->getPosition3D().z));
		cam->runAction(moveCamera);

		std::stringstream s;
		s << "Angle: " << rotateAngle;
		cocos2d::log(s.str().c_str());
	}
}

Vec2 HelloWorld::GetSpriteHeading(Sprite* sprite)
{
	float rotationRad = sprite->getRotation() * 3.14159265359 / 180;
	return Vec2(cos(rotationRad), sin(rotationRad));
}

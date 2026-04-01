#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
        return false;

    _setupBackground();
    _tableController.attach(this);

    return true;
}

void HelloWorld::_setupBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 全屏底色（TableDemoController 会再叠上棕/紫分区；此处避免未覆盖区域露黑）
    auto bg = LayerColor::create(Color4B(0xA6, 0x80, 0x50, 255),
                                 visibleSize.width, visibleSize.height);
    bg->setPosition(origin);
    addChild(bg, -11);

    // 关闭按钮
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    if (closeItem)
    {
        closeItem->setPosition(Vec2(origin.x + visibleSize.width
                                     - closeItem->getContentSize().width / 2,
                                     origin.y + closeItem->getContentSize().height / 2));
        auto menu = Menu::create(closeItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        addChild(menu, 10);
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

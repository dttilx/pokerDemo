#include "views/CardView.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

CardView* CardView::create(CardFaceType face, CardSuitType suit)
{
    return create(face, suit, 72.0f, 96.0f);
}

CardView* CardView::create(CardFaceType face, CardSuitType suit, float width, float height)
{
    auto* p = new (std::nothrow) CardView();
    if (p && p->init(face, suit, width, height))
    {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}

bool CardView::init(CardFaceType face, CardSuitType suit)
{
    return init(face, suit, 96.0f, 144.0f);
}

bool CardView::init(CardFaceType face, CardSuitType suit, float CARD_W, float CARD_H)
{
    if (!Node::init())
        return false;

    _face = face;
    _suit = suit;
    _tapCallback = nullptr;
    _touchListener = nullptr;

    setContentSize(Size(CARD_W, CARD_H));

    // 轻微投影，使牌面浮在背景上
    auto* shadow = LayerColor::create(Color4B(0, 0, 0, 28), CARD_W + 3.0f, CARD_H + 3.0f);
    shadow->setIgnoreAnchorPointForPosition(false);
    shadow->setAnchorPoint(Vec2(0.5f, 0.5f));
    shadow->setPosition(CARD_W * 0.5f - 1.5f, CARD_H * 0.5f - 1.5f);
    addChild(shadow, -1);

    // --- 1. 卡背底图 ---
    auto* bg = Sprite::create(_firstExistingPath({
        "res/card_general.png",
        "res/cards/card_general.png",
        "res/card/card_general.png"
    }));
    if (!bg)
        return false;
    const Size bgSize = bg->getContentSize();
    bg->setPosition(CARD_W * 0.5f, CARD_H * 0.5f);
    bg->setScaleX(CARD_W / bgSize.width);
    bg->setScaleY(CARD_H / bgSize.height);
    addChild(bg);

    // --- 2. 大数字图（显示在牌面中央） ---
    std::string numPath = _numberPath(face, suit);
    if (!numPath.empty())
    {
        auto* numSpr = Sprite::create(numPath);
        if (numSpr)
        {
            float targetH = CARD_H * 0.40f;
            float s = targetH / numSpr->getContentSize().height;
            numSpr->setScale(s);
            numSpr->setPosition(CARD_W * 0.5f, CARD_H * 0.47f);
            addChild(numSpr, 1);

            // 左上角小点数标记，贴近示例牌面的角标效果
            auto* cornerNum = Sprite::create(numPath);
            if (cornerNum)
            {
                const float cornerTargetH = CARD_H * 0.14f;
                const float cornerScale = cornerTargetH / cornerNum->getContentSize().height;
                cornerNum->setScale(cornerScale);
                cornerNum->setAnchorPoint(Vec2(0.0f, 1.0f));
                cornerNum->setPosition(CARD_W * 0.08f, CARD_H * 0.94f);
                addChild(cornerNum, 2);
            }
        }
    }

    // --- 3. 左上角小花色图标 ---
    std::string suitPath = _suitToPath(suit);
    if (!suitPath.empty())
    {
        auto* suitSpr = Sprite::create(suitPath);
        if (suitSpr)
        {
            float targetH = CARD_H * 0.085f;
            float s = targetH / suitSpr->getContentSize().height;
            suitSpr->setScale(s);
            // 放在左上角，与小点数组合为角标
            suitSpr->setAnchorPoint(Vec2(0.0f, 1.0f));
            suitSpr->setPosition(CARD_W * 0.27f, CARD_H * 0.915f);
            addChild(suitSpr, 2);
        }
    }

    return true;
}

CardFaceType CardView::getFace() const
{
    return _face;
}

CardSuitType CardView::getSuit() const
{
    return _suit;
}

void CardView::setTapCallback(const std::function<void(CardView*)>& callback)
{
    _tapCallback = callback;
    _registerTouch();
}

void CardView::_registerTouch()
{
    if (_touchListener)
    {
        getEventDispatcher()->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }

    _touchListener = EventListenerTouchOneByOne::create();
    EventListenerTouchOneByOne* lst = _touchListener;
    lst->setSwallowTouches(true);
    lst->onTouchBegan = [this](Touch* t, Event*) -> bool {
        Vec2 localPt = convertToNodeSpace(t->getLocation());
        Rect bb(0, 0, getContentSize().width, getContentSize().height);
        bool hit = bb.containsPoint(localPt);
        CCLOG("[CardView] touch began  face=%d suit=%d  localPt=(%.1f,%.1f)  hit=%d",
              (int)_face, (int)_suit, localPt.x, localPt.y, hit);
        return hit;
    };
    lst->onTouchEnded = [this](Touch* t, Event*) {
        Vec2 localPt = convertToNodeSpace(t->getLocation());
        Rect bb(0, 0, getContentSize().width, getContentSize().height);
        if (bb.containsPoint(localPt))
        {
            CCLOG("[CardView] tap fired  face=%d suit=%d", (int)_face, (int)_suit);
            if (_tapCallback)
                _tapCallback(this);
        }
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(lst, this);
}

std::string CardView::_faceToString(CardFaceType face)
{
    switch (face)
    {
    case CFT_ACE:   return "A";
    case CFT_TWO:   return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR:  return "4";
    case CFT_FIVE:  return "5";
    case CFT_SIX:   return "6";
    case CFT_SEVEN: return "7";
    case CFT_EIGHT: return "8";
    case CFT_NINE:  return "9";
    case CFT_TEN:   return "10";
    case CFT_JACK:  return "J";
    case CFT_QUEEN: return "Q";
    case CFT_KING:  return "K";
    default:        return "";
    }
}

std::string CardView::_suitToPath(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS:    return _firstExistingPath({"res/suits/club.png", "res/suits/clubs.png"});
    case CST_DIAMONDS: return _firstExistingPath({"res/suits/diamond.png", "res/suits/diamonds.png"});
    case CST_HEARTS:   return _firstExistingPath({"res/suits/heart.png", "res/suits/hearts.png"});
    case CST_SPADES:   return _firstExistingPath({"res/suits/spade.png", "res/suits/spades.png"});
    default:           return "";
    }
}

std::string CardView::_numberPath(CardFaceType face, CardSuitType suit)
{
    std::string key = _faceToString(face);
    if (key.empty())
        return "";

    bool red = _isRed(suit);
    if (red)
    {
        return _firstExistingPath({
            "res/number/big_red_" + key + ".png",
            "res/number/red_" + key + ".png"
        });
    }
    return _firstExistingPath({
        "res/number/big_black_" + key + ".png",
        "res/number/black_" + key + ".png"
    });
}

bool CardView::_isRed(CardSuitType suit)
{
    return suit == CST_HEARTS || suit == CST_DIAMONDS;
}

std::string CardView::_firstExistingPath(const std::vector<std::string>& candidates) const
{
    auto* fu = FileUtils::getInstance();
    for (size_t i = 0; i < candidates.size(); ++i)
    {
        if (fu->isFileExist(candidates[i]))
            return candidates[i];
    }
    return candidates.empty() ? std::string() : candidates[0];
}

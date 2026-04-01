#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <functional>
#include <vector>
#include "cocos2d.h"
#include "models/Card.h"

/**
 * @brief 卡牌视图组件。
 *
 * 负责将一张纸牌（CardFace + CardSuit）渲染为可视节点。
 * 由卡背底图、大数字、花色小图标组合拼成。
 * 放在 classes/views/ 目录下。
 */
class CardView : public cocos2d::Node
{
public:
    static CardView* create(CardFaceType face, CardSuitType suit);
    static CardView* create(CardFaceType face, CardSuitType suit, float width, float height);

    virtual bool init(CardFaceType face, CardSuitType suit);
    virtual bool init(CardFaceType face, CardSuitType suit, float width, float height);

    /**
     * @brief 获取当前视图展示的点数。
     */
    CardFaceType getFace() const;

    /**
     * @brief 获取当前视图展示的花色。
     */
    CardSuitType getSuit() const;

    /**
     * @brief 注册点击整张牌时的回调（在触摸结束且落点在牌范围内触发）。
     * @param callback 收到点击时调用，参数为 this。
     */
    void setTapCallback(const std::function<void(CardView*)>& callback);

private:
    /** 在候选路径中返回第一个存在的资源路径。 */
    std::string _firstExistingPath(const std::vector<std::string>& candidates) const;

    /** 根据点数枚举返回数字串（如 "A"、"K"、"10"）。 */
    std::string _faceToString(CardFaceType face);

    /** 根据花色枚举返回资源路径。 */
    std::string _suitToPath(CardSuitType suit);

    /** 根据点数和花色返回大字数字图路径。 */
    std::string _numberPath(CardFaceType face, CardSuitType suit);

    /** 判断是否为红色系花色（红桃/方块）。 */
    bool _isRed(CardSuitType suit);

    void _registerTouch();

    cocos2d::EventListenerTouchOneByOne* _touchListener;

    CardFaceType _face;
    CardSuitType _suit;
    std::function<void(CardView*)> _tapCallback;
};

#endif // CARDVIEW_H

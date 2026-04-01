#ifndef MYPOKER_CARD_H
#define MYPOKER_CARD_H

#include <string>

/**
 * @brief 表示一张基础纸牌的数据模型。
 *
 * 该类仅负责描述一张牌的静态属性，包括花色、点数以及是否为万能牌。
 * 它不关心牌在牌局中的位置、是否被翻开等运行时状态，这些由更高层的
 * 实例模型（例如 CardInstance）进行管理。
 * 此类适用于：构建牌堆、序列化存档、以及根据牌面计算匹配规则等纯数据操作。
 */

// 花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

class Card
{
public:
    /**
     * @brief 默认构造函数，创建一张空牌。
     *
     * 默认情况下花色与点数均为 NONE，且不是万能牌。
     * 通常用于反序列化或后续通过 setter 进行赋值。
     */
    Card();

    /**
     * @brief 使用指定花色、点数和万能标记创建一张牌。
     * @param suit 花色枚举值。
     * @param face 点数枚举值。
     * @param isWild 是否为万能牌，true 表示可以匹配任意底牌。
     */
    Card(CardSuitType suit, CardFaceType face, bool isWild);

    /**
     * @brief 获取当前牌的花色。
     * @return 花色枚举值。
     */
    CardSuitType getSuit() const;

    /**
     * @brief 获取当前牌的点数。
     * @return 点数枚举值。
     */
    CardFaceType getFace() const;

    /**
     * @brief 设置当前牌的花色。
     * @param suit 新的花色枚举值。
     */
    void setSuit(CardSuitType suit);

    /**
     * @brief 设置当前牌的点数。
     * @param face 新的点数枚举值。
     */
    void setFace(CardFaceType face);

    /**
     * @brief 获取当前牌是否为万能牌。
     * @return true 表示是万能牌，false 表示普通牌。
     */
    bool isWild() const;

    /**
     * @brief 设置当前牌是否为万能牌。
     * @param wild 是否为万能牌。
     */
    void setWild(bool wild);

    /**
     * @brief 获取用于匹配计算的点数字面值。
     *
     * 对于 A-K，会返回 1-13 这样的整数；对于 CFT_NONE 则返回 0。
     * 调用方可以据此进行差值为 1 的匹配判断。
     *
     * @return 整型点数值，范围通常为 [1, 13]，无效牌返回 0。
     */
    int getNumericValue() const;

private:
    /** @brief 牌的花色。 */
    CardSuitType _suit;

    /** @brief 牌的点数。 */
    CardFaceType _face;

    /** @brief 是否为万能牌。 */
    bool _isWild;
};

#endif // MYPOKER_CARD_H


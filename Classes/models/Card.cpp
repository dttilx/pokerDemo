#include "Card.h"

/**
 * @brief Card 类实现文件。
 *
 * 该文件提供 Card 数据模型的具体方法实现，包含花色、点数、
 * 万能标记的基础操作以及用于匹配计算的点数转换逻辑。
 */

Card::Card()
    : _suit(CST_NONE)
    , _face(CFT_NONE)
    , _isWild(false)
{
}

Card::Card(CardSuitType suit, CardFaceType face, bool isWild)
    : _suit(suit)
    , _face(face)
    , _isWild(isWild)
{
}

CardSuitType Card::getSuit() const
{
    return _suit;
}

CardFaceType Card::getFace() const
{
    return _face;
}

void Card::setSuit(CardSuitType suit)
{
    _suit = suit;
}

void Card::setFace(CardFaceType face)
{
    _face = face;
}

bool Card::isWild() const
{
    return _isWild;
}

void Card::setWild(bool wild)
{
    _isWild = wild;
}

int Card::getNumericValue() const
{
    const CardFaceType face = _face;
    if (face <= CFT_NONE || face >= CFT_NUM_CARD_FACE_TYPES)
    {
        return 0;
    }

    // CFT_ACE 为 0，CFT_KING 为 12；此处返回与枚举序一致的整型，供匹配逻辑使用。
    return static_cast<int>(face);
}


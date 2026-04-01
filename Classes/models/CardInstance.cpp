#include "CardInstance.h"

/**
 * @brief CardInstance 类实现文件。
 *
 * 提供牌实例在牌局中的运行时状态管理，包括所属区域、翻开状态
 * 以及唯一标识 id 的基础读写操作。
 */

CardInstance::CardInstance()
    : _card()
    , _faceUp(false)
    , _areaType(AreaType::Unset)
    , _id(0)
{
}

CardInstance::CardInstance(const Card& card, AreaType area, bool faceUp)
    : _card(card)
    , _faceUp(faceUp)
    , _areaType(area)
    , _id(0)
{
}

const Card& CardInstance::getCard() const
{
    return _card;
}

void CardInstance::setCard(const Card& card)
{
    _card = card;
}

bool CardInstance::isFaceUp() const
{
    return _faceUp;
}

void CardInstance::setFaceUp(bool faceUp)
{
    _faceUp = faceUp;
}

CardInstance::AreaType CardInstance::getAreaType() const
{
    return _areaType;
}

void CardInstance::setAreaType(AreaType area)
{
    _areaType = area;
}

std::int32_t CardInstance::getId() const
{
    return _id;
}

void CardInstance::setId(std::int32_t id)
{
    _id = id;
}


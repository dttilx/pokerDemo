#include "LevelConfig.h"

/**
 * @brief LevelConfig 与 LevelCardConfig 实现文件。
 *
 * 该文件实现了关卡静态配置的基本存取方法，用于在加载配置文件后
 * 将 JSON 数据映射为可供业务层使用的结构化对象。
 */

CardFaceType LevelCardConfig::getFace() const
{
    return _face;
}

void LevelCardConfig::setFace(CardFaceType face)
{
    _face = face;
}

CardSuitType LevelCardConfig::getSuit() const
{
    return _suit;
}

void LevelCardConfig::setSuit(CardSuitType suit)
{
    _suit = suit;
}

cocos2d::Vec2 LevelCardConfig::getPosition() const
{
    return _position;
}

void LevelCardConfig::setPosition(const cocos2d::Vec2& position)
{
    _position = position;
}

const std::vector<LevelCardConfig>& LevelConfig::getPlayfieldCards() const
{
    return _playfieldCards;
}

void LevelConfig::setPlayfieldCards(const std::vector<LevelCardConfig>& cards)
{
    _playfieldCards = cards;
}

const std::vector<LevelCardConfig>& LevelConfig::getStackCards() const
{
    return _stackCards;
}

void LevelConfig::setStackCards(const std::vector<LevelCardConfig>& cards)
{
    _stackCards = cards;
}


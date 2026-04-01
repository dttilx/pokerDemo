#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <vector>
#include "cocos2d.h"
#include "models/Card.h"

/**
 * @brief 单张牌的关卡静态配置。
 *
 * LevelCardConfig 用于描述一张牌在关卡中的初始状态，包括
 * 点数、花色以及在设计分辨率坐标系中的摆放位置。
 * 该类不包含任何运行时逻辑，仅作为 LevelConfig 的数据单元。
 */
class LevelCardConfig
{
public:
    /**
     * @brief 获取牌面的点数类型。
     * @return 点数枚举值。
     */
    CardFaceType getFace() const;

    /**
     * @brief 设置牌面的点数类型。
     * @param face 新的点数枚举值。
     */
    void setFace(CardFaceType face);

    /**
     * @brief 获取牌面的花色类型。
     * @return 花色枚举值。
     */
    CardSuitType getSuit() const;

    /**
     * @brief 设置牌面的花色类型。
     * @param suit 新的花色枚举值。
     */
    void setSuit(CardSuitType suit);

    /**
     * @brief 获取该牌在设计分辨率坐标系中的初始位置。
     * @return 位置向量，单位为像素。
     */
    cocos2d::Vec2 getPosition() const;

    /**
     * @brief 设置该牌在设计分辨率坐标系中的初始位置。
     * @param position 新的坐标位置。
     */
    void setPosition(const cocos2d::Vec2& position);

private:
    /** @brief 牌面点数。 */
    CardFaceType _face;

    /** @brief 牌面花色。 */
    CardSuitType _suit;

    /** @brief 牌在设计分辨率坐标系中的初始位置。 */
    cocos2d::Vec2 _position;
};

/**
 * @brief 描述单个关卡布局的静态配置。
 *
 * LevelConfig 保存主牌区（Playfield）和堆牌区（Stack）的
 * 纸牌初始布置信息，对应你提供的 JSON 配置结构。
 * 该类不负责解析文件，仅作为反序列化后的承载对象。
 */
class LevelConfig
{
public:
    /**
     * @brief 获取主牌区所有牌的配置列表。
     * @return 主牌区牌配置数组的只读引用。
     */
    const std::vector<LevelCardConfig>& getPlayfieldCards() const;

    /**
     * @brief 设置主牌区所有牌的配置列表。
     * @param cards 新的主牌区牌配置数组。
     */
    void setPlayfieldCards(const std::vector<LevelCardConfig>& cards);

    /**
     * @brief 获取堆牌区所有牌的配置列表。
     * @return 堆牌区牌配置数组的只读引用。
     */
    const std::vector<LevelCardConfig>& getStackCards() const;

    /**
     * @brief 设置堆牌区所有牌的配置列表。
     * @param cards 新的堆牌区牌配置数组。
     */
    void setStackCards(const std::vector<LevelCardConfig>& cards);

private:
    /** @brief 主牌区牌配置列表。 */
    std::vector<LevelCardConfig> _playfieldCards;

    /** @brief 堆牌区牌配置列表。 */
    std::vector<LevelCardConfig> _stackCards;
};

#endif // LEVELCONFIG_H


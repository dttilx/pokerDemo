#ifndef CARDINSTANCE_H
#define CARDINSTANCE_H

#include <cstdint>
#include "Card.h"

/**
 * @brief 表示一张牌在牌局中的实例状态。
 *
 * CardInstance 在 Card 的基础上增加了运行时属性，例如是否翻开、
 * 所属的逻辑区域（主牌区、底牌区、备用牌堆等）以及唯一标识 id。
 * 该类不负责业务逻辑，仅作为 GameState 中的基础数据单元，
 * 便于进行序列化、反序列化和简单状态查询。
 */
class CardInstance
{
public:
    /**
     * @brief 逻辑区域类型，用于区分牌当前所在的区域。
     *
     * 枚举名避免使用 None 等常见标识，以防在包含 Windows 头文件时被宏替换。
     */
    enum class AreaType : std::uint8_t
    {
        Unset = 0,
        PlayField,
        Tray,
        ReservePile
    };

    /**
     * @brief 默认构造函数，创建一张未翻开的空牌实例。
     */
    CardInstance();

    /**
     * @brief 使用指定基础牌与区域创建实例。
     * @param card 基础牌数据。
     * @param area 当前所属区域类型。
     * @param faceUp 是否已翻开。
     */
    CardInstance(const Card& card, AreaType area, bool faceUp);

    /**
     * @brief 获取该实例对应的基础牌。
     * @return const 引用，调用方不应直接修改。
     */
    const Card& getCard() const;

    /**
     * @brief 设置该实例对应的基础牌数据。
     * @param card 新的基础牌对象。
     */
    void setCard(const Card& card);

    /**
     * @brief 获取该牌是否已翻开。
     * @return true 表示翻开，false 表示背面朝上。
     */
    bool isFaceUp() const;

    /**
     * @brief 设置该牌是否翻开。
     * @param faceUp 是否翻开。
     */
    void setFaceUp(bool faceUp);

    /**
     * @brief 获取当前牌所在的逻辑区域。
     * @return 区域类型枚举值。
     */
    AreaType getAreaType() const;

    /**
     * @brief 设置当前牌所在的逻辑区域。
     * @param area 区域类型枚举值。
     */
    void setAreaType(AreaType area);

    /**
     * @brief 获取该实例的唯一标识 id。
     *
     * 唯一 id 可用于在视图层建立牌实例与 UI 节点之间的映射，
     * 便于在控制器中通过 id 查找对应的视图元素。
     *
     * @return 整型唯一 id。
     */
    std::int32_t getId() const;

    /**
     * @brief 设置该实例的唯一标识 id。
     * @param id 新的唯一 id。
     */
    void setId(std::int32_t id);

private:
    /** @brief 基础牌数据。 */
    Card _card;

    /** @brief 是否已翻开。 */
    bool _faceUp;

    /** @brief 当前所属逻辑区域。 */
    AreaType _areaType;

    /** @brief 实例唯一标识 id。 */
    std::int32_t _id;
};

#endif // CARDINSTANCE_H


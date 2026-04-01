#ifndef MATCHSERVICE_H
#define MATCHSERVICE_H

#include "models/Card.h"

/**
 * @brief 无状态匹配规则服务。
 *
 * 不持有数据，仅根据点数枚举判断桌面牌与手牌顶牌是否可按「差 1」规则匹配。
 */
class MatchService
{
public:
    /**
     * @brief 判断桌面牌点数与手牌顶牌点数之差的绝对值是否为 1（不含花色限制）。
     * @param tableFace 桌面被点的牌的点数。
     * @param handTopFace 当前手牌区顶牌的点数。
     * @return 可匹配时为 true。
     */
    static bool isAdjacentRank(CardFaceType tableFace, CardFaceType handTopFace);
};

#endif // MATCHSERVICE_H

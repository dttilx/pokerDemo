#include "MatchService.h"
#include <cstdlib>

bool MatchService::isAdjacentRank(CardFaceType tableFace, CardFaceType handTopFace)
{
    if (tableFace <= CFT_NONE || tableFace >= CFT_NUM_CARD_FACE_TYPES)
        return false;
    if (handTopFace <= CFT_NONE || handTopFace >= CFT_NUM_CARD_FACE_TYPES)
        return false;

    int a = static_cast<int>(tableFace);
    int b = static_cast<int>(handTopFace);
    // 普通线性相邻（2-3、Q-K 等）
    if (std::abs(a - b) == 1)
        return true;

    // 特殊规则：首尾相邻，A 和 K 也算一对
    const int minFace = static_cast<int>(CFT_ACE);
    const int maxFace = static_cast<int>(CFT_KING);
    bool isAandK =
        (a == minFace && b == maxFace) ||
        (a == maxFace && b == minFace);

    return isAandK;
}

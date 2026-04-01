#ifndef MYPOKER_UNDOMOVERECORD_H
#define MYPOKER_UNDOMOVERECORD_H

#include "cocos2d.h"

class CardView;

/**
 * @brief 描述一次「牌移动到顶牌位并替换顶牌」操作，用于回退恢复。
 *
 * 适配两类动作：
 * - 主牌（且可点击）移动到顶牌位，成为新顶牌；
 * - 备用牌首牌移动到顶牌位，成为新顶牌。
 */
enum class UndoMoveKind
{
    ReserveToTop,   ///< 备用牌首牌移动到顶牌位
    MainToTop       ///< 主牌移动到顶牌位
};

struct UndoMoveRecord
{
    UndoMoveKind kind;
    CardView* movedCard;
    CardView* prevTopCard;
    cocos2d::Vec2 movedFromPos;
    cocos2d::Vec2 topPos;
    int mainSlotIndex;
    int reserveInsertIndex;
};

#endif // MYPOKER_UNDOMOVERECORD_H

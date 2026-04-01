#ifndef TABLEDEMOCONTROLLER_H
#define TABLEDEMOCONTROLLER_H

#include <vector>
#include "cocos2d.h"
#include "managers/UndoManager.h"

// 前向声明：单张牌的 UI 视图组件。
class CardView;

namespace cocos2d
{
class Label;
}

/**
 * @brief 主界面牌桌的业务控制器。
 *
 * 该控制器持有并管理一局游戏中与“牌桌”相关的全部状态：
 * - 创建并布局主牌区 / 备用牌区 / 顶牌区域的 CardView；
 * - 根据 MatchService 的匹配规则处理牌的点击与移动；
 * - 通过 UndoManager 记录每一步操作并实现“回退”功能；
 * - 管理简单的 UI 状态（如 “NO” 提示和回退按钮可见性）。
 *
 * 控制器本身不负责具体牌面渲染细节，由 views 层的 CardView 负责；
 * 也不负责持久化数据，仅在一局游戏生命周期内存在。
 */
class TableDemoController
{
public:
    /**
     * @brief 将控制器挂接到指定场景并完成一次性初始化。
     *
     * 调用后将依次：
     * - 创建背景色块与边框；
     * - 随机发牌并生成主牌区、备用牌区以及顶牌；
     * - 绑定卡牌点击和回退按钮事件。
     *
     * @param scene 有效的场景指针，作为所有 UI 节点的父节点。
     */
    void attach(cocos2d::Scene* scene);

private:
    /// 主牌区中每一个“槽位”的信息（位置 + 当前占用的牌）。
    struct MainSlot
    {
        cocos2d::Vec2 pos;
        CardView* card;
        int row;
        int col;
    };

    /// 创建棕色主牌区、紫色底部区域以及灰色边框。
    void _createBackground();

    /// 随机生成一局牌，并创建主牌 / 备用牌 / 顶牌的视图对象。
    void _createLayout();

    /// 创建右下角“回退”文本按钮并接入触摸事件。
    void _setupUndoButton();

    /// 为单张牌视图绑定点击回调，统一转发至 _onCardTapped。
    void _bindCardTap(CardView* card);

    /// 牌被点击时的统一入口，根据来源分发到备用牌或主牌逻辑。
    void _onCardTapped(CardView* card);

    /// 按当前备用牌顺序重新排布底部左侧“牌堆”。
    void _layoutReserveCards();

    /// 根据当前主牌状态重新计算主牌区两簇斜排的位置。
    void _layoutMainCards();

    /// 将丢弃的顶牌移动到隐藏位置，避免干扰点击。
    void _layoutDiscardCards();

    /// 在备用牌数组中查找指定牌的下标，找不到返回 -1。
    int _findReserveIndex(CardView* card) const;

    /// 在主牌槽位数组中查找指定牌所在槽位的下标，找不到返回 -1。
    int _findMainSlotIndex(CardView* card) const;

    /// 判断给定主牌槽位上的牌是否完全暴露（不被其他牌遮挡）。
    bool _isMainCardUncovered(int slotIndex) const;

    /// 处理“从备用牌堆翻到顶牌”的尝试动作。
    void _tryReserveToTop(CardView* reserveCard);

    /// 处理“从主牌区移动到顶牌”的尝试动作。
    void _tryMainToTop(int mainSlotIndex);

    /// 将指定牌移动到顶牌位置，并记录撤销信息。
    void _moveCardToTop(CardView* movedCard, UndoMoveKind kind, int mainSlotIndex, int reserveInsertIndex);

    /// “回退”按钮点击时的回调，执行一步撤销操作。
    void _onUndoClicked(cocos2d::Ref* sender);

    /// 设置控制器当前是否处于动画执行中，防止多次点击。
    void _setBusy(bool busy);

    /// 在界面中央短暂显示 “NO” 提示，表示当前操作不合法。
    void _showNoHint();

    /// 根据撤销栈状态更新“回退”按钮的可见性。
    void _updateUndoLabel();

    cocos2d::Scene* _scene;
    cocos2d::Label* _noHintLabel;
    cocos2d::Label* _undoLabel;
    cocos2d::EventListenerTouchOneByOne* _undoTouchListener;
    UndoManager _undoManager;
    bool _busy;

    CardView* _topCard;
    std::vector<CardView*> _reserveCards;
    std::vector<CardView*> _discardCards;
    cocos2d::Vec2 _topPos;
    cocos2d::Vec2 _reserveBasePos;
    cocos2d::Vec2 _discardHidePos;
    std::vector<MainSlot> _mainSlots;
};

#endif // TABLEDEMOCONTROLLER_H

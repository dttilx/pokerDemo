#include "TableDemoController.h"

#include <algorithm>
#include <random>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "cocos2d.h"
#include "models/UndoMoveRecord.h"
#include "services/MatchService.h"
#include "views/CardView.h"

USING_NS_CC;

namespace
{
    const float kMoveDuration = 0.22f;
    const int   kMainCardCount = 20;    // 主牌区改为 14 张，左右各 7 张可严格对称
    const int   kReserveCardCount = 5;
    // 基础牌尺寸为 72x96，这里整体放大 2 倍
    const float kCardWidth = 144.0f;
    const float kCardHeight = 192.0f;
}

void TableDemoController::attach(Scene* scene)
{
    _scene = scene;
    _busy = false;
    _noHintLabel = nullptr;
    _undoLabel = nullptr;
    _undoTouchListener = nullptr;
    _topCard = nullptr;
    _reserveCards.clear();
    _discardCards.clear();
    _mainSlots.clear();
    _undoManager.clear();

    _createBackground();
    _createLayout();
    _setupUndoButton();
}

void TableDemoController::_createBackground()
{
    const Size  sz = _scene->getContentSize();
    const float W  = static_cast<float>(sz.width);
    const float H  = static_cast<float>(sz.height);

    const float kPanelH = H * 0.28f;
    const float kBoardH = H * 0.72f;

    const Color4B kBrown(0xA6, 0x80, 0x50, 255);
    const Color4B kPurple(0x8E, 0x3A, 0x8E, 255);
    const Color4B kBorder(0x55, 0x55, 0x55, 255);

    auto* brownLayer = LayerColor::create(kBrown, W, kBoardH);
    brownLayer->setPosition(0.0f, kPanelH);
    _scene->addChild(brownLayer, -10);

    auto* purpleLayer = LayerColor::create(kPurple, W, kPanelH);
    purpleLayer->setPosition(0.0f, 0.0f);
    _scene->addChild(purpleLayer, -10);

    const float t = 3.0f;
    auto* bTop = LayerColor::create(kBorder, W, t);
    bTop->setPosition(0.0f, H - t);
    _scene->addChild(bTop, -9);
    auto* bBot = LayerColor::create(kBorder, W, t);
    bBot->setPosition(0.0f, 0.0f);
    _scene->addChild(bBot, -9);
    auto* bLeft = LayerColor::create(kBorder, t, H);
    bLeft->setPosition(0.0f, 0.0f);
    _scene->addChild(bLeft, -9);
    auto* bRight = LayerColor::create(kBorder, t, H);
    bRight->setPosition(W - t, 0.0f);
    _scene->addChild(bRight, -9);
}

void TableDemoController::_setupUndoButton()
{
    const Size  sz      = _scene->getContentSize();
    const float W       = static_cast<float>(sz.width);
    const float H       = static_cast<float>(sz.height);
    const float kPanelH = H * 0.28f;

    // 回退按钮使用系统中文字体，避免 TTF 缺字导致乱码/方框
    _undoLabel = Label::createWithSystemFont(u8"回退", "Microsoft YaHei", 34);
    _undoLabel->setTextColor(Color4B::WHITE);
    _undoLabel->enableOutline(Color4B::BLACK, 1);
    _undoLabel->setPosition(Vec2(W * 0.87f, kPanelH * 0.33f));
    _undoLabel->setVisible(false);
    _scene->addChild(_undoLabel, 30);

    auto* lst = EventListenerTouchOneByOne::create();
    lst->setSwallowTouches(true);
    lst->onTouchBegan = [this](Touch* t, Event*) -> bool {
        Vec2 localPt = _undoLabel->convertToNodeSpace(t->getLocation());
        Rect bb(0, 0, _undoLabel->getContentSize().width, _undoLabel->getContentSize().height);
        return bb.containsPoint(localPt);
    };
    lst->onTouchEnded = [this](Touch* t, Event*) {
        Vec2 localPt = _undoLabel->convertToNodeSpace(t->getLocation());
        Rect bb(0, 0, _undoLabel->getContentSize().width, _undoLabel->getContentSize().height);
        if (bb.containsPoint(localPt))
            _onUndoClicked(nullptr);
    };
    _undoTouchListener = lst;
    _scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lst, _undoLabel);
}

void TableDemoController::_createLayout()
{
    const Size  sz = _scene->getContentSize();
    const float W  = static_cast<float>(sz.width);
    const float H  = static_cast<float>(sz.height);

    const float kPanelH = H * 0.28f;

    _discardHidePos = Vec2(-W * 2.0f, -H * 2.0f);

    // 紫色面板中的位置：顶牌在备用牌堆右侧，形成“抽牌后替换顶牌”的视觉关系
    const float kPanelY = kPanelH * 0.5f;
    _reserveBasePos = Vec2(W * 0.26f, kPanelY * 0.92f);
    _topPos         = Vec2(W * 0.46f, kPanelY * 0.92f);

    // 创建随机牌组
    std::vector<std::pair<CardFaceType, CardSuitType>> allCards;
    for (int f = CFT_ACE; f <= CFT_KING; ++f)
    {
        for (int s = CST_CLUBS; s <= CST_SPADES; ++s)
        {
            allCards.push_back({static_cast<CardFaceType>(f), static_cast<CardSuitType>(s)});
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allCards.begin(), allCards.end(), g);

    // 初始化主牌槽位（14 张：左右各 7 张，用于中轴镜像布局）
    for (int i = 0; i < kMainCardCount; ++i)
    {
        MainSlot s;
        s.row = (i < (kMainCardCount / 2)) ? i : (i - kMainCardCount / 2);
        s.col = (i < (kMainCardCount / 2)) ? 0 : 1;
        s.card = nullptr;
        _mainSlots.push_back(s);
    }

    // 创建备用牌 - 使用放大尺寸
    for (int i = 0; i < kReserveCardCount; ++i)
    {
        CardFaceType face = allCards[i].first;
        CardSuitType suit = allCards[i].second;
        _reserveCards.push_back(CardView::create(face, suit, kCardWidth, kCardHeight));
    }

    // 创建顶牌
    if (kReserveCardCount < static_cast<int>(allCards.size()))
    {
        CardFaceType face = allCards[kReserveCardCount].first;
        CardSuitType suit = allCards[kReserveCardCount].second;
        _topCard = CardView::create(face, suit, kCardWidth, kCardHeight);
        _topCard->setPosition(_topPos);
        _topCard->setLocalZOrder(100);
        _scene->addChild(_topCard, 100);
    }

    // 创建主牌 - 使用放大尺寸
    for (int i = 0; i < kMainCardCount; ++i)
    {
        int cardIdx = kReserveCardCount + 1 + i;
        if (cardIdx < static_cast<int>(allCards.size()))
        {
            CardFaceType face = allCards[cardIdx].first;
            CardSuitType suit = allCards[cardIdx].second;
            _mainSlots[i].card = CardView::create(face, suit, kCardWidth, kCardHeight);
            _scene->addChild(_mainSlots[i].card, 10);
            _bindCardTap(_mainSlots[i].card);
        }
    }

    // 添加备用牌到场景
    for (CardView* c : _reserveCards)
    {
        _scene->addChild(c, 50);
        _bindCardTap(c);
    }

    _layoutReserveCards();
    _layoutMainCards();
}

void TableDemoController::_layoutReserveCards()
{
    // 备用牌采用水平方向叠放，所有牌保持同一条 y 轴
    const Size sz = _scene->getContentSize();
    const float W  = static_cast<float>(sz.width);
    const float H  = static_cast<float>(sz.height);
    const float kPanelH = H * 0.28f;
    const float kPanelY = kPanelH * 0.5f;

    _reserveBasePos = Vec2(W * 0.26f, kPanelY * 0.92f);

    // index 越大越靠右，形成横向叠牌效果（仅在 x 方向叠放）
    const float overlapX = kCardWidth * 0.12f;
    const float overlapY = kCardHeight * 0.045f; // 目前不再使用，仅保留以便后续需要垂直偏移时调整
    for (size_t i = 0; i < _reserveCards.size(); ++i)
    {
        CardView* c = _reserveCards[i];
        c->stopAllActions();
        float depth = static_cast<float>(_reserveCards.size() - 1 - i);
        float targetX = _reserveBasePos.x - depth * overlapX;
        float targetY = _reserveBasePos.y; // 所有备用牌共用同一条 y 轴
        c->setPosition(Vec2(targetX, targetY));
        c->setLocalZOrder(static_cast<int>(50 + i));
    }
}

void TableDemoController::_layoutMainCards()
{
    const Size  sz = _scene->getContentSize();
    const float W  = static_cast<float>(sz.width);
    const float H  = static_cast<float>(sz.height);
    const float kPanelH = H * 0.28f;
    const float kBoardH = H * 0.72f;

    // 主牌区改为“牌不旋转，仅通过 x/y 形成斜排”
    const float centerX = W * 0.5f;
    const float groupHalfGap = W * 0.22f;
    const float leftX = centerX - groupHalfGap;
    const float rightX = centerX + groupHalfGap;
    const float topY = kPanelH + kBoardH * 0.82f;
    const float stepY = kCardHeight * 0.34f;
    const float slopeX = kCardWidth * 0.09f;
    const float shiftX = W / 12.0f;
    const int totalSlots = static_cast<int>(_mainSlots.size());
    const int leftCount = totalSlots / 2;

    for (int slotIdx = 0; slotIdx < totalSlots; ++slotIdx)
    {
        bool isLeftGroup = slotIdx < leftCount;
        int localIdx = isLeftGroup ? slotIdx : (slotIdx - leftCount);

        float x = isLeftGroup
            ? (leftX + static_cast<float>(localIdx) * slopeX)
            : (rightX - static_cast<float>(localIdx) * slopeX);
        x -= shiftX;
        float y = topY - static_cast<float>(localIdx) * stepY;

        _mainSlots[slotIdx].pos = Vec2(x, y);

        if (_mainSlots[slotIdx].card)
        {
            _mainSlots[slotIdx].card->setPosition(_mainSlots[slotIdx].pos);
            _mainSlots[slotIdx].card->setLocalZOrder(10 + localIdx);
            _mainSlots[slotIdx].card->setRotation(0.0f);
        }
    }

    if (_topCard)
        _topCard->setLocalZOrder(100);
}

void TableDemoController::_bindCardTap(CardView* card)
{
    card->setTapCallback([this](CardView* tapped) { _onCardTapped(tapped); });
}

int TableDemoController::_findReserveIndex(CardView* card) const
{
    for (int i = 0; i < static_cast<int>(_reserveCards.size()); ++i)
    {
        if (_reserveCards[i] == card)
            return i;
    }
    return -1;
}

int TableDemoController::_findMainSlotIndex(CardView* card) const
{
    for (int i = 0; i < static_cast<int>(_mainSlots.size()); ++i)
    {
        if (_mainSlots[i].card == card)
            return i;
    }
    return -1;
}

void TableDemoController::_onCardTapped(CardView* card)
{
    if (_busy)
        return;
    if (card == _topCard)
        return;

    const int reserveIndex = _findReserveIndex(card);
    if (reserveIndex >= 0)
    {
        // 备用牌只有最上面那张（index 最大的）可以点击
        if (reserveIndex != static_cast<int>(_reserveCards.size()) - 1)
            return;
        _tryReserveToTop(card);
        return;
    }

    const int mainIndex = _findMainSlotIndex(card);
    if (mainIndex >= 0)
    {
        _tryMainToTop(mainIndex);  
    }
}

void TableDemoController::_tryReserveToTop(CardView* reserveCard)
{
    const int idx = _findReserveIndex(reserveCard);
    _moveCardToTop(reserveCard, UndoMoveKind::ReserveToTop, -1, idx);
}

void TableDemoController::_tryMainToTop(int mainSlotIndex)
{
    MainSlot& slot = _mainSlots[mainSlotIndex];
    if (slot.card == nullptr)
        return;

    if (!MatchService::isAdjacentRank(slot.card->getFace(), _topCard->getFace()))
    {
        _showNoHint();
        return;
    }

    _moveCardToTop(slot.card, UndoMoveKind::MainToTop, mainSlotIndex, -1);
}

void TableDemoController::_moveCardToTop(CardView* movedCard,
                                         UndoMoveKind  kind,
                                         int           mainSlotIndex,
                                         int           reserveInsertIndex)
{
    UndoMoveRecord rec;
    rec.kind               = kind;
    rec.movedCard          = movedCard;
    rec.prevTopCard       = _topCard;
    rec.movedFromPos      = movedCard->getPosition();
    rec.topPos            = _topPos;
    rec.mainSlotIndex     = mainSlotIndex;
    rec.reserveInsertIndex = reserveInsertIndex;
    _undoManager.pushRecord(rec);

    _setBusy(true);

    if (kind == UndoMoveKind::MainToTop && mainSlotIndex >= 0)
    {
        _mainSlots[mainSlotIndex].card = nullptr;
    }
    else if (kind == UndoMoveKind::ReserveToTop)
    {
        auto it = std::find(_reserveCards.begin(), _reserveCards.end(), movedCard);
        if (it != _reserveCards.end())
            _reserveCards.erase(it);
        _layoutReserveCards();
    }

    _discardCards.push_back(_topCard);
    for (CardView* c : _discardCards)
    {
        c->stopAllActions();
        c->setPosition(_discardHidePos);
        c->setVisible(false);
        c->setLocalZOrder(-100);
    }

    movedCard->runAction(Sequence::create(
        MoveTo::create(kMoveDuration, _topPos),
        CallFunc::create([this]() {
            _layoutMainCards();
            _setBusy(false);
            _updateUndoLabel();
        }),
        nullptr));

    _topCard = movedCard;
}

void TableDemoController::_onUndoClicked(Ref*)
{
    if (_busy)
        return;

    UndoMoveRecord rec;
    if (!_undoManager.popRecord(rec))
        return;

    _setBusy(true);

    _topCard->runAction(Sequence::create(
        MoveTo::create(kMoveDuration, rec.movedFromPos),
        nullptr));

    if (rec.kind == UndoMoveKind::MainToTop && rec.mainSlotIndex >= 0)
    {
        _mainSlots[rec.mainSlotIndex].card = rec.movedCard;
    }
    else if (rec.kind == UndoMoveKind::ReserveToTop)
    {
        int idx = rec.reserveInsertIndex;
        if (idx < 0) idx = 0;
        if (idx > static_cast<int>(_reserveCards.size()))
            idx = static_cast<int>(_reserveCards.size());
        _reserveCards.insert(_reserveCards.begin() + idx, rec.movedCard);
    }

    rec.prevTopCard->stopAllActions();
    rec.prevTopCard->setVisible(true);
    rec.prevTopCard->setOpacity(255);
    rec.prevTopCard->setPosition(_discardHidePos);
    rec.prevTopCard->runAction(MoveTo::create(kMoveDuration, _topPos));

    if (!_discardCards.empty() && _discardCards.back() == rec.prevTopCard)
        _discardCards.pop_back();
    else
    {
        for (auto it = _discardCards.begin(); it != _discardCards.end(); ++it)
        {
            if (*it == rec.prevTopCard)
            {
                _discardCards.erase(it);
                break;
            }
        }
    }

    _topCard = rec.prevTopCard;

    rec.movedCard->runAction(Sequence::create(
        DelayTime::create(kMoveDuration),
        CallFunc::create([this]() {
            _layoutMainCards();
            _layoutReserveCards();
            _setBusy(false);
            _updateUndoLabel();
        }),
        nullptr));
}

void TableDemoController::_setBusy(bool busy)
{
    _busy = busy;
}

void TableDemoController::_layoutDiscardCards()
{
    for (CardView* c : _discardCards)
    {
        c->stopAllActions();
        c->setPosition(_discardHidePos);
        c->setVisible(false);
        c->setLocalZOrder(-100);
    }
}

void TableDemoController::_showNoHint()
{
    const Size sz = _scene->getContentSize();
    if (_noHintLabel == nullptr)
    {
        TTFConfig ttfConfig;
        ttfConfig.fontFilePath = "fonts/Marker Felt.ttf";
        ttfConfig.fontSize = 40;
        ttfConfig.glyphs = GlyphCollection::DYNAMIC;
        ttfConfig.outlineSize = 2;

        _noHintLabel = Label::createWithTTF(ttfConfig, "NO");
        _noHintLabel->setTextColor(Color4B(255, 100, 100, 255));
        _noHintLabel->enableOutline(Color4B::BLACK, 2);
        _noHintLabel->setPosition(Vec2(sz.width * 0.5f, sz.height * 0.55f));
        _scene->addChild(_noHintLabel, 200);
    }
    _noHintLabel->stopAllActions();
    _noHintLabel->setOpacity(255);
    _noHintLabel->setVisible(true);
    _noHintLabel->runAction(Sequence::create(
        DelayTime::create(0.5f),
        FadeOut::create(0.15f),
        Hide::create(),
        nullptr));
}

void TableDemoController::_updateUndoLabel()
{
    if (_undoLabel)
    {
        _undoLabel->setVisible(_undoManager.canUndo());
    }
}

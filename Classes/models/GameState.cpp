#include "GameState.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

/**
 * @brief GameState 类实现文件。
 *
 * 提供完整牌局状态的数据存取与 JSON 序列化、反序列化能力。
 * JSON 结构中会包含版本号、主牌区、底牌区以及备用牌堆等信息，
 * 以支持游戏存档与读档功能。
 */

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::StringBuffer;
using rapidjson::Writer;

namespace
{
    /** @brief 当前存档格式版本号。 */
    const int kCurrentSaveVersion = 1;

    /** @brief 用于 JSON 字段名的常量。 */
    const char kFieldSaveVersion[] = "saveVersion";
    const char kFieldMainPiles[] = "mainPiles";
    const char kFieldTrayCard[] = "trayCard";
    const char kFieldReserveDeck[] = "reserveDeck";
    const char kFieldSuit[] = "suit";
    const char kFieldFace[] = "face";
    const char kFieldIsWild[] = "isWild";
    const char kFieldFaceUp[] = "faceUp";
}

GameState::GameState()
    : _saveVersion(kCurrentSaveVersion)
    , _mainPiles()
    , _trayCard()
    , _reserveDeck()
{
}

const std::vector<std::vector<CardInstance>>& GameState::getMainPiles() const
{
    return _mainPiles;
}

void GameState::setMainPiles(const std::vector<std::vector<CardInstance>>& piles)
{
    _mainPiles = piles;
}

const CardInstance& GameState::getTrayCard() const
{
    return _trayCard;
}

void GameState::setTrayCard(const CardInstance& card)
{
    _trayCard = card;
}

const std::vector<CardInstance>& GameState::getReserveDeck() const
{
    return _reserveDeck;
}

void GameState::setReserveDeck(const std::vector<CardInstance>& deck)
{
    _reserveDeck = deck;
}

int GameState::getSaveVersion() const
{
    return _saveVersion;
}

void GameState::setSaveVersion(int version)
{
    _saveVersion = version;
}

namespace
{
    Value makeCardJson(const CardInstance& instance, Document::AllocatorType& allocator)
    {
        Value obj(rapidjson::kObjectType);

        const Card& card = instance.getCard();
        obj.AddMember(kFieldSuit, static_cast<int>(card.getSuit()), allocator);
        obj.AddMember(kFieldFace, static_cast<int>(card.getFace()), allocator);
        obj.AddMember(kFieldIsWild, card.isWild(), allocator);
        obj.AddMember(kFieldFaceUp, instance.isFaceUp(), allocator);

        return obj;
    }

    bool parseCardJson(const Value& value, CardInstance& outInstance)
    {
        if (!value.IsObject())
        {
            return false;
        }

        if (!value.HasMember(kFieldSuit) || !value.HasMember(kFieldFace) ||
            !value.HasMember(kFieldIsWild) || !value.HasMember(kFieldFaceUp))
        {
            return false;
        }

        int suitInt = value[kFieldSuit].GetInt();
        int faceInt = value[kFieldFace].GetInt();
        bool isWild = value[kFieldIsWild].GetBool();
        bool faceUp = value[kFieldFaceUp].GetBool();

        CardSuitType suit = static_cast<CardSuitType>(suitInt);
        CardFaceType face = static_cast<CardFaceType>(faceInt);

        Card card(suit, face, isWild);

        outInstance.setCard(card);
        outInstance.setFaceUp(faceUp);

        return true;
    }
}

std::string GameState::serializeToJson() const
{
    Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    doc.AddMember(kFieldSaveVersion, _saveVersion, allocator);

    // mainPiles
    Value pilesArray(rapidjson::kArrayType);
    for (const auto& pile : _mainPiles)
    {
        Value pileArray(rapidjson::kArrayType);
        for (const auto& instance : pile)
        {
            Value cardValue = makeCardJson(instance, allocator);
            pileArray.PushBack(cardValue, allocator);
        }
        pilesArray.PushBack(pileArray, allocator);
    }
    doc.AddMember(kFieldMainPiles, pilesArray, allocator);

    // trayCard
    Value trayValue = makeCardJson(_trayCard, allocator);
    doc.AddMember(kFieldTrayCard, trayValue, allocator);

    // reserveDeck
    Value reserveArray(rapidjson::kArrayType);
    for (const auto& instance : _reserveDeck)
    {
        Value cardValue = makeCardJson(instance, allocator);
        reserveArray.PushBack(cardValue, allocator);
    }
    doc.AddMember(kFieldReserveDeck, reserveArray, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

bool GameState::deserializeFromJson(const std::string& jsonString)
{
    Document doc;
    if (doc.Parse(jsonString.c_str()).HasParseError() || !doc.IsObject())
    {
        return false;
    }

    if (!doc.HasMember(kFieldSaveVersion) ||
        !doc.HasMember(kFieldMainPiles) ||
        !doc.HasMember(kFieldTrayCard) ||
        !doc.HasMember(kFieldReserveDeck))
    {
        return false;
    }

    _saveVersion = doc[kFieldSaveVersion].GetInt();

    // mainPiles
    const Value& pilesArray = doc[kFieldMainPiles];
    if (!pilesArray.IsArray())
    {
        return false;
    }

    std::vector<std::vector<CardInstance>> newPiles;
    newPiles.reserve(pilesArray.Size());

    for (rapidjson::SizeType i = 0; i < pilesArray.Size(); ++i)
    {
        const Value& pileArray = pilesArray[i];
        if (!pileArray.IsArray())
        {
            return false;
        }

        std::vector<CardInstance> pile;
        pile.reserve(pileArray.Size());

        for (rapidjson::SizeType j = 0; j < pileArray.Size(); ++j)
        {
            CardInstance instance;
            if (!parseCardJson(pileArray[j], instance))
            {
                return false;
            }
            pile.push_back(instance);
        }

        newPiles.push_back(pile);
    }

    // trayCard
    CardInstance trayInstance;
    if (!parseCardJson(doc[kFieldTrayCard], trayInstance))
    {
        return false;
    }

    // reserveDeck
    const Value& reserveArray = doc[kFieldReserveDeck];
    if (!reserveArray.IsArray())
    {
        return false;
    }

    std::vector<CardInstance> reserveDeck;
    reserveDeck.reserve(reserveArray.Size());

    for (rapidjson::SizeType i = 0; i < reserveArray.Size(); ++i)
    {
        CardInstance instance;
        if (!parseCardJson(reserveArray[i], instance))
        {
            return false;
        }
        reserveDeck.push_back(instance);
    }

    _mainPiles = newPiles;
    _trayCard = trayInstance;
    _reserveDeck = reserveDeck;

    return true;
}


#include "LevelConfigService.h"

#include "cocos2d.h"
#include "external/json/document.h"

using rapidjson::Document;
using rapidjson::Value;

namespace
{
    const char kFieldPlayfield[] = "Playfield";
    const char kFieldStack[] = "Stack";
    const char kFieldCardFace[] = "CardFace";
    const char kFieldCardSuit[] = "CardSuit";
    const char kFieldPosition[] = "Position";
    const char kFieldX[] = "x";
    const char kFieldY[] = "y";

    bool tryParseFace(int faceValue, CardFaceType& outFace)
    {
        if (faceValue < static_cast<int>(CFT_ACE) || faceValue >= static_cast<int>(CFT_NUM_CARD_FACE_TYPES))
        {
            return false;
        }
        outFace = static_cast<CardFaceType>(faceValue);
        return true;
    }

    bool tryParseSuit(int suitValue, CardSuitType& outSuit)
    {
        if (suitValue < static_cast<int>(CST_CLUBS) || suitValue >= static_cast<int>(CST_NUM_CARD_SUIT_TYPES))
        {
            return false;
        }
        outSuit = static_cast<CardSuitType>(suitValue);
        return true;
    }

    bool tryParsePosition(const Value& value, cocos2d::Vec2& outPosition)
    {
        if (!value.IsObject() || !value.HasMember(kFieldX) || !value.HasMember(kFieldY))
        {
            return false;
        }

        if (!value[kFieldX].IsNumber() || !value[kFieldY].IsNumber())
        {
            return false;
        }

        outPosition.x = value[kFieldX].GetFloat();
        outPosition.y = value[kFieldY].GetFloat();
        return true;
    }

    bool tryParseCardConfig(const Value& value, LevelCardConfig& outCardConfig)
    {
        if (!value.IsObject())
        {
            return false;
        }

        if (!value.HasMember(kFieldCardFace) || !value.HasMember(kFieldCardSuit) || !value.HasMember(kFieldPosition))
        {
            return false;
        }

        if (!value[kFieldCardFace].IsInt() || !value[kFieldCardSuit].IsInt())
        {
            return false;
        }

        CardFaceType face = CFT_NONE;
        CardSuitType suit = CST_NONE;
        if (!tryParseFace(value[kFieldCardFace].GetInt(), face))
        {
            return false;
        }

        if (!tryParseSuit(value[kFieldCardSuit].GetInt(), suit))
        {
            return false;
        }

        cocos2d::Vec2 position;
        if (!tryParsePosition(value[kFieldPosition], position))
        {
            return false;
        }

        outCardConfig.setFace(face);
        outCardConfig.setSuit(suit);
        outCardConfig.setPosition(position);
        return true;
    }

    bool tryParseCardConfigArray(const Value& value, std::vector<LevelCardConfig>& outCards)
    {
        if (!value.IsArray())
        {
            return false;
        }

        std::vector<LevelCardConfig> cards;
        cards.reserve(value.Size());

        for (rapidjson::SizeType i = 0; i < value.Size(); ++i)
        {
            LevelCardConfig cardConfig;
            if (!tryParseCardConfig(value[i], cardConfig))
            {
                return false;
            }
            cards.push_back(cardConfig);
        }

        outCards = cards;
        return true;
    }
}

bool LevelConfigService::parseFromJsonString(const std::string& jsonString, LevelConfig& outConfig)
{
    Document doc;
    if (doc.Parse(jsonString.c_str()).HasParseError() || !doc.IsObject())
    {
        return false;
    }

    if (!doc.HasMember(kFieldPlayfield) || !doc.HasMember(kFieldStack))
    {
        return false;
    }

    std::vector<LevelCardConfig> playfieldCards;
    if (!tryParseCardConfigArray(doc[kFieldPlayfield], playfieldCards))
    {
        return false;
    }

    std::vector<LevelCardConfig> stackCards;
    if (!tryParseCardConfigArray(doc[kFieldStack], stackCards))
    {
        return false;
    }

    outConfig.setPlayfieldCards(playfieldCards);
    outConfig.setStackCards(stackCards);
    return true;
}

bool LevelConfigService::loadFromFile(const std::string& fullPath, LevelConfig& outConfig)
{
    std::string fileContent = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    if (fileContent.empty())
    {
        return false;
    }
    return parseFromJsonString(fileContent, outConfig);
}

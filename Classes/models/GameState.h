#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include "CardInstance.h"

/**
 * @brief 表示一整局牌局状态的数据模型。
 *
 * GameState 负责保存主牌区、底牌区以及备用牌堆等所有运行时数据，
 * 不包含复杂业务逻辑，仅提供基础的状态查询与 JSON 序列化能力。
 * 该类用于游戏运行时状态管理、存档保存以及读档恢复，是控制器层
 * 协调业务逻辑时依赖的核心数据结构。
 */
class GameState
{
public:
    /**
     * @brief 构造一个空的牌局状态。
     *
     * 默认情况下不会包含任何牌堆数据，通常由关卡加载逻辑或反序列化
     * 过程来填充具体内容。
     */
    GameState();

    /**
     * @brief 获取当前主牌区的所有牌堆。
     * @return 只读引用，外部不应直接修改内部容器。
     */
    const std::vector<std::vector<CardInstance>>& getMainPiles() const;

    /**
     * @brief 设置主牌区的所有牌堆。
     * @param piles 新的主牌区牌堆数据。
     */
    void setMainPiles(const std::vector<std::vector<CardInstance>>& piles);

    /**
     * @brief 获取当前底牌区的牌实例。
     * @return 底牌实例的常量引用。
     */
    const CardInstance& getTrayCard() const;

    /**
     * @brief 设置当前底牌区的牌实例。
     * @param card 新的底牌实例。
     */
    void setTrayCard(const CardInstance& card);

    /**
     * @brief 获取当前备用牌堆。
     * @return 备用牌实例数组的常量引用。
     */
    const std::vector<CardInstance>& getReserveDeck() const;

    /**
     * @brief 设置当前备用牌堆。
     * @param deck 新的备用牌堆数据。
     */
    void setReserveDeck(const std::vector<CardInstance>& deck);

    /**
     * @brief 获取存档版本号。
     * @return 存档版本号整数。
     */
    int getSaveVersion() const;

    /**
     * @brief 设置存档版本号。
     * @param version 新的版本号。
     */
    void setSaveVersion(int version);

    /**
     * @brief 将当前牌局状态序列化为 JSON 字符串。
     *
     * 该方法不执行文件 IO，只负责将内存中的状态转换为 JSON 文本，
     * 由 services 层负责写入磁盘。
     *
     * @return JSON 格式的字符串表示；失败时返回空字符串。
     */
    std::string serializeToJson() const;

    /**
     * @brief 从 JSON 字符串反序列化并覆盖当前牌局状态。
     *
     * 调用方在传入 JSON 文本前应确保数据来源可信，方法内部会进行
     * 基本的字段校验，但不会抛出异常。
     *
     * @param jsonString JSON 格式的牌局状态数据。
     * @return true 表示解析成功并已更新状态，false 表示解析失败。
     */
    bool deserializeFromJson(const std::string& jsonString);

private:
    /** @brief 存档版本号，便于后续扩展字段时保持兼容。 */
    int _saveVersion;

    /** @brief 主牌区的所有牌堆，每个子 vector 表示一列或一堆。 */
    std::vector<std::vector<CardInstance>> _mainPiles;

    /** @brief 当前底牌区的牌实例。 */
    CardInstance _trayCard;

    /** @brief 当前备用牌堆中的牌实例列表。 */
    std::vector<CardInstance> _reserveDeck;
};

#endif // GAMESTATE_H


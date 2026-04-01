#ifndef LEVELCONFIGSERVICE_H
#define LEVELCONFIGSERVICE_H

#include <string>
#include "configs/LevelConfig.h"

/**
 * @brief 关卡配置加载服务。
 *
 * LevelConfigService 负责将关卡 JSON 数据解析为 LevelConfig 对象，
 * 支持从字符串或文件进行加载。该类属于 services 层：
 * - 仅处理业务逻辑（解析与校验），不持有关卡数据生命周期；
 * - 不依赖 controllers；
 * - 对外提供静态方法，便于控制器在需要时直接调用。
 */
class LevelConfigService
{
public:
    /**
     * @brief 从 JSON 字符串解析关卡配置。
     * @param jsonString 关卡 JSON 字符串内容。
     * @param outConfig 输出的关卡配置对象（会被覆盖）。
     * @return true 表示解析成功；false 表示 JSON 格式或字段不合法。
     */
    static bool parseFromJsonString(const std::string& jsonString, LevelConfig& outConfig);

    /**
     * @brief 从文件加载并解析关卡配置。
     * @param fullPath 文件完整路径（或可被 FileUtils 解析的路径）。
     * @param outConfig 输出的关卡配置对象（会被覆盖）。
     * @return true 表示读取与解析都成功；false 表示读取失败或解析失败。
     */
    static bool loadFromFile(const std::string& fullPath, LevelConfig& outConfig);
};

#endif // LEVELCONFIGSERVICE_H


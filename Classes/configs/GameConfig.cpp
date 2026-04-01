#include "GameConfig.h"

/**
 * @brief GameConfig 静态配置实现。
 *
 * 该文件提供全局游戏静态配置常量的定义和访问方法，
 * 包括设计分辨率以及主牌区和堆牌区的逻辑尺寸。
 */

const int GameConfig::kDesignWidth = 1080;
const int GameConfig::kDesignHeight = 2080;
const int GameConfig::kPlayfieldHeight = 1500;
const int GameConfig::kStackAreaHeight = 580;

int GameConfig::getDesignWidth()
{
    return kDesignWidth;
}

int GameConfig::getDesignHeight()
{
    return kDesignHeight;
}

int GameConfig::getPlayfieldHeight()
{
    return kPlayfieldHeight;
}

int GameConfig::getStackAreaHeight()
{
    return kStackAreaHeight;
}


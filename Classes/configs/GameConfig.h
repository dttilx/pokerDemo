#ifndef GAMECONFIG_H
#define GAMECONFIG_H

/**
 * @brief 全局游戏配置数据。
 *
 * 该类集中存放与设备无关的固定配置，例如设计分辨率、
 * 主牌区与堆牌区的逻辑尺寸等。所有模块在需要这些常量
 * 时应通过本类提供的访问接口获取，避免在代码中出现魔法数字。
 *
 * GameConfig 只提供只读访问，不管理任何运行时状态。
 */
class GameConfig
{
public:
    /**
     * @brief 获取设计分辨率的宽度。
     * @return 设计分辨率宽度（像素）。
     */
    static int getDesignWidth();

    /**
     * @brief 获取设计分辨率的高度。
     * @return 设计分辨率高度（像素）。
     */
    static int getDesignHeight();

    /**
     * @brief 获取主牌区的逻辑高度。
     * @return 主牌区高度（像素）。
     */
    static int getPlayfieldHeight();

    /**
     * @brief 获取堆牌区的逻辑高度。
     * @return 堆牌区高度（像素）。
     */
    static int getStackAreaHeight();

private:
    /** @brief 设计分辨率宽度。 */
    static const int kDesignWidth;

    /** @brief 设计分辨率高度。 */
    static const int kDesignHeight;

    /** @brief 主牌区高度。 */
    static const int kPlayfieldHeight;

    /** @brief 堆牌区高度。 */
    static const int kStackAreaHeight;
};

#endif // GAMECONFIG_H


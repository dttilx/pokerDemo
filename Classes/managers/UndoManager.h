#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <vector>
#include "models/UndoMoveRecord.h"

/**
 * @brief 管理卡牌移动操作的撤销栈。
 *
 * 由控制器持有，禁止单例。仅存储 UndoMoveRecord 数据，不反向依赖控制器。
 * 弹出记录后由控制器负责执行反向动画与指针恢复。
 */
class UndoManager
{
public:
    /**
     * @brief 压入一条可撤销的互换记录（应在播放正向动画之前调用，坐标为互换前位置）。
     */
    void pushRecord(const UndoMoveRecord& record);

    /**
     * @brief 是否还有可撤销记录。
     */
    bool canUndo() const;

    /**
     * @brief 取出栈顶记录（不弹出），用于预检。
     */
    const UndoMoveRecord* peek() const;

    /**
     * @brief 弹出栈顶记录；栈空时返回 false。
     * @param outRecord 输出的记录。
     */
    bool popRecord(UndoMoveRecord& outRecord);

    /**
     * @brief 清空全部历史。
     */
    void clear();

private:
    std::vector<UndoMoveRecord> _records;
};

#endif // UNDOMANAGER_H

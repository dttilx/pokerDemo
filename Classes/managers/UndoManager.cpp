#include "UndoManager.h"

void UndoManager::pushRecord(const UndoMoveRecord& record)
{
    _records.push_back(record);
}

bool UndoManager::canUndo() const
{
    return !_records.empty();
}

const UndoMoveRecord* UndoManager::peek() const
{
    if (_records.empty())
        return nullptr;
    return &_records.back();
}

bool UndoManager::popRecord(UndoMoveRecord& outRecord)
{
    if (_records.empty())
        return false;
    outRecord = _records.back();
    _records.pop_back();
    return true;
}

void UndoManager::clear()
{
    _records.clear();
}

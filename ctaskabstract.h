#ifndef CTASKABSTRACT_H
#define CTASKABSTRACT_H

#include "ctaskstate.h"
#include <cstdint>
#include <QString>
#include <QFile>
#include <QDebug>

/**
 * @class CTaskAbstract
 * @brief this is abstract class for tasks which should be allowed to used
 * polymorphically
 */
class CTaskAbstract
{
public:
    CTaskAbstract() = default;
    virtual ~CTaskAbstract();
    virtual CTaskState Process();
    /**
     * @brief returns state of current task, basically class with stats
     */
    const CTaskState & GetState() const;
    /**
     * @brief sets memory limit to be used, if not called, memory check doesnt work
     * @param memLimit  maximal memory limit to be used
     */
    void SetMemLimit( uint32_t memLimit );
    /**
     * @return returns memory limit to be used
     */
    uint32_t GetMemLimit() const;
    /**
     * @brief GetTaskState
     * @return tasks CTaskState instance which can't be modified
     */
    const CTaskState & GetTaskState() const;
protected:
    /**
     * @brief compares result and reference files by comparing sizes, then by reading lines
     * @return true - same files / false - something went wrong
     */
    bool Compare() const;

    QString m_inFile = "", m_refFile = "", m_resFile = "", m_errorFile = "error_log.txt", m_taskPath = "";
    uint32_t m_memLimit = 0;
    CTaskState m_taskState;
};

#endif // CTASKABSTRACT_H

#ifndef CTASKSTATE_H
#define CTASKSTATE_H

#include <cstdint>
#include "QString"
#include "QTime"

class CTaskState
{
public:
    enum TASK_STATE : int { SUCCESSFUL = 100 , FAILED , PART_FAIL , TERROR };

                        CTaskState();
    void                SetEndTime();
    TASK_STATE          GetTaskState()      const;
    bool                IsSegFault()        const;
    bool                MemoryLeak()        const;
    uint32_t            TimeTaken()         const;
    const QString &     GetTaskMessage()    const;
    uint32_t            MemoryUsed()        const;

    void                SetTaskState( TASK_STATE state );
    void                SetSegfault( bool segfault );
    void                SetMemoryLeak( bool leaked );
    void                SetMessage( const QString & message );
    void                MemoryUsed( uint32_t memoryUsed );
private:
    TASK_STATE  m_taskState                 = TASK_STATE::SUCCESSFUL;
    bool        m_segfault                  = false;
    bool        m_memoryLeaked              = false;
    QTime       m_startTime, m_endTime;
    QString     m_message                   = "";
    uint32_t    m_memoryUsed                = __UINT32_MAX__;
};

#endif // CTASKSTATE_H

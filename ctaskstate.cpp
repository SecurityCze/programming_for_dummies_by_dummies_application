#include "ctaskstate.h"

CTaskState::CTaskState()
    : m_startTime( QTime().currentTime() )
{
}

void CTaskState::SetEndTime()
{
    m_endTime = QTime().currentTime();
}

CTaskState::TASK_STATE CTaskState::GetTaskState() const
{
    return m_taskState;
}

bool CTaskState::IsSegFault() const
{
    return m_segfault;
}

uint32_t CTaskState::TimeTaken() const
{
    return m_startTime.msecsTo( m_endTime );
}

const QString & CTaskState::GetTaskMessage() const
{
    return m_message;
}

uint32_t CTaskState::MemoryUsed() const
{
    return m_memoryUsed;
}

void CTaskState::SetTaskState( TASK_STATE state )
{
    m_taskState = state;
}

void CTaskState::SetSegfault( bool segfault )
{
    m_segfault = segfault;
}

void CTaskState::SetMessage( const QString & message )
{
    m_message = message;
}

void CTaskState::MemoryUsed( uint32_t memoryUsed )
{
    m_memoryUsed = memoryUsed;
}

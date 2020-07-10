#include "ctaskabstract.h"
#include <cstdint>
#include <QString>
#include <QFile>
#include <QDebug>

CTaskState::TASK_STATE CTaskAbstract::Process()
{
    return m_taskState.GetTaskState();
}

const CTaskState & CTaskAbstract::GetState() const
{
    return m_taskState;
}

void CTaskAbstract::SetMemLimit( uint32_t memLimit )
{
    m_memLimit = memLimit;
}

uint32_t CTaskAbstract::GetMemLimit() const
{
    return m_memLimit;
}

const CTaskState & CTaskAbstract::GetTaskState() const
{
    return m_taskState;
}

bool CTaskAbstract::Compare() const
{
    QFile resFile( m_resFile );
    QFile refFile( m_refFile );
    QString resLine = "", refLine = "";

    if(    ! resFile.open( QIODevice::ReadOnly | QIODevice::Text )
        || ! refFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Files were unable to open...";
        return false;
    }

    if( resFile.size() != refFile.size() )  return false;
    while( ! resFile.atEnd() && ! refFile.atEnd() )
    {
        resLine = resFile.readLine();
        refLine = refFile.readLine();

        if( resLine != refLine ) return false;
    }

    if( ! resFile.atEnd() || ! refFile.atEnd() ) return false;
    resFile.close();
    refFile.close();

    return true;
}

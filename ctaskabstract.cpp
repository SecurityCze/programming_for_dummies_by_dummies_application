#include "ctaskabstract.h"
#include <cstdint>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>

CTaskAbstract::~CTaskAbstract()
{
}

CTaskState CTaskAbstract::Process()
{
    return m_taskState;
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

    QTextStream resStream( &resFile );
    QTextStream refStream( &refFile );

    //if( resFile.size() != refFile.size() )  return false;
    while( ! resStream.atEnd() && ! refStream.atEnd() )
    {
        resLine = resStream.readLine();
        refLine = refStream.readLine();

        if( resLine != refLine ) return false;
    }

    if( ! resStream.atEnd() || ! refStream.atEnd() ) return false;
    resFile.close();
    refFile.close();

    return true;
}

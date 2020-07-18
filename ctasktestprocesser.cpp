#include "ctasktestprocesser.h"

#include <QString>
#include <QList>
#include <QRegularExpression>
#include <QFile>

QList< CTaskTestProcesser::CTaskSettings > CTaskTestProcesser::Parse( const QString & taskIndexerPath , uint32_t & timeLimit )
{
    QList< CTaskSettings > returnList;
    QFile taskIndexer( taskIndexerPath + "/" + CConstants::s_TEST_FOLDER + "/" + CConstants::s_TEST_INDEXER );
    if( ! taskIndexer.exists() || ! taskIndexer.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return returnList;

    QRegularExpression pattern( s_REG_EXP_TASK_INDEXER_PATTERN );
    QRegularExpressionMatch match;
    uint32_t newTimeLimit = 0;

    while( ! taskIndexer.atEnd() )
    {
        match = pattern.match( taskIndexer.readLine() );
        if( match.hasMatch() )
            returnList.append( NewSettings( match.captured( 1 ) ,
                                            match.captured( 2 ).toInt() ,
                                            match.captured( 3 ).toUInt() ,
                                            match.captured( 4 ).toUInt() ));
        newTimeLimit = match.captured( 4 ).toUInt();
        if( newTimeLimit > timeLimit ) timeLimit = newTimeLimit;
    }
    taskIndexer.close();
    return returnList;
}

CTaskTestProcesser::CTaskSettings CTaskTestProcesser::NewSettings( const QString & testName , bool leakcheck , uint32_t memlimit , uint32_t timelimit )
{
    CTaskSettings t = { testName , leakcheck , memlimit , timelimit };
    return t;
}

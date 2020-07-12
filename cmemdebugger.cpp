#include "cmemdebugger.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegularExpression>

bool CMemdebugger::IsAvailable()
{
#if defined _WIN32 || defined _WIN64
    qDebug() << s_WINDOWS_ERROR_MESSAGE;
    return false;
#elif defined __linux__
    system("valgrind 2> valgrind-check.txt");
    bool avail = false;
    QFile tmp("valgrind-check.txt");
    if(    ! tmp.exists()
        || ! tmp.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return false;
    QString checkLine = tmp.readLine();
    checkLine.remove( checkLine.length() - 1 , 1 );
    if( checkLine != "valgrind: no program specified") avail = true;
    else                                               avail = false;

    RemoveGarbage( tmp );
    return  avail;
#endif
}

CMemdebugger::deb_struct CMemdebugger::Process( const QString & path , const QString & errorPath )
{
    deb_struct result = CreateInvalidStruct();
#if defined _WIN32 || defined _WIN64
    qDebug() << s_WINDOWS_ERROR_MESSAGE;
    return CreateInvalidStruct();
#elif defined __linux__
    QFile program( path );
    if( ! program.exists() )
    {
        qDebug() << s_UNABLE_TO_LOAD_PORGRAM;
        return CreateInvalidStruct();
    }
     system("valgrind --log-file=\"valgrind-out.txt\" ./a.out 2> " + errorPath );
     system("tail -5 valgrind-out.txt | head -1 > leaks.txt");
     system("tail -1 valgrind-out.txt > error-count.txt");
     system("tail -15 valgrind-out.txt > memory-usage.txt");
#endif

    QFile leaks( "leaks.txt" );
    if( ! leaks.exists() || ! leaks.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << s_OUTPUT_MISSING_LEAKS;
        return CreateInvalidStruct();
    }
    GetLeaks( result , leaks );

    QFile errorCnt( "error-count.txt ");
    if( ! errorCnt.exists() || ! errorCnt.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << s_OUTPUT_MISSING_ERRORS;
        return CreateInvalidStruct();
    }
    GetErrorCnt( result , errorCnt );

    QFile usedMem( "memory-usage.txt" );
    if( ! usedMem.exists() || ! usedMem.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << s_OUTPUT_MISSING_USD_MEM;
        return CreateInvalidStruct();
    }

    RemoveGarbage( leaks );
    RemoveGarbage( errorCnt );
    RemoveGarbage( usedMem );

    return result;
}

void CMemdebugger::RemoveGarbage( QFile & file )
{
    file.close();
    file.remove();
}

CMemdebugger::deb_struct CMemdebugger::CreateInvalidStruct()
{
    deb_struct t;
    t.m_errorCnt = t.m_usedMem = -1;
    t.m_leakedMem = true;
    return t;
}

void CMemdebugger::GetLeaks( deb_struct & result , QFile & leaks )
{
    QRegularExpression leaksREGEX( s_REG_EX_LEAKS );
    QRegularExpressionMatch leaksREGEXMatch = leaksREGEX.match( leaks.readLine() );
    if( leaksREGEXMatch.hasMatch() ) result.m_leakedMem = false;
    else                             result.m_leakedMem = true;
}

void CMemdebugger::GetErrorCnt( deb_struct & result , QFile & errorCnt )
{
    QRegularExpression errorREGEX( s_REG_EX_ERRORS );
    QRegularExpressionMatch errorREGEXMatch = errorREGEX.match( errorCnt.readLine() );
    if( errorREGEXMatch.hasMatch() ) result.m_errorCnt = errorREGEXMatch.captured( 1 ).toInt();
    else                             result.m_errorCnt = -1;
}

void CMemdebugger::GetUsedMem( deb_struct & result , QFile & usedMem )
{
    QRegularExpression usedMemREGEX( s_REG_EX_USED_MEM );
    QRegularExpressionMatch usedMemREGEXMatch = usedMemREGEX.match( usedMem.readLine() );
    if( usedMemREGEXMatch.hasMatch() ) result.m_usedMem = usedMemREGEXMatch.captured( 1 ).remove(',').toInt();
    else                               result.m_usedMem = -1;
}

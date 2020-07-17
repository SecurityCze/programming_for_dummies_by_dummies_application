#include "ctasknolimit.h"

#include <QThread>
#if defined _WIN32 || defined _WIN64
    #include <windows.h>
#endif

CTaskNoLimit::CTaskNoLimit( const QString & testName , const QString & taskPath )
{
    m_inFile = taskPath + "/" + CConstants::s_TEST_FOLDER + "/" + testName + "_in.txt";
    m_refFile = taskPath + "/" + CConstants::s_TEST_FOLDER + "/" + testName + "_ref.txt";
    m_resFile = taskPath + "/" + CConstants::s_TEST_FOLDER + "/" + testName + "_res.txt";
    m_errorFile = taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + m_errorFile;
    m_taskPath = taskPath;
}

CTaskState CTaskNoLimit::Process()
{
    CTaskState::TASK_STATE returnState;
    CMemdebugger::deb_struct memResult{ -1 , -1 , false };
    RemoveGarbage();

    QFile finished( m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_TASK_FINISHED );
    if( finished.exists() ) finished.remove();

#if defined _WIN32 || defined _WIN64
    QFile m_program( m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN );
    if( ! m_program.exists() )
    {
        qDebug() << "It doesn't exist";
        m_taskState.SetTaskState( CTaskState::TASK_STATE::TERROR );
        return m_taskState;
    }
    ShellExecuteA( 0 , "open" , "cmd.exe" , ( "/C " + m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN + " < " + m_inFile + " > " + m_resFile + " 2> " + m_errorFile ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
    QFile m_program( m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_LIN );
    if( ! m_program.exists() )
    {
        m_taskState.SetTaskState( CTaskState::TASK_STATE::TERROR );
        return m_taskState;
    }
    // system( m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_LIN + " < " + m_inFile + " > " + m_resFile + " 2> " + m_errorFile ); // shouldnt uncomment, just for future I dont know what the fuck I fucked up moment to realise to how to that
    memResult = CMemdebugger::Process( "a.out " , m_errorFile );
#endif
    m_taskState.SetEndTime();

    uint32_t waitTime = CConstants::ui_timeLimit;
    while( waitTime )
    {
        QThread::msleep( 500 );
        waitTime -= 500;
        if( finished.exists() ) break;
    }

    if( ! finished.exists() )
    {
        m_taskState.SetMessage( s_TIME_EXCEEDED );
        m_taskState.SetTaskState( CTaskState::TASK_STATE::FAILED );
        return m_taskState;
    }
    else
        finished.remove();

    if( IsSegfault() ) returnState = CTaskState::TASK_STATE::FAILED;

    if( Compare() ) returnState = CTaskState::TASK_STATE::SUCCESSFUL;
    else            returnState = CTaskState::TASK_STATE::FAILED;

    if(    IsValidMemResult( memResult ) && memResult.m_leakedMem
        && returnState == CTaskState::TASK_STATE::SUCCESSFUL )
            returnState = CTaskState::TASK_STATE::PART_FAIL;

    if( IsValidMemResult( memResult ) ) m_taskState.MemoryUsed( memResult.m_usedMem );

    m_taskState.SetTaskState( returnState );
    return m_taskState;
}

bool CTaskNoLimit::IsSegfault()
{
    QFile errorFile( m_taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + m_errorFile );
    if( errorFile.exists() && errorFile.size() > 0 )
    {
        m_taskState.SetSegfault( true );
        errorFile.open( QIODevice::ReadOnly | QIODevice::Text );
        m_taskState.SetMessage( errorFile.readAll() );
        errorFile.close();
        return true;
    }
    return false;
}

bool CTaskNoLimit::IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const
{
    return ! ( memResult.m_usedMem < 0 || memResult.m_errorCnt < 0 );
}

void CTaskNoLimit::RemoveGarbage()
{
    QFile( m_errorFile ).remove();
    QFile( m_resFile ).remove();
}

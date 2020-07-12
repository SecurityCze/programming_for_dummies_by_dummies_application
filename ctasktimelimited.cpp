#include "ctasktimelimited.h"

#if defined _WIN32 || defined _WIN64
    #include <windows.h>
#endif

CTaskTimeLimited::CTaskTimeLimited( const QString & inputPath , const QString & referencePath , uint32_t timeLimit )
    : m_timeLimit( timeLimit )
{
    m_inFile = inputPath;
    m_refFile = referencePath;
    m_resFile = "result.txt";
}

CTaskState::TASK_STATE CTaskTimeLimited::Process()
{
    CTaskState::TASK_STATE returnState = CTaskState::TASK_STATE::SUCCESSFUL;
    CMemdebugger::deb_struct memResult{ -1 , -1 , false };
    RemoveGarbage();

#if defined _WIN32 || defined _WIN64
    QFile m_program("a.exe");
    if( ! m_program.exists() ) return CTaskState::TASK_STATE::TERROR;
    ShellExecuteA( 0 , "open" , "cmd.exe" , ( "/C a.exe < " + m_inFile + " > " + m_resFile + " 2> " + m_errorFile ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
    QFile m_program("a.out");
    if( ! m_program.exists() ) return CTaskState::TASK_STATE::TERROR;
    // system("./a.out < " + m_inFile + " > " + m_resFile + " 2> " + errorPath ); // shouldnt uncomment, just for future I dont know what the fuck I fucked up moment to realise to how to that
    memResult = CMemdebugger::Process( "a.out " , m_errorFile );
#endif
    m_taskState.SetEndTime();

    if( m_taskState.TimeTaken() > m_timeLimit ) returnState = CTaskState::TASK_STATE::PART_FAIL;

    if( IsSegfault() ) returnState = CTaskState::TASK_STATE::FAILED;

    if( ! Compare() ) returnState = CTaskState::TASK_STATE::FAILED;

    if(    IsValidMemResult( memResult ) && memResult.m_leakedMem
        && (    returnState == CTaskState::TASK_STATE::SUCCESSFUL
             || returnState == CTaskState::TASK_STATE::PART_FAIL ) )
            returnState = CTaskState::TASK_STATE::PART_FAIL;

    if( IsValidMemResult( memResult ) ) m_taskState.MemoryUsed( memResult.m_usedMem );

    m_taskState.SetTaskState( returnState );
    return m_taskState.GetTaskState();
}

bool CTaskTimeLimited::IsSegfault()
{
    QFile errorFile( m_errorFile );
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

bool CTaskTimeLimited::IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const
{
    return ! ( memResult.m_usedMem < 0 || memResult.m_errorCnt < 0 );
}

void CTaskTimeLimited::RemoveGarbage()
{
    QFile( m_errorFile ).remove();
    QFile( m_resFile ).remove();
}

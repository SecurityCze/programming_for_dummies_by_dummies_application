#include "ctasknolimit.h"

#if defined _WIN32 || defined _WIN64
    #include <windows.h>
#endif

CTaskNoLimit::CTaskNoLimit( const QString & inputPath , const QString & referencePath )
{
    m_inFile = inputPath;
    m_refFile = referencePath;
    m_resFile = "result.txt";
}

CTaskState::TASK_STATE CTaskNoLimit::Process()
{
    QString errorPath = "error_log.txt";
    // TODO: add memory support
#if defined _WIN32 || defined _WIN64
    QFile m_program("a.exe");
    if( ! m_program.exists() ) return CTaskState::TASK_STATE::TERROR;
    ShellExecuteA( 0 , "open" , "cmd.exe" , ( "/C a.exe < " + m_inFile + " > " + m_resFile + " 2> " + errorPath ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
    QFile m_program("a.out");
    if( ! m_program.exists() ) return CTaskState::TASK_STATE::TERROR;
    system("./a.out < " + m_inFile + " > " + m_resFile + " 2> " + errorPath );
#endif

    QFile errorFile( errorPath );
    if( errorFile.exists() && errorPath.size() > 0 )
    {
        m_taskState.SetSegfault( true );
        errorFile.open( QIODevice::ReadOnly | QIODevice::Text );
        m_taskState.SetMessage( errorFile.readAll() );
        errorFile.close();
        return CTaskState::TASK_STATE::FAILED;
    }

    if( Compare() )
    {
        QFile( errorPath ).remove();
        QFile( m_resFile ).remove();
        return CTaskState::TASK_STATE::SUCCESSFUL;
    }
    else
    {
        QFile( errorPath ).remove();
        QFile( m_resFile ).remove();
        return CTaskState::TASK_STATE::FAILED;
    }
}

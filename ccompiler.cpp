#include "ccompiler.h"

#include <QFileDialog>
#include <QDebug>
#include <QThread>
#if defined _WIN32 || defined _WIN64
    #include <windows.h>
#endif

CCompiler::CCompiler()
{
}

CCompiler::COMP_STATES CCompiler::IsAvailable() const
{
    ShellExecuteA(0, "open", "cmd.exe", "/C g++ 2> tmp.txt", 0, SW_HIDE);
    QThread::msleep( 50 );  // program while file is not created (dfaq is this?, I <3 Ubuntu, not this garbage)

    QFile tmpFile("tmp.txt");
    if( ! tmpFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return COMP_STATES::COMP_ERROR;

    QString tmpString = tmpFile.readLine();
    tmpString.remove( tmpString.length() - 1 , 1 );

    if( tmpString == "g++: fatal error: no input files" )
    {
        CloseAndDelete( tmpFile );
        return COMP_STATES::COMP_AVAILABLE;
    }
    CloseAndDelete( tmpFile );
    return COMP_STATES::COMP_MISSING;
}

void CCompiler::CloseAndDelete( QFile & file ) const
{
    file.close();
    file.remove();
}

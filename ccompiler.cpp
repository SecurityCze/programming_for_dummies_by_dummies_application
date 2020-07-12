#include "ccompiler.h"

#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QList>

#if defined _WIN32 || defined _WIN64
    #include <windows.h>
    #define PLATFORM    0
#elif defined __linux__
    #define PLATFORM    1
#else
    #define PLATFORM    -1
#endif

// ---------- PUBLIC ----------

CCompiler::CCompiler()
{
}

CCompiler::COMP_STATES CCompiler::IsAvailable()
{
    if( ! SupportedPlatform() )
        return COMP_STATES::COMP_ERROR;

    RunDefaultCompilerDP();

    QFile tmpFile("tmp.txt");
    if( ! tmpFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return COMP_STATES::COMP_ERROR;

    QString tmpString = tmpFile.readLine();
    tmpString.remove( tmpString.length() - 1 , 1 ); // parses QString and deletes \n read by readLine

    if( tmpString == "g++: fatal error: no input files" )
    {
        CloseAndDelete( tmpFile );
        return COMP_STATES::COMP_AVAILABLE;
    }
    CloseAndDelete( tmpFile );
    return COMP_STATES::COMP_MISSING;
}

bool CCompiler::SupportedPlatform()
{
    return ( PLATFORM == 0 || PLATFORM == 1 );
}

CCompiler::COMPILATION CCompiler::Compile( const QString & filePath , const QList< CCompiler::COMP_PARAMS > & arguments )
{
#if defined _WIN32 || defined _WIN64
        QFile result("a.exe");
#elif defined __linux__
        QFile result("a.out");
#endif

    QFile errors("errors.txt");

    if( result.exists() ) result.remove();
    if( errors.exists() ) errors.remove();

    CompileFileParamsDP( filePath , PrepareParams( arguments ) );
    return EvaluateCompilation();
}

void CCompiler::CompileFileParamsDP( const QString & filePath , const QString & params )
{
#if defined _WIN32 || defined _WIN64
    ShellExecuteA( 0 , "open" , "cmd.exe" , ( "/C g++ " + params + " " + filePath + " 2> errors.txt" ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
    system( "g++ " + params + " " + filePath + " 2> errors.txt" ).toStdString().c_str() );
#endif
}

// ---------- PRIVATE ----------

void CCompiler::RunDefaultCompilerDP()
{
#if defined _WIN32 || defined _WIN64
        ShellExecuteA( 0 , "open" , "cmd.exe" , "/C g++ 2> tmp.txt" , 0 , SW_HIDE );
#elif defined __linux__
        system("g++ 2> tmp.txt");
#endif
    QThread::msleep( 50 );  // program has to wait while file is not created (dfaq is this?, I <3 Ubuntu, not this garbage);
}

void CCompiler::CloseAndDelete( QFile & file )
{
    file.close();
    file.remove();
}

QString CCompiler::PrepareParams( const QList< CCompiler::COMP_PARAMS > & arguments )
{
    QString retParams = "";
    for( const auto & param : arguments )
    {
        retParams += " ";
        if     ( param == COMP_PARAMS::PEDANTIC ) retParams += "-pedantic";
        else if( param == COMP_PARAMS::WEXTRA   ) retParams += "-Wextra";
        else if( param == COMP_PARAMS::WALL     ) retParams += "-Wall";
    }
    return retParams;
}

CCompiler::COMPILATION CCompiler::EvaluateCompilation()
{
    QThread::msleep( 1000 );    // wait for compilation be done and files be created

#if defined _WIN32 || defined _WIN64
        QFile result("a.exe");
#elif defined __linux__
        QFile result("a.out");
#endif

    QFile errors("errors.txt");

    if     ( result.exists() && errors.size() == 0 ) return COMPILATION::SUCCESSFUL;
    else if( result.exists() && errors.size() >  0 ) return COMPILATION::WITH_WARNINGS;
    else if( ! result.exists() )                     return COMPILATION::FAILED;

    return COMPILATION::FAILED;
}

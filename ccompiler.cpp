#include "ccompiler.h"

#include <QFileDialog>
#include <QDebug>
#include <QThread>
#include <QList>
#include <QDir>

#if defined _WIN32 || defined _WIN64
    #include <windows.h>
    #define PLATFORM    0
#elif defined __linux__
    #define PLATFORM    1
#else
    #define PLATFORM    -1
#endif

// ---------- PUBLIC ----------

CCompiler::COMP_STATES CCompiler::IsAvailable()
{
    if( ! SupportedPlatform() )
        return COMP_STATES::COMP_ERROR;

    RunDefaultCompilerDP();

    QFile tmpFile( s_COMPILER_CHECK_FILE );
    if( ! tmpFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return COMP_STATES::COMP_ERROR;

    QString tmpString = tmpFile.readLine();
    tmpString.remove( tmpString.length() - 1 , 1 ); // parses QString and deletes \n read by readLine
    qDebug() << "|" << tmpString << "|\n";
    if( tmpString == s_COMPILER_CHECK_QUOTE )
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

CCompiler::COMPILATION CCompiler::Compile( const QString & filePath , const QString & taskPath , const QList< CCompiler::COMP_PARAMS > & arguments )
{
    // if "sources" file doesn't exist, create it
    if( ! QDir().exists( taskPath + "/" + CConstants::s_SOURCE_FOLDER ) ) QDir().mkdir( taskPath + "/" + CConstants::s_SOURCE_FOLDER );
    QString processedPath = taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CCodeParser::Parse( filePath , taskPath );
#if defined _WIN32 || defined _WIN64
        QFile result( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN );
#elif defined __linux__
        QFile result( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_LIN );
#endif
    QFile errors( taskPath + CConstants::s_SOURCE_FOLDER );

    if( result.exists() ) result.remove();
    if( errors.exists() ) errors.remove();

    CompileFileParamsDP( processedPath , taskPath , PrepareParams( arguments ) );
    return EvaluateCompilation( taskPath );
}

void CCompiler::CompileFileParamsDP( const QString & filePath , const QString & taskPath , const QString & params )
{
    QString debug = ( "/C g++ " + params + " -o " + taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN + " " + filePath + " 2> " + taskPath +  "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_COMPILATION_ERRORS );
    qDebug() << debug;
#if defined _WIN32 || defined _WIN64
    ShellExecuteA( 0 , "open" , "cmd.exe" , ( "/C g++ " + params + " -o " + taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN + " " + filePath + " 2> " + taskPath +  "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_COMPILATION_ERRORS ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
    system( "g++ " + params + " -o " + taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN + " " + filePath + " 2> " + taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_COMPILATION_ERRORS ).toStdString().c_str() );
#endif
}

// ---------- PRIVATE ----------

void CCompiler::RunDefaultCompilerDP()
{
    unsigned int msRemTime = 5000;  // compiler will have 5 secs to create folder and check it
#if defined _WIN32 || defined _WIN64
        ShellExecuteA( 0 , "open" , "cmd.exe" , ( QString( "/C g++ 2> " ) + QString( s_COMPILER_CHECK_FILE ) ).toStdString().c_str() , 0 , SW_HIDE );
#elif defined __linux__
        system( ( QString( "/C g++ 2> " ) + QString( s_COMPILER_CHECK_FILE ) ).toStdString().c_str() );
#endif
    for( ; msRemTime ; msRemTime -= 100)
        if( QFile( s_COMPILER_CHECK_FILE ).exists() ) break;
        else                                          QThread::msleep( 50 );  // program has to wait while file is not created (dfaq is this?, I <3 Ubuntu, not this garbage);
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

CCompiler::COMPILATION CCompiler::EvaluateCompilation( const QString & taskPath )
{
    QThread::msleep( 1000 );    // wait for compilation be done and files be created

#if defined _WIN32 || defined _WIN64
        QFile result( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_WIN );
#elif defined __linux__
        QFile result( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_READY_BINARY_LIN );
#endif

    QFile errors( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + CConstants::s_COMPILATION_ERRORS );

    if     ( result.exists() && errors.size() == 0 ) return COMPILATION::SUCCESSFUL;
    else if( result.exists() && errors.size() >  0 ) return COMPILATION::WITH_WARNINGS;
    else if( ! result.exists() )                     return COMPILATION::FAILED;

    return COMPILATION::FAILED;
}

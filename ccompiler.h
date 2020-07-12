#ifndef CCOMPILER_H
#define CCOMPILER_H

#include <QFileDialog>
#include <QList>

class CCompiler
{
public:
    /* states representing if compilator is present on pc - windows and linux only */
    enum COMP_STATES : int { COMP_AVAILABLE = 0  , COMP_MISSING  , COMP_ERROR };
    /* parameters inserted to Compile function */
    enum COMP_PARAMS : int { PEDANTIC       = 10 , WEXTRA        , WALL       };
    /* states of success of compilated program */
    enum COMPILATION : int { SUCCESSFUL     = 20 , WITH_WARNINGS , FAILED     };

    CCompiler();
    /**
     * @brief returns COMP_STATES::{ COMP_AVAILABLE , COMP_MISSING , COMP_ERROR } depending on if compilator is present
     * @return COMP_STATES::{ COMP_AVAILABLE , COMP_MISSING , COMP_ERROR }
     */
    static CCompiler::COMP_STATES IsAvailable();
    /**
     * @brief checks if compilator is available on current platform
     * @return true / false
     */
    static bool SupportedPlatform();
    /**
     * @brief compiles program given by filePath with given arguments
     * @param filePath      path to source code from current instance of running program
     * @param arguments     COMP_PARAMS::{ PEDANTIC = -pedantic , WEXTRA = -Wextra , WALL = -Wall }
     *                      entered as list of integers, for example: 'Compile("main.cpp", { CCompiler::COMP_PARAMS::PEDANTIC , CCompiler::COMP_PARAMS::WALL } );'
     * @return COMPILATION::{ SUCCESSFUL , WITH_WARNING , FAILED }
     */
    static CCompiler::COMPILATION Compile( const QString & filePath , const QList< CCompiler::COMP_PARAMS > & arguments );
private:
    /**
     * @brief compiles file with given parameters
     */
    static void CompileFileParamsDP( const QString & filePath , const QString & params );
    /**
     * @brief tries to run compiler without any files, just to get known if compiler is available
     * DP - "depending on platform"
     */
    static void RunDefaultCompilerDP();
    /**
     * @brief closses and deletes given file instance
     * @param file          opened file
     */
    static void CloseAndDelete( QFile & file );
    /**
     * @brief expands argument list and stacks it to string which is used for compilation
     * @param arguments     COMP_PARAMS::{ PEDANTIC = -pedantic , WEXTRA = -Wextra , WALL = -Wall }
     * @return
     */
    static QString PrepareParams( const QList< CCompiler::COMP_PARAMS > & arguments );
    /**
     * @brief evaluates compilation by checking if "a.out/a.exe" was created and by size of error list
     * @return COMPILATION::{ SUCCESSFUL , WITH_WARNING , FAILED }
     */
    static CCompiler::COMPILATION EvaluateCompilation();
};

#endif // CCOMPILER_H

#ifndef CMEMDEBUGGER_H
#define CMEMDEBUGGER_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegularExpression>

/**
 * @class CMemdebugger
 * @brief CMemdebugger is static class used for running programs with active
 * memory management control and some advanced error control, currently
 * only platform supported is Linux (using installed valgrind via system commands)
 */
class CMemdebugger
{
public:
    /**
     * @brief structure representing memory and error output of processed program
     * includes: m_errorCnt (int), m_usedMem (int), m_leakedMem (bool)
     */
    struct deb_struct {
        int m_errorCnt;
        int m_usedMem;
        bool m_leakedMem;
    };
    CMemdebugger() = default;
    /**
     * @brief checks if user used platform is supported by CMemdebugger
     * @return true - available / false - unavailable
     */
    static bool IsAvailable();
    /**
     * @brief runs program with memdebugger on, should be called after IsAvailable == true call
     * @param path      path to program that should be ran with memdebugger
     * @param errorPath path to location where error file should be stored
     * @return structure with data about program memory usage,
     *         if something went wrong, -1 values are used
     */
    static deb_struct Process( const QString & path , const QString & errorPath );
private:
    /**
     * @brief closes and removes file given by parameter
     * @param file given file to be removed
     */
    static void RemoveGarbage( QFile & file );
    /**
     * @brief creates invalid deb_struct in case something went wrong
     * @return deb_struct := m_errorCnt = -1, m_usedMem = -1, m_leakedMem = false
     */
    static deb_struct CreateInvalidStruct();
    /**
     * @brief checks if program leaks by parsing valgrind output files
     * @param result    deb_struct reference to be used
     * @param leaks     file to be parsed
     */
    static void GetLeaks( deb_struct & result , QFile & leaks );
    /**
     * @brief checks if program throws errors by parsing valgrind output files
     * @param result    deb_struct reference to be used
     * @param errorCnt  file to be parsed
     */
    static void GetErrorCnt( deb_struct & result , QFile & errorCnt );
    /**
     * @brief checks how much memory was used by parsing valgrind output files
     * @param result    deb_struct reference to be used
     * @param usedMem   file to be parsed
     */
    static void GetUsedMem( deb_struct & result , QFile & usedMem );

    static constexpr const char *const s_WINDOWS_ERROR_MESSAGE  = "For some time, we don't support memory checking on windows devices, sorry. :( Get Linux.";
    static constexpr const char *const s_UNABLE_TO_LOAD_PORGRAM = "I wasn't able to find your program. :(";
    static constexpr const char *const s_OUTPUT_MISSING_ERRORS  = "I wasn't able to find or open valgrind error-count.txt output file.";
    static constexpr const char *const s_OUTPUT_MISSING_LEAKS   = "I wasn't able to find or open valgrind leaks.txt output file.";
    static constexpr const char *const s_OUTPUT_MISSING_USD_MEM = "I wasn't able to find or open valgrind memory-usage.txt file.";

    /** regex patterns used by Get... functions */
    static constexpr const char *const s_REG_EX_ERRORS          = "==[\\d]+== ERROR SUMMARY: ([\\d]+) errors from [\\d]+ contexts \\(suppressed: 0 from 0\\)";
    static constexpr const char *const s_REG_EX_LEAKS           = "==[\\d]+== All heap blocks were freed -- no leaks are possible";
    static constexpr const char *const s_REG_EX_USED_MEM        = "==[\\d]+==   total heap usage: [\\d]+ allocs, [\\d]+ frees, ([\\d,]+) bytes allocated";
};

#endif // CMEMDEBUGGER_H

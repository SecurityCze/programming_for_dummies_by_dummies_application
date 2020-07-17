#ifndef CCONSTANTS_H
#define CCONSTANTS_H

#include <cstdint>

/**
 * @class CConstants
 * @brief CConstants class is designed to hold constants paths, names
 * and other by program used information
 */
class CConstants
{
public:
    CConstants() = delete;
    static constexpr const char *const s_SOURCE_FOLDER      = "sources";
    static constexpr const char *const s_READY_BINARY_LIN   = "a.out";
    static constexpr const char *const s_READY_BINARY_WIN   = "a.exe";
    static constexpr const char *const s_COMPILATION_ERRORS = "compilation-errors.txt";

    static constexpr const char *const s_ERROR_COMPILER_ERR = "I couldn't find compiler on your PC, try again or install compiler and add it to your path. :)";
    static constexpr const char *const s_ERROR_COMP_FAILED  = "I wasn't able to compile your code. Probably bad code. :)";
    static constexpr const char *const s_ERROR_COMP_WARNS   = "I compiled your code but I found some nasty errors. Go fix them. Mark penalisation: ";
    static constexpr const char *const s_COMPILATION_SUCCES = "Compilation succesful!";
    static constexpr const char *const s_NO_TESTS_FOUND     = "I wasn't able to find any tests, task is probably broken. Contact administators for more help.";

    static constexpr const char *const s_TEST_INDEXER       = "test_indexer.txt";
    static constexpr const char *const s_TEST_FOLDER        = "tests";

    static constexpr const char *const s_TASK_FINISHED      = "finished.txt";

    static const uint32_t ui_timeLimit = 14000;
};

#endif // CCONSTANTS_H

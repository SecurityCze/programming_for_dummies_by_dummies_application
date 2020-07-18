#ifndef CTASKTESTPROCESSER_H
#define CTASKTESTPROCESSER_H

#include "cconstants.h"

#include <QString>
#include <QList>
#include <QRegularExpression>
#include <QFile>

/**
 * @class CTaskTestProcesser
 * @brief CTaskTestProcesser class serves as test loader for task
 * and validator of results
 *
 * test-indexer structure: test name | leakcheck | memory usage limit | time limit
 * test name: 00 (programs works with _in.txt, _ref.txt system, therefore 00_in.txt will result into 00_in.txt_in.txt)
 * leakcheck: "bool", 0 - off, 1 - on
 * memory usage limit: uint, 0 - off, (0, x> - x is upper limit in bytes
 * time limit: uint, 0 - off, (0, x> - x is upper limit in milliseconds
 *
 * example: awesome-test 1 10000 0
 * program will look for awesome-test_in.txt and awesome-test_ref.txt
 * leakcheck is on
 * memory limit is 10000 bytes
 * time limit is off
 */
class CTaskTestProcesser
{
public:
    /**
     * @brief return structure of task settigns
     */
    struct CTaskSettings
    {
        QString     m_testName;
        bool        m_leakcheck;
        uint32_t    m_memLimit;
        uint32_t    m_timeLimit;
    };

    CTaskTestProcesser() = delete;
    /**
     * @brief parses task_indexer.txt by CTaskSettings template,
     * more infor in CTaskTestProcceser class comment
     * @param taskIndexer   path to task_indexer.txt
     * @param tiemLimit     timeLimit given to CCompiler as maximal time limit for program
     * @return  list of parsed tasks settings
     */
    static QList< CTaskSettings > Parse( const QString & taskIndexerPath , uint32_t & timeLimit );

private:
    /**
     * @brief makes new CTaskSettings structure
     * @param testName  name of current tasktest
     * @param leakcheck if should check leaks
     * @param memlimit  limit of memory program can use
     * @param timelimit time limit program can run
     * @return allocated CTaskSettings structure
     */
    static CTaskSettings NewSettings( const QString & testName , bool leakcheck , uint32_t memlimit , uint32_t timelimit );

    static constexpr const char *const s_REG_EXP_TASK_INDEXER_PATTERN = "([\\da-zA-Z-_.]+) ([01]) ([\\d]+) ([\\d]+)";
};

#endif // CTASKTESTPROCESSER_H

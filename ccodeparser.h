#ifndef CCODEPARSER_H
#define CCODEPARSER_H

#include "cconstants.h"
#include <QRegularExpression>
#include <QString>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <QDebug>

/**
 * @class CCodeParser
 * @brief parser class for parsing code to add time control
 * and segfault management
 */
class CCodeParser
{
public:
    CCodeParser() = delete;
    /**
     * @brief method for parsing code to add some libraries for better
     * work with segfaults and possible other stuff like adding
     * custom libraries
     * @param codePath  path to code to be parsed
     * @param taskPath  path to location where parsed code should be stored
     * @return name of file where is stored new code
     */
    static QString Parse( const QString & codePath , const QString & taskPath );
private:
    static void InsertHeaders( QTextStream & ts , const QString & header );
    static void InsertMain( QTextStream & ts , const QString & funcName );
    static void InsertFooter( QTextStream & ts , const QString & footer );
    static void RenameMain( QTextStream & ts , const QString & main , const QString & funcName );

    static constexpr const char *const s_REG_EX_MAIN    = "(.*)int main\\([\\s]*void[\\s]*\\)[\\s]*{(.*)}(.*)";
    static constexpr const char *const s_PROCESSED_CODE = "processed-code.cpp";
};

#endif // CCODEPARSER_H

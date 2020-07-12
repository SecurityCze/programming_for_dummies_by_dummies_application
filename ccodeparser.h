#ifndef CCODEPARSER_H
#define CCODEPARSER_H

#include <QRegularExpression>
#include <QString>
#include <QFile>
#include <QTextStream>

/**
 * @class CCodeParser
 * @brief parser class for parsing code to add time control
 * and segfault management
 */
class CCodeParser
{
public:
    CCodeParser() = delete;
    static QString Parse( const QString & path );
private:
    static constexpr const char *const s_REG_EX_MAIN    = "(.*)int main\\([\\s]*void[\\s]*\\)[\\s]*{(.*)}(.*)";
};

#endif // CCODEPARSER_H

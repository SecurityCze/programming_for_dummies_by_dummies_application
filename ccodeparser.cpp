#include "ccodeparser.h"

#include <QRegularExpression>
#include <QString>
#include <QFile>
QString CCodeParser::Parse( const QString & path )
{
    QFile fileToParse( path );
    if( ! fileToParse.exists() || ! fileToParse.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return ".";

    QRegularExpression matchPattern( s_REG_EX_MAIN );
    QRegularExpressionMatch mainMatch = matchPattern.match( fileToParse.readAll() );
    if( mainMatch.hasMatch() )
    {
        QFile newCode("new-code.cpp");
        newCode.open( QIODevice::WriteOnly | QIODevice::Text );

        QTextStream ts( & newCode );
        ts << mainMatch.captured( 1 );
        ts << "int main( void )\n{";
        ts << mainMatch.captured( 2 );
        ts << "}\n\n";
        ts << mainMatch.captured( 3 );
        newCode.close();
        return "new-code.cpp";
    }
    return "";
}

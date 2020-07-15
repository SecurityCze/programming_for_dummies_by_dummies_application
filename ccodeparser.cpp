#include "ccodeparser.h"

#include <QRegularExpression>
#include <QString>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <QDebug>

// int	msecsSinceStartOfDay() const
QString CCodeParser::Parse( const QString & codePath , const QString & taskPath )
{
    QFile fileToParse( codePath );
    if( ! fileToParse.exists() || ! fileToParse.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return "";

    QRegularExpression matchPattern( s_REG_EX_MAIN , QRegularExpression::DotMatchesEverythingOption );
    QRegularExpressionMatch mainMatch = matchPattern.match( fileToParse.readAll() );
    if( mainMatch.hasMatch() )
    {
        QFile newCode( taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + s_PROCESSED_CODE );
        if( newCode.exists() ) newCode.remove();
        newCode.open( QIODevice::WriteOnly | QIODevice::Text );
        QString funcName = "func" + QString::number(  QTime().currentTime().msecsSinceStartOfDay() );

        QTextStream ts( & newCode );
        InsertHeaders( ts , mainMatch.captured( 1 ) );
        RenameMain( ts , mainMatch.captured( 2 ) , funcName ); // this function serves to make func instead of main for no collisions
        InsertFooter( ts , mainMatch.captured( 3 ) );
        InsertMain( ts , funcName );
        newCode.close();
        return s_PROCESSED_CODE;
    }
    return "";
}

void CCodeParser::InsertHeaders( QTextStream & ts , const QString & header )
{
    ts << "#include <stdio.h>\n";
    ts << "#include <stdlib.h>\n";
    ts << "#include <csignal>\n";
    ts << header;
}

void CCodeParser::InsertMain( QTextStream & ts , const QString & funcName )
{
    ts << "\nvoid signal" << funcName << "( int signal )\n{\n\t( void )signal;\n\tfprintf( stderr , \"segfault\\n\" );\n\texit( 1 );\n}\n\n";
    ts << "int main( void )\n{\n\tsignal( SIGSEGV , signal" << funcName << " );\n\t" << funcName << "();\n\n\treturn 0;\n}\n\n";
}

void CCodeParser::InsertFooter( QTextStream & ts , const QString & footer )
{
    ts << footer;
}

void CCodeParser::RenameMain( QTextStream & ts , const QString & main , const QString & funcName )
{
    ts << "int " << funcName << "()\n{";
    ts << main;
    ts << "}\n";
}

#include "ccodeparser.h"

#include <QRegularExpression>
#include <QString>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

// int	msecsSinceStartOfDay() const
QString CCodeParser::Parse( const QString & codePath , const QString & taskPath , uint32_t timeLimit )
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
        ts.setCodec( QTextCodec::codecForName("UTF-8") );

        InsertHeaders( ts , mainMatch.captured( 1 ) , funcName , timeLimit );
        RenameMain( ts , mainMatch.captured( 2 ) , funcName ); // this function serves to make func instead of main for no collisions
        InsertFooter( ts , mainMatch.captured( 3 ) );
        InsertMain( ts , funcName , taskPath );
        newCode.close();
        return s_PROCESSED_CODE;
    }
    return "";
}

void CCodeParser::InsertHeaders( QTextStream & ts , const QString & header , const QString & funcName , uint32_t timeLimit )
{
    ts << "#include <stdio.h>\n";
    ts << "#include <stdlib.h>\n";
    ts << "#include <csignal>\n";
    ts << "#include <pthread.h>\n";
    ts << "#include <unistd.h>\n";

    ts << "\n";

    ts << "int " + funcName + "bFinished = 0;\n";
    ts << "int " + funcName + "timeLimit = " + QString::number( timeLimit ) + ";\n";

    ts << "\n";

    ts << "void signal" + funcName + "( int signal )\n";
    ts << "{\n";
    ts << "\t( void )signal;\n";
    ts << "\tfprintf( stderr , \"segfault\\n\" );\n";
    ts << "\texit( 1 );\n";
    ts << "}\n";

    ts << "\n";

    ts << header.toUtf8();
}

void CCodeParser::InsertMain( QTextStream & ts , const QString & funcName , const QString & taskPath )
{
    ts << "\n";

    ts << "void * t" + funcName + "( void * args )\n";
    ts << "{\n";
    ts << "\t" + funcName + "();\n";
    ts << "\t" + funcName + "bFinished = 1;\n";
    ts << "\treturn args;\n";
    ts << "}\n";

    ts << "\n";

    ts << "int main( void )\n";
    ts << "{\n";
    ts << "\tsignal( SIGSEGV , signal" + funcName + " );\n";

    ts << "\n";

    ts << "\tpthread_t thread_id;\n";

    ts << "\n";

    ts << "\tpthread_create( &thread_id , NULL , t" + funcName + " , NULL );\n";
    ts << "\twhile( " + funcName + "timeLimit )\n";
    ts << "\t{\n";
    ts << "\t\tif( " + funcName + "bFinished == 1 ) break;\n";
    ts << "\t\tusleep( 500000 );\n";
    ts << "\t\t" + funcName + "timeLimit -= 500;\n";
    ts << "\t}\n";

    ts << "\n";

    ts << "\tif( " + funcName + "bFinished == 1 )\n";
    ts << "\t{\n";
    ts << "\t\tFILE * fptr = fopen( \"" + taskPath + "/" + CConstants::s_SOURCE_FOLDER + "/" + "finished.txt\" , \"w\" );\n";
    ts << "\t\tif( fptr ) fclose( fptr );\n";
    ts << "\t}\n";
    ts << "\telse\n";
    ts << "\t\tpthread_cancel( thread_id );\n";

    ts << "\n";

    ts << "\treturn 0;\n";
    ts << "}\n";
}

void CCodeParser::InsertFooter( QTextStream & ts , const QString & footer )
{
    ts << footer.toUtf8();
}

void CCodeParser::RenameMain( QTextStream & ts , const QString & main , const QString & funcName )
{
    ts << "\n";

    ts << "int " + funcName + "( void )\n";
    ts << "{\n";

    ts << main.toUtf8();

    ts << "}\n";

    ts << "\n";
}

#ifndef CCOMPILER_H
#define CCOMPILER_H

#include <QFileDialog>

class CCompiler
{
public:
    enum COMP_STATES : int { COMP_AVAILABLE = 0 , COMP_MISSING , COMP_ERROR };

    CCompiler();
    CCompiler::COMP_STATES IsAvailable() const;

private:
    void CloseAndDelete( QFile & file ) const;
};

#endif // CCOMPILER_H

#ifndef CTASKNOLIMIT_H
#define CTASKNOLIMIT_H

#include <QThread>

#include "ctaskabstract.h"
#include "cmemdebugger.h"
#include "cconstants.h"

class CTaskNoLimit : public CTaskAbstract
{
public:
    CTaskNoLimit( const QString & testName , const QString & taskPath );
    CTaskState Process() override;

private:
    bool IsSegfault();
    bool IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const;
    void RemoveGarbage();
};

#endif // CTASKNOLIMIT_H

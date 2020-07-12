#ifndef CTASKNOLIMIT_H
#define CTASKNOLIMIT_H

#include "ctaskabstract.h"
#include "cmemdebugger.h"

class CTaskNoLimit : public CTaskAbstract
{
public:
    CTaskNoLimit( const QString & inputPath , const QString & referencePath );
    CTaskState::TASK_STATE Process() override;

private:
    bool IsSegfault();
    bool IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const;
    void RemoveGarbage();
};

#endif // CTASKNOLIMIT_H

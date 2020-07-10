#ifndef CTASKNOLIMIT_H
#define CTASKNOLIMIT_H

#include "ctaskabstract.h"

class CTaskNoLimit : public CTaskAbstract
{
public:
    CTaskNoLimit( const QString & inputPath , const QString & referencePath );
    CTaskState::TASK_STATE Process() override;
};

#endif // CTASKNOLIMIT_H

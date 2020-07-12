#ifndef CTASKTIMELIMITED_H
#define CTASKTIMELIMITED_H

#include "ctaskabstract.h"
#include "cmemdebugger.h"
#include <cstdint>

class CTaskTimeLimited : public CTaskAbstract
{
public:
    CTaskTimeLimited( const QString & inputPath , const QString & referencePath , uint32_t timeLimit );
    CTaskState::TASK_STATE Process() override;

    const uint32_t m_timeLimit = 0;
private:
    bool IsSegfault();
    bool IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const;
    void RemoveGarbage();
};

/*
#include "cmemdebugger.h"

class CTaskNoLimit : public CTaskAbstract
{
public:
    CTaskNoLimit( const QString & inputPath , const QString & referencePath );

private:

};
*/

#endif // CTASKTIMELIMITED_H

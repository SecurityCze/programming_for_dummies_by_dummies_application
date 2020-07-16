#ifndef CTASKTIMELIMITED_H
#define CTASKTIMELIMITED_H

#include "ctaskabstract.h"
#include "cmemdebugger.h"
#include <cstdint>

class CTaskTimeLimited : public CTaskAbstract
{
public:
    CTaskTimeLimited( const QString & inputPath , const QString & referencePath , uint32_t timeLimit );
    CTaskState Process() override;

    const uint32_t m_timeLimit = 0;
private:
    bool IsSegfault();
    bool IsValidMemResult( const CMemdebugger::deb_struct & memResult ) const;
    void RemoveGarbage();
};

#endif // CTASKTIMELIMITED_H

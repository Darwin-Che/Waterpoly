#ifndef _ACADEMICBUILDINGSTRATEGY_H_
#define _ACADEMICBUILDINGSTRATEGY_H_
#include "VisitStrategy.h"

class AcademicBuildingStrategy : public VisitStrategy {
  public:
    virtual void acceptVisitor(std::shared_ptr<Player> player,
            std::shared_ptr<Board> board,
            std::istream in, std::ostream out) override;
};

#endif


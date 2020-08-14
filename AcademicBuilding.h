#ifndef _ACADEMICBUILDING_H_
#define _ACADEMICBUILDING_H_
#include <vector>
#include "Building.h"
#include "Subject.h"

class AcademicBuilding : public Building, public Subject {
    int improvementCost;
    std::vector<int> improvementFee;
    int improvementLevel;

  public:
    AcademicBuilding(const std::string& name, int location, const std::string &description,
            int purchaseCost, bool isMortgaged, int improvementCost,
            const std::vector<int>& improvementFee);
    int getImprovementCost() const;
    int getImprovementFee() const;
    int getImprovementLevel() const;
    void setImprovementLevel(int improvementLevel);
    virtual int calculateNetworth() const;
    virtual std::string getInfo() const;
    virtual std::string saveInfo() const;
    virtual Info getSubjectInfo();
};

#endif


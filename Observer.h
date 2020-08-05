#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Building;
class AcademicBuilding;
class Player;

class Observer {
  public:
    // Pass the Subject that called the notify method.
    virtual void notify( Building & whoNotified ) = 0; 
    virtual void notify( AcademicBuilding & whoNotified ) = 0; 
    virtual void notify( Player & whoNotified ) = 0; 
};

#endif

#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

class Observer;

class Subject {
    std::vector<std::shared_ptr<Observer>> observers;
  protected: 
    virtual void notifyObservers();
  public:
    void attach(<std::shared_ptr<Observer> o);  
};

#endif

#include "Subject.h"
#include "Observer.h"

void Subject::attach(Observer *o) { observers.emplace_back(o); }


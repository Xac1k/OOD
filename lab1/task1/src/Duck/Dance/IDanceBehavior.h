#ifndef SIMUDUCK_DANCE_H
#define SIMUDUCK_DANCE_H

class IDanceBehavior {
public:
    virtual ~IDanceBehavior() = default;
    virtual void Dance() = 0;
};

#endif //SIMUDUCK_DANCE_H

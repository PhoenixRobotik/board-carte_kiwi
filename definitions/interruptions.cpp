#include "interruptions.h"


class Motor
: public InterruptListener
{
public:
    Motor();
    ~Motor();



    bool callback() {
        ...
    }


    {
        Timer1.subscribe(std::bind(&Motor::callback, this));


    }

};

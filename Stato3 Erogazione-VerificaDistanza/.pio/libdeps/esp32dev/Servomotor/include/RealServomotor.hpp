#ifndef REAL_SERVOMOTOR_HPP
#define REAL_SERVOMOTOR_HPP

#include "PwmOutInterface.hpp"
#include "Servomotor.hpp"

class RealServomotor : public Servomotor
{
    public:
        RealServomotor(PwmOutInterface &pwm_out);
        virtual void set_position_down();
    
    private:
        PwmOutInterface &pwm_out;
};

#endif
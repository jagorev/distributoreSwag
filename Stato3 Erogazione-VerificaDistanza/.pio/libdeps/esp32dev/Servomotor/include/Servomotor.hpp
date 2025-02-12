#ifndef SERVOMOTOR_HPP
#define SERVOMOTOR_HPP

#include "PwmOutInterface.hpp"
#include "ServomotorInterface.hpp"

class Servomotor : public ServomotorInterface
{
    public:
        Servomotor(PwmOutInterface &pwm_out);
        virtual void set_position_up();
        virtual void set_position_down();
        virtual void set_angle(const float angle_degree);
             
        float up_angle;
    
    private:
        PwmOutInterface &pwm_out;
};

#endif
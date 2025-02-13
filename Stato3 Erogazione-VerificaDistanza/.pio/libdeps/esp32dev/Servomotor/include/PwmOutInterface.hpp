#ifndef PWM_OUT_INTERFACE_HPP
#define PWM_OUT_INTERFACE_HPP

class PwmOutInterface
{
    public:
        virtual void pulsewidth_us(int us) = 0;    
};

#endif
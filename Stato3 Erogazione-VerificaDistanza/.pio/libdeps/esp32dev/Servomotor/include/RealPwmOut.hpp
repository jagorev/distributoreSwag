#include "PwmOutInterface.hpp"
#include "mbed.h"

class RealPwmOut : public PwmOutInterface
{
    public:
        RealPwmOut(PinName pin) :
            pwm_out(pin)
        {
        }
        
        virtual void pulsewidth_us(int us)
        {
            pwm_out.pulsewidth_us(us);
        }
               
    private:
        PwmOut pwm_out;
};
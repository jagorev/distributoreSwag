#include "RealServomotor.hpp"
#include "mbed.h"

RealServomotor::RealServomotor(PwmOutInterface &pwm_out) : 
    Servomotor(pwm_out),
    pwm_out(pwm_out)
{
}

void RealServomotor::set_position_down()
{
    set_angle(0);
    wait_ms(300);
    pwm_out.pulsewidth_us(0);
}

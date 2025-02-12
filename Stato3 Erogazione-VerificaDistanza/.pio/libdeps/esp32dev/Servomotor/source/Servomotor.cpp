#include "Servomotor.hpp"

Servomotor::Servomotor(PwmOutInterface &pwm_out) : 
    up_angle(90),
    pwm_out(pwm_out)
{
    
}

//Angle 0 degree = duty cycle 500 ns
//Angle 90 degree = duty cycle 1500 ns
//Angle 180 degree = duty cycle 2500 ns
void Servomotor::set_angle(const float angle_degree)
{
    const float duty_cycle_us = (angle_degree / 180.0) * 2000 + 500;
    pwm_out.pulsewidth_us(duty_cycle_us);
}

void Servomotor::set_position_up()
{
    set_angle(up_angle);
}

void Servomotor::set_position_down()
{
    set_angle(up_angle - 90);
}

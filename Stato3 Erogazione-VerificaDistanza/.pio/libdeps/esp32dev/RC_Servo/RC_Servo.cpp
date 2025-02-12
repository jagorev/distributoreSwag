#include "RC_Servo.h"

RC_Servo::RC_Servo(PinName PWM, int _extended) : _RCpwm(PWM)
{
    if (_extended) {
        _RCpMin = 400;
        _RCpMax = 2400;
    } else {
        _RCpMin = 1000;
        _RCpMax = 2000;
    }
}

int RC_Servo::setLimits (int Tmin, int Tmax)
{
    if ((Tmin > 400) && (Tmin < Tmax))  _RCpMin = Tmin;
    else return Tmin;
    if ((Tmax < 2400) && (Tmin < Tmax)) _RCpMax = Tmax;
    else return Tmax;
    return 0;
}

void RC_Servo::write (double position)
{
    if ((position >= 0) && (position <= 1))
        _RCpwm.pulsewidth_us (_RCpMin + (long)((_RCpMax - _RCpMin) * position));
}

RC_Servo& RC_Servo::operator= (double position)
{
    write (position);
    return *this;
}

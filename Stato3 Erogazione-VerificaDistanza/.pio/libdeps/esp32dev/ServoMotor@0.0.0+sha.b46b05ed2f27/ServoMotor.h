#ifndef SERVOMOTOR
#define SERVOMOTOR

#include "mbed.h"

class SERVO_MOTOR{

    public:
    SERVO_MOTOR(PinName pin, 
                double angle0 = 0, double PulseWidth0 = 1200,
                double angle1 = 180, double PulseWidth1 = 1800);
    void move(double angle, bool sync = true);
    void go(double angle);
    
    bool tick(void);
    void set_speed(double speed);
    double get_angle(void);
    
    private:
    PwmOut _pwm;
    double _angle0;
    double _angle1;
    double _PulseWidth0_us;
    double _PulseWidth1_us;
    double _speed;
    double _angle_command;
    double _angle_now;
    double _time;
    Timer _timer;
    
};

#endif
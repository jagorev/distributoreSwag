#include "ServoMotor.h"
#include "mbed.h"

SERVO_MOTOR :: SERVO_MOTOR(PinName pin, 
            double angle0, double PulseWidth0_us, 
            double angle1, double PulseWidth1_us) : _pwm(pin) {

    if(angle0 < angle1){
        _angle0 = angle0;
        _angle1 = angle1;
        _PulseWidth0_us = PulseWidth0_us;
        _PulseWidth1_us = PulseWidth1_us;
    }else{
        _angle0 = angle1;
        _angle1 = angle0;
        _PulseWidth0_us = PulseWidth1_us;
        _PulseWidth1_us = PulseWidth0_us;
    }
    _pwm.period_ms(20);// pulse cycle = 20ms
    _speed = 10;         //5deg/s
    _timer.start();
    if(_angle0 <= 0 && 0 <= _angle1){
        _angle_now = 0;
    }else{
        _angle_now = (angle0+angle1)/2.0;
    }

}

void SERVO_MOTOR :: set_speed(double speed){
    _speed = speed;
}

double SERVO_MOTOR :: get_angle(void){
    return _angle_now;
}

bool SERVO_MOTOR :: tick(void){

    double PulseWidth_us;
    double time_now;
    
    if(_time > _timer.read()){
        return true;
    }

    if(_angle_now == _angle_command){
        return false;
    }
    
    time_now = _timer.read();
    
    if(_angle_command > _angle_now){
        if(_angle_command > _angle_now + _speed*(time_now - _time)){
            _angle_now += _speed*(time_now - _time);
        }else{
            _angle_now = _angle_command;
        }
    }else{
        if(_angle_command < _angle_now - _speed*(time_now - _time)){
            _angle_now -= _speed*(time_now - _time);
        }else{
            _angle_now = _angle_command;
        }
    }

    _time = time_now;

    PulseWidth_us = (_PulseWidth1_us - _PulseWidth0_us)/(_angle1 - _angle0) * (_angle_now - _angle0) + _PulseWidth0_us;    
    _pwm.pulsewidth(PulseWidth_us * 1e-6);    

    return true;
    
}

void SERVO_MOTOR :: move(double angle, bool sync){    

    if(angle < _angle0){
        angle = _angle0;
    }
    if(angle > _angle1){
        angle = _angle1;
    }    

    _angle_command = angle;
    _timer.reset();
    _time = _timer.read();

    if(sync){
        while(tick());
    }

}

void SERVO_MOTOR :: go(double angle){
    double PulseWidth_us;
    _angle_command = angle;    
    _angle_now = _angle_command;
    PulseWidth_us = (_PulseWidth1_us - _PulseWidth0_us)/(_angle1 - _angle0) * (_angle_now - _angle0) + _PulseWidth0_us;    
    _pwm.pulsewidth(PulseWidth_us * 1e-6);
}
/**
 * @author Hugues Angelis
 *
 * @section LICENSE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Any classic RC Servomotor (ie : Analog) Library
 *
 *  This library will operate any servo using a simple PWM to be driven.
 *
 *  The library générate a PWM signal with a period of 20ms
 *
 *  @note It may operate in classic mode (eg : min = 1ms, max = 2ms) or in extended mode (eg : Min = 400us, Max = 2,4ms) if you set _extended. Other value can be set
 */

#ifndef Servo_H
#define Servo_H

/**
 * Includes : Mbed Library
 */
#include "mbed.h"
#include "SoftPWM.h"

/**
 * RC_Servo
 * Analog servomotor (without external feedback)
 * More information : http://www.digitalspirit.org/wiki/docs/servo-moteurs
 */
class RC_Servo
{

public :

    /**
     * Constructor of a RC Servo (analog).
     *
     * @param PWM (PinName) : is the Mbed pin used to generate the PWM signal
     * @param _extended (int, = 0) : if set let servo use modified pulsewidth limits
     */
    RC_Servo(PinName PWM, int _extended = 0);

    /**
     * Set the min and max pulsewidth time associated with both extreme position
     *
     * @param Tmin : pulsewidth (in us) associated with Min position (must be more than 400)
     * @param Tmax : pulsewidth (in us) associated with Max position (must be less than 2400)
     * @return 0 if OK, any other value if FAIL
     */
    int setLimits (int Tmin, int Tmax);

    /**
     * Set the position of the Servo
     *
     * @param position : float number between 0 and 1 (0 = Min, 1 = Max)
     */
    void write (double position);

    /**
     * A short hand for write
     */
    RC_Servo& operator= (double position);

private :
    int         _RCpMin, _RCpMax;
    SoftPWM     _RCpwm;

protected :

};
#endif //GP2A_H
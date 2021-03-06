/*********************************************************************************
  Copyright (c) 2017 ChedaDSP@Makers-NS. All right reserved.

  esp2866 as web controller for MikroE Buggy

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*****************************************************************************/
#include "buggy.h"

void setupBuggy()
{
  // We will use these 4 output PWMs
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  
  stopBuggy();
}

void stopBuggy()
{
  analogWrite(D1, 0);
  analogWrite(D2, 0);
  analogWrite(D3, 0);
  analogWrite(D4, 0);
}

/* Routine that updates 4 PWM motor drivers based on
 * position of a virtual joystick.
 * 
 * direction: -100 - 100
 * throttle: -100 - 100
 */
void moveBuggy(int direction, int throttle)
{
  int movingForward = (throttle > 0);

  if(!movingForward)
    throttle = - throttle; // we should go in reverse -> flip the sign
  if(throttle < THROTTLE_THRESHOLD)
  {
    stopBuggy();
    return;
  }

  int leftPWM = map(throttle, THROTTLE_THRESHOLD, THROTTLE_MAX, PWM_MIN, PWM_MAX);
  int rightPWM = leftPWM;
  
  if(direction > DIRECTION_THRESHOLD)
    rightPWM = 0;
  if(direction < - DIRECTION_THRESHOLD)
    leftPWM = 0;

  leftPWM = constrain(leftPWM, 0, PWM_MAX);
  rightPWM = constrain(rightPWM, 0, PWM_MAX);
  
  String m = "d ";
  Serial.println(m + direction + " t " + throttle + "\nL " + leftPWM + " R " + rightPWM);

  // Move
  if(movingForward)
  {
    analogWrite(D1, 0);
    analogWrite(D2, leftPWM);
    analogWrite(D3, 0);
    analogWrite(D4, rightPWM);
  }
  else{
    analogWrite(D1, leftPWM);
    analogWrite(D2, 0);
    analogWrite(D3, rightPWM);
    analogWrite(D4, 0);
  }
}


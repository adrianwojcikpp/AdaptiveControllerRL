/**
  ******************************************************************************
  * @file           : /VisualDSP/Torque Drive AW/AW_PID_SpeedController.h
  * @author         : Adrian Wojcik, MSc
  * @version        : V1.0
  * @date           : 04-Dec-2019
  * @brief          : Linear (PID) speed controller
  ******************************************************************************
  */
     
#ifndef __AW_SPEEDPIDRCONTROLLER_H_
#define __AW_SPEEDPIDRCONTROLLER_H_

float pid_speed_reg(float w_ref, float w_meas);

#endif // __AW_SPEEDPIDRCONTROLLER_H_

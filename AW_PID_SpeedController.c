/**
  ******************************************************************************
  * @file           : /VisualDSP/Torque Drive AW/AW_PID_SpeedController.c
  * @author         : Adrian Wojcik, MSc
  * @version        : V1.0
  * @date           : 04-Dec-2019
  * @brief          : Linear (PID) speed controller
  ******************************************************************************
  */

/* AW -- Function declarations --------------------------------- */
float AW_speed_pid_reg(float AW_w_ref, float AW_w_meas);
float AW_speed_filter(float AW_w_meas);

/* AW -- Function definitions ---------------------------------- */
float AW_speed_pid_reg(float AW_w_ref, float AW_w_meas)
{
	/* PID gains */
	float AW_Kp = (0.6f * AW_SPEED_REG_J_mean) / (2.0f*AW_SPEED_REG_K_T * AW_SPEED_REG_TauI);
	float AW_Ki = 1.0f / ((4.0f * AW_SPEED_REG_TauI );
	float AW_Kd = 0.0f;
	
	/* input signals */
	float AW_w_meas_filter;
	float AW_e_w  = 0.0;
	float AW_de_w = 0.0;
	
	/* output signal */
	float AW_iqRef = 0;
	float AW_iqRef_sat = 0;

	/* Error computation */
	AW_e_w = AW_w_ref - AW_w_meas;
	
	AW_de_w = AW_e_w - AW_e_w_old;
	AW_e_w_old = AW_e_w;
	
	AW_e_w_integral += (Ts)*(AW_e_w + (1.0/AW_Kp)*AW_delta_iqRef);
		
	// PID controller
	AW_iqRef = AW_Kp*(AW_e_w + AW_Ki*AW_e_w_integral + AW_Kd*AW_de_w);
	AW_iqRef_sat = AW_iqRef;
	
	// Output saturation
	if( AW_iqRef > CURRENT_LIMIT){
		AW_iqRef_sat = CURRENT_LIMIT;
	}
	else if ( AW_iqRef < -CURRENT_LIMIT ){
		AW_iqRef_sat = -CURRENT_LIMIT;
	}
	
	// Anti windup
	AW_delta_iqRef = AW_iqRef_sat - AW_iqRef;
	
	// Return value
	return AW_iqRef_sat;
}

float AW_speed_filter(float AW_w_meas)
{
	/* input signals */
	float AW_w_meas_filter;

	/* Speed measurement filter */
	AW_w_meas_filter = AW_w_meas;

	switch( AW_SPEED_REG_FILTER_TYPE ){
		case FIR: AW_w_meas_filter =    fir(AW_w_meas_filter, AW_SPEED_REG_FILTER_COEFFS, AW_SPEED_REF_FILTER_STATE, AW_SPEED_REF_FILTER_ORDER + 1 ); 
			break;		
		case IIR: AW_w_meas_filter = biquad(AW_w_meas_filter, AW_SPEED_REG_FILTER_COEFFS, AW_SPEED_REF_FILTER_STATE, AW_SPEED_REG_FILLTER_SECTIONS); 
			break;
		default: break; 
	};
		
	// Return value
	return AW_w_meas_filter;
}

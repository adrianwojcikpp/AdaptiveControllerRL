/**
  ******************************************************************************
  * @file           : /VisualDSP/Torque Drive AW/AW_RL_ANN_SpeedController.c
  * @author         : Adrian Wojcik, MSc
  * @version        : V1.0
  * @date           : 11-Dec-2019
  * @brief          : RL (reinforcement learning) ANN speed controller
  ******************************************************************************
  */

float AW_speed_neural_reg(float AW_w_ref, float AW_w_meas, float AW_q_meas, mat_type* AW_ann_dsp_INPUT_old, mat_type* AW_ann_dsp_OUTPUT_old)
{
	int i; 
	
	// Format input vector	
	ann_dsp_INPUT.pData[0] = AW_w_ref;
	ann_dsp_INPUT.pData[1] = AW_w_meas;
	ann_dsp_INPUT.pData[2] = AW_q_meas;
	ann_dsp_INPUT.pData[3] = ANN_w_meas_old1;
	ann_dsp_INPUT.pData[4] = ANN_w_meas_old2;
	ann_dsp_INPUT.pData[5] = ANN_w_meas_old3;
	
	// copy old samples
	ANN_w_meas_old3 = ANN_w_meas_old2;
	ANN_w_meas_old2 = ANN_w_meas_old1;
	ANN_w_meas_old1 = AW_w_meas;
	
	// Compute output 
	ann_compute_output(&ann_dsp, &ann_dsp_INPUT, &ann_dsp_OUTPUT);
	
	// PI output
	AW_ANN_OUTPUT_P = ann_dsp_OUTPUT.pData[0];
	AW_ANN_OUTPUT_I += (Ts)*(ann_dsp_OUTPUT.pData[1] + AW_ANN_OUTPUT_DELTA);
	
	AW_ANN_OUTPUT = AW_ANN_OUTPUT_P + AW_ANN_Ki*AW_ANN_OUTPUT_I;
	AW_ANN_OUTPUT_SAT = AW_ANN_OUTPUT;
	
	// Output saturation
	if( AW_ANN_OUTPUT > AW_ANN_OUTPUT_LIMIT)
	{
		AW_ANN_OUTPUT_SAT = AW_ANN_OUTPUT_LIMIT;
	}
	else if ( AW_ANN_OUTPUT < -AW_ANN_OUTPUT_LIMIT )
	{
		AW_ANN_OUTPUT_SAT = -AW_ANN_OUTPUT_LIMIT;
	}
	
	// Anti windup
	AW_ANN_OUTPUT_DELTA = AW_ANN_OUTPUT_SAT - AW_ANN_OUTPUT;
	
	// SEMAPHORE - copy input data to update procedure
	if(AW_ANN_Semaphore == 0)
	{
		for(i = 0; i < ann_dsp_INPUT_SIZE; i++)
			AW_ann_dsp_INPUT_old->pData[i] = ann_dsp_INPUT.pData[i];
			
		for(i = 0; i < ann_dsp_OUTPUT_SIZE; i++)
			AW_ann_dsp_OUTPUT_old->pData[i] = ann_dsp_OUTPUT.pData[i];
	}
	
	return AW_ANN_OUTPUT_SAT;
}

void AW_speed_neural_reg_update(mat_type* AW_ann_dsp_INPUT_old, mat_type* AW_ann_dsp_OUTPUT_old)
{
	AW_ANN_Semaphore = 1;
	scalar_type AW_ANN_e[ann_dsp_OUTPUT_SIZE] = {0.0 , 0.0};
	mat_type AW_ANN_E = { ann_dsp_OUTPUT_SIZE, 1, AW_ANN_e };
	
	// Compute critic
	AW_ANN_E.pData[0] = (AW_ann_dsp_INPUT_old->pData[0] - AW_ann_dsp_INPUT_old->pData[1]) - AW_ANN_Ec*AW_ann_dsp_OUTPUT_old->pData[0];
	AW_ANN_E.pData[1] = (AW_ann_dsp_INPUT_old->pData[0] - AW_ann_dsp_INPUT_old->pData[1]) - AW_ANN_Ec*AW_ann_dsp_OUTPUT_old->pData[1];
	
	// Perform backprop
	ann_backpropagation(&ann_dsp, AW_ann_dsp_INPUT_old, &AW_ANN_E, AW_ANN_n);
	AW_ANN_Semaphore = 0;
}


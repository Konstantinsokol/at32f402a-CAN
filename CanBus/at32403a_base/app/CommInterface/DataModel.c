/*
 * UART.c
 *
 *  Created on: 2 июл. 2018 г.
 *      Author: Andrey
 */
#include <DataModel.h>
/*EXTERNALS*/
motorSettings_st motorSettings;

void SaveByHand(uint16_t ok);
uint8_t reset_active = 0;


inline void Motor_screen(Message_st *);
inline void Live_screen(Message_st *);
inline void Drive_screen(Message_st *message);
inline void Battery_screen(Message_st *message);
inline void Log_screen(Message_st *message);
inline void Log_settings(Message_st *message);
inline void Save_screen(Message_st *message);
inline void Ident_screen(Message_st *message);
inline void Reg_screen(Message_st*message);
inline void Info_screen(Message_st*message);
inline void Inputs_screen(Message_st*message);
inline void Buttons_screen(Message_st*message);
inline void CAN_screen(Message_st*message);
inline void SensorLess_screen(Message_st*message);
inline void Analog_screen(Message_st*message);
inline void Functions_screen(Message_st*message);

void Process_screen()
{
	union message_un *ptrMessage = getMessagePrt();
	switch (ptrMessage->message_layer.screen_num)
	{
		case LIVE_SCREEN: //Live update screen
		{
			Live_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case MOTOR_SCREEN: //Motor settings screen
		{
			Motor_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case DRIVE_SCREEN: //Drive screen
		{
			Drive_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case IDENT_SCREEN: //Ident screen
		{
			Ident_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case BATTERY_SCREEN: //Battery screen
		{
			Battery_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case REG_SCREEN: //Regulator screen
		{
			Reg_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case INPUTS_SCREEN: //Inputs screen
		{
			Inputs_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case CAN_SCREEN:// CAn screen
		{
			CAN_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case SENSORLESS_SCREEN://SensorlEss screen
		{
			SensorLess_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case ANALOG_SCREEN:// Analog screen
		{
			Analog_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case FUNCTIONS_SCREEN://Functions screen
		{
			Functions_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}

		case BUTTONS_SCREEN:
		{
			Buttons_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case LOGSETINNGS_SCREEN://Logsettings screen
		{
			Log_settings(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case INFO_SCREEN://INfo screen
		{
			Info_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case SAVE_SCREEN://Save to flash screen
		{
			Save_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
		case LOGREAD_SCREEN://LOg reading screen
		{
			Log_screen(&ptrMessage->message_layer);
			sendData(&ptrMessage->message_layer);
			break;
		}
	}//end of switch

}
//ToDo Values range control must be implemented on controller side
inline void Motor_screen(Message_st *message)
{

	if(message->cmd == 0) /*Reading*/{
		message->data_buffer[0] = CO_OD_ROM.motor_Data.polePairs;
		message->data_buffer[1] = CO_OD_ROM.restrictParams.twin_max;
		message->data_buffer[2] = CO_OD_ROM.restrictParams.twin_Zero;
		message->data_buffer[3] = CO_OD_ROM.motor_Data.wnom;
		message->data_buffer[4] = CO_OD_ROM.motor_Data.direction;
		message->data_buffer[5] = CO_OD_ROM.hardWare.temp_sensor;
		message->data_buffer[6] = CO_OD_ROM.hardWare.position_sensor;
		message->data_buffer[7] = CO_OD_ROM.motor_Data.rs *1000.f;
		message->data_buffer[8] = CO_OD_ROM.motor_Data.ls *1e6f;
		message->data_buffer[9]= CO_OD_ROM.motor_Data.flux_nom *1e5f;
		message->data_buffer[10]=((int16_t) (CO_OD_ROM.motor_Data.phase_corr*1000.f));
		message->data_buffer[11]= CO_OD_ROM.hardWare.pulses_perRev;
		message->data_buffer[12]= ((int16_t) (CO_OD_ROM.hardWare.I_kv*1000.f));
		message->data_buffer[13]= ((int16_t) (CO_OD_ROM.hardWare.I_shift*1.f));
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.motor_Data.polePairs = message->data_buffer[0];
		CO_OD_ROM.restrictParams.twin_max = message->data_buffer[1];
		CO_OD_ROM.restrictParams.twin_Zero = message->data_buffer[2];
		CO_OD_ROM.motor_Data.wnom = message->data_buffer[3];
		CO_OD_ROM.motor_Data.direction = message->data_buffer[4];
		CO_OD_ROM.hardWare.temp_sensor = message->data_buffer[5];
		CO_OD_ROM.hardWare.position_sensor = message->data_buffer[6];
		CO_OD_ROM.motor_Data.rs = message->data_buffer[7] *1e-3f;
		CO_OD_ROM.motor_Data.ls = message->data_buffer[8] *1e-6f;
		CO_OD_ROM.motor_Data.flux_nom = message->data_buffer[9] *1e-5f;
		CO_OD_ROM.motor_Data.phase_corr =((int16_t)message->data_buffer[10]) * 1e-3f;
		CO_OD_ROM.hardWare.pulses_perRev = message->data_buffer[11];
		CO_OD_ROM.hardWare.I_kv = ((int16_t)message->data_buffer[12]) * 1e-3f;
		CO_OD_ROM.hardWare.I_shift = ((int16_t)message->data_buffer[13]) * 1.f;
		message->cmd = OK;
	}
	message->screen_num = MOTOR_SCREEN;

}
inline void Live_screen(Message_st *message)
{
	if(message->cmd == 0) /*Reading*/{
		message->data_buffer[0] = BLDC_drive.Udc*100.f;
		message->data_buffer[1] = BLDC_drive.Uz*100.f;
		message->data_buffer[2] = (int16_t)BLDC_drive.Iamp;
		message->data_buffer[3] = (int16_t)BLDC_drive.W_rot_el;
		message->data_buffer[4] = (int16_t)mod(BLDC_drive.P_el);
		message->data_buffer[5] = (int16_t)max(BLDC_drive.Tboard2,BLDC_drive.Tboard);
		message->data_buffer[6] = BLDC_drive.Psi_calc_f*1e4f;
		message->data_buffer[7] = IO.Brake_Act*1000.f;
		message->data_buffer[8] = (int16_t)(BLDC_drive.PhaseCalulated*1000.f);
		message->data_buffer[9]=  (int16_t)BLDC_drive.Twin;
		message->data_buffer[10]= IO.Hall_cnt;
		message->data_buffer[11]= IO.Trottle_Act*1000.f;
		message->data_buffer[12]= BLDC_drive.stat_word;
		message->data_buffer[13]= BLDC_drive.errors_word;
	}
	message->screen_num = LIVE_SCREEN;
}
inline void Drive_screen(Message_st *message)
{

	if(message->cmd == 0) /*Reading*/{
		message->data_buffer[0] = CO_OD_ROM.controlParams.torque_ramp_up;
		message->data_buffer[1] = CO_OD_ROM.controlParams.torque_ramp_down;
		message->data_buffer[2] = CO_OD_ROM.controlParams.torque_current_max;
		message->data_buffer[3] = CO_OD_ROM.controlParams.brake_ramp_up;
		message->data_buffer[4] = CO_OD_ROM.controlParams.brake_ramp_down;
		message->data_buffer[5] = CO_OD_ROM.controlParams.brake_current_max;
		message->data_buffer[6] = CO_OD_ROM.controlParams.DBrake_ramp_up;
		message->data_buffer[7] = CO_OD_ROM.controlParams.DBrake_ramp_down;
		message->data_buffer[8]	= CO_OD_ROM.controlParams.DBrake_current_max;
		message->data_buffer[9]	= CO_OD_ROM.command_word_static;
		message->data_buffer[10]= CO_OD_ROM.controlParams.autoBrakeCurrent;
		message->data_buffer[11]= CO_OD_ROM.controlParams.control_frec;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;

	}
	if(message->cmd ==1) /*Writing */
	{

		CO_OD_ROM.controlParams.torque_ramp_up = message->data_buffer[0];
		CO_OD_ROM.controlParams.torque_ramp_down = message->data_buffer[1];
		CO_OD_ROM.controlParams.torque_current_max = message->data_buffer[2];
		CO_OD_ROM.controlParams.brake_ramp_up = message->data_buffer[3];
		CO_OD_ROM.controlParams.brake_ramp_down = message->data_buffer[4];
		CO_OD_ROM.controlParams.brake_current_max = message->data_buffer[5];
		CO_OD_ROM.controlParams.DBrake_ramp_up = message->data_buffer[6];
		CO_OD_ROM.controlParams.DBrake_ramp_down = message->data_buffer[7];
		CO_OD_ROM.controlParams.DBrake_current_max = message->data_buffer[8];
		CO_OD_ROM.command_word_static = message->data_buffer[9];
		CO_OD_ROM.controlParams.autoBrakeCurrent = message->data_buffer[10];
		CO_OD_ROM.controlParams.control_frec = message->data_buffer[11];
		message->cmd = OK;
	}
	message->screen_num = DRIVE_SCREEN; /*Drive screen*/
}
inline void Ident_screen(Message_st *message)
{

	if(message->cmd == 0) /*Reading*/{
		message->data_buffer[0] = CO_OD_ROM.HAll_Array[0];
		message->data_buffer[1] = CO_OD_ROM.HAll_Array[1];
		message->data_buffer[2] = CO_OD_ROM.HAll_Array[2];
		message->data_buffer[3] = CO_OD_ROM.HAll_Array[3];
		message->data_buffer[4] = CO_OD_ROM.HAll_Array[4];
		message->data_buffer[5] = CO_OD_ROM.HAll_Array[5];
		message->data_buffer[6] = BLDC_drive.Rs *1e3;
		message->data_buffer[7] = BLDC_drive.Ls *1e6;
		message->data_buffer[8] = BLDC_drive.Psi_nom *1e4;
		message->data_buffer[9] = BLDC_drive.cmd.bit_DETECT_MODE;
		message->data_buffer[10]= CO_OD_RAM.identCurreltLevel[0];
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= BLDC_drive.cmd.bit_DETECT_MODE;;
	}
	if(message->cmd ==1) /*Writing */
	{

		if(message->data_buffer[0] == 1) /*Ident Rs*/
		{
			BLDC_drive.cmd.bit_DETECT_MODE = RS;
		}
		if(message->data_buffer[0] == 2) /*Ident Ls*/
		{
			BLDC_drive.cmd.bit_DETECT_MODE = LS;
		}
		if(message->data_buffer[0] == 3) /*Ident Hall Static*/
		{
			BLDC_drive.cmd.bit_DETECT_MODE = HALL_STEP;
		}
		if(message->data_buffer[0] == 4) /*Ident Hall Rotation*/
		{
			BLDC_drive.cmd.bit_DETECT_MODE = HALL_ROTARY;
		}
		if(message->data_buffer[0] == 5) /*Ident Emf*/
		{
			BLDC_drive.cmd.bit_DETECT_MODE = EMF;
		}
		if(message->data_buffer[0] == 6) /*Manual Hall Comm setting*/
		{
			CO_OD_ROM.HAll_Array[0] = message->data_buffer[4];
			CO_OD_ROM.HAll_Array[1] = message->data_buffer[5];
			CO_OD_ROM.HAll_Array[2] = message->data_buffer[6];
			CO_OD_ROM.HAll_Array[3] = message->data_buffer[7];
			CO_OD_ROM.HAll_Array[4] = message->data_buffer[8];
			CO_OD_ROM.HAll_Array[5] = message->data_buffer[9];

		}
		/*Установка тока идентификации*/
		CO_OD_RAM.identCurreltLevel[0] = message->data_buffer[1];

		message->data_buffer[0] = CO_OD_ROM.HAll_Array[0];
		message->data_buffer[1] = CO_OD_ROM.HAll_Array[1];
		message->data_buffer[2] = CO_OD_ROM.HAll_Array[2];
		message->data_buffer[3] = CO_OD_ROM.HAll_Array[3];
		message->data_buffer[4] = CO_OD_ROM.HAll_Array[4];
		message->data_buffer[5] = CO_OD_ROM.HAll_Array[5];
		message->data_buffer[6] = BLDC_drive.Rs *1e3;
		message->data_buffer[7] = BLDC_drive.Ls *1e6;
		message->data_buffer[8] = BLDC_drive.Psi_nom *1e4;
		message->data_buffer[9] = BLDC_drive.cmd.bit_DETECT_MODE;
		message->data_buffer[10]= CO_OD_RAM.identCurreltLevel[0];
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;
		message->data_buffer[9] = BLDC_drive.cmd.bit_DETECT_MODE;
		message->cmd = OK;
	}
	message->screen_num = IDENT_SCREEN; /*Ident screen*/
}
inline void Battery_screen(Message_st *message)
{
	if(message->cmd == 0) /*Reading*/{
			message->data_buffer[0] = CO_OD_ROM.restrictParams.U_high;
			message->data_buffer[1] = CO_OD_ROM.restrictParams.U_low;
			message->data_buffer[2] = CO_OD_ROM.controlParams.battery_curr_drv;/*Max Battery discharge current*/
			message->data_buffer[3] = CO_OD_ROM.controlParams.battery_curr_gen;/*Max Dattery charge current*/
			message->data_buffer[4] = CO_OD_ROM.controlParams.pmax_drive;
			message->data_buffer[5] = CO_OD_ROM.restrictParams.U_low + CO_OD_ROM.restrictParams.U_low_diap;
			message->data_buffer[6] = 0;/*Resistance*/
			message->data_buffer[7] = 0;
			message->data_buffer[8] = 0;
			message->data_buffer[9] = 0;
			message->data_buffer[10]= 0;
			message->data_buffer[11]= 0;
			message->data_buffer[12]= 0;
			message->data_buffer[13]= 0;
		}
		if(message->cmd ==1) /*Writing */
		{

			CO_OD_ROM.restrictParams.U_high = message->data_buffer[0];
			CO_OD_ROM.restrictParams.U_low = message->data_buffer[1];
			CO_OD_ROM.controlParams.battery_curr_drv = message->data_buffer[2];
			CO_OD_ROM.controlParams.battery_curr_gen = message->data_buffer[3];
			CO_OD_ROM.controlParams.pmax_drive = message->data_buffer[4];
			CO_OD_ROM.restrictParams.U_low_diap = message->data_buffer[5]-CO_OD_ROM.restrictParams.U_low;
			//message->data_buffer[6];
			message->cmd = OK;
		}
		message->screen_num = BATTERY_SCREEN;
}
inline void Reg_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CO_OD_ROM.idCurrentKoef[0];
		message->data_buffer[1] = CO_OD_ROM.speedRegKoef[0]*1e4;
		message->data_buffer[2] = CO_OD_ROM.speedRegKoef[1]*1e4;
		message->data_buffer[3] = CO_OD_ROM.FW_regKoef[0]*1e4;
		message->data_buffer[4] = CO_OD_ROM.FW_regKoef[1]*1e4;
		message->data_buffer[5] = mod(CO_OD_ROM.controlParams.id_min);
		message->data_buffer[6] = CO_OD_ROM.controlParams.mainGear;
		message->data_buffer[7] = CO_OD_ROM.controlParams.secondGear;
		message->data_buffer[8] = CO_OD_ROM.controlParams.wheel_diameter_mm;
		message->data_buffer[9] = CO_OD_ROM.controlParams.speed_ramp_up;
		message->data_buffer[10]= CO_OD_ROM.controlParams.speed_ramp_down;
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.idCurrentKoef[0] = message->data_buffer[0];
		CO_OD_ROM.speedRegKoef[0] = message->data_buffer[1] * 1e-4f;
		CO_OD_ROM.speedRegKoef[1] = message->data_buffer[2] * 1e-4f;
		CO_OD_ROM.FW_regKoef[0] = message->data_buffer[3] * 1e-4f;
		CO_OD_ROM.FW_regKoef[1] = message->data_buffer[4] * 1e-4f;
		CO_OD_ROM.controlParams.id_min = ((int16_t)message->data_buffer[5]*1.f);
		/*Drive giar and wheel*/
		CO_OD_ROM.controlParams.mainGear = message->data_buffer[6];
		CO_OD_ROM.controlParams.secondGear = message->data_buffer[7];
		CO_OD_ROM.controlParams.wheel_diameter_mm = message->data_buffer[8];
		CO_OD_ROM.controlParams.speed_ramp_up = message->data_buffer[9];
		CO_OD_ROM.controlParams.speed_ramp_down = message->data_buffer[10];
		message->cmd = OK;
	}
	message->screen_num = REG_SCREEN; /*Regulators screen*/
}
inline void Inputs_screen(Message_st*message)
{
	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = IO.Udc_ADC;
		message->data_buffer[1] = IO.Ia_ADC;
		message->data_buffer[2] = IO.Ic_ADC;
		message->data_buffer[3] = IO.AB_cnt;
		message->data_buffer[4] = IO.PWM_cnt;
		message->data_buffer[5] = IO.IN_1;
		message->data_buffer[6] = IO.IN_2;
		message->data_buffer[7] = IO.IN_3;
		message->data_buffer[8] = IO.IN_4;
		message->data_buffer[9] = IO.IN_CAN;
		message->data_buffer[10]= IO.Hall_cnt;
		message->data_buffer[11]= IO.Trottle_Act*1000;
		message->data_buffer[12]= IO.Brake_Act*1000;
		message->data_buffer[13]= (uint16_t) (IO.Usupply*100.f);
	}
	message->screen_num = INPUTS_SCREEN; /*Inputs screen*/
}

inline void Info_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CompileTime.kvu_time_stamp &0xFFFF; //2
		message->data_buffer[1] = (CompileTime.kvu_time_stamp >>16) & 0XFFFF; //4
		message->data_buffer[2] = Controller_Base.MAX_current;//6
		message->data_buffer[3] = Controller_Base.MAX_voltage;//8
		message->data_buffer[4] = CO_OD_ROM.controlParams.control_frec;//10
		message->data_buffer[5] = Controller_Base.OTPusedСode;//12
		message->data_buffer[6] = 0;//14
		message->data_buffer[7] = 0;//16
		message->data_buffer[8] = 0;//18
		message->data_buffer[9] = 0;//20
		message->data_buffer[10]= 0;//22
		message->data_buffer[11]= 0;//24
		message->data_buffer[12]= CO_OD_RAM.MCU_ID & 0xFFFF;
		message->data_buffer[13]= (CO_OD_RAM.MCU_ID >> 16) & 0xFFFF;
	}
	message->screen_num = INFO_SCREEN; /*Info screen*/
}
inline void CAN_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CO_OD_ROM.CANNodeID;
		message->data_buffer[1] = CO_OD_ROM.CANBitRate;
		message->data_buffer[2] = CO_OD_ROM.hardWare.control_source & 0x1;
		message->data_buffer[3] = 0;
		message->data_buffer[4] = 0;
		message->data_buffer[5] = 0;
		message->data_buffer[6] = 0;/*Resistance*/
		message->data_buffer[7] = 0;
		message->data_buffer[8] = 0;
		message->data_buffer[9] = 0;
		message->data_buffer[10]= 0;
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.CANNodeID = message->data_buffer[0]&0x7ff;
		CO_OD_ROM.CANBitRate = message->data_buffer[1];
		CO_OD_ROM.hardWare.control_source = message->data_buffer[2] & 0x1;
		message->cmd = OK;
	}
	message->screen_num = CAN_SCREEN;
}
inline void Analog_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CO_OD_ROM.hardWare.gas_koef_A*1000;
		message->data_buffer[1] = CO_OD_ROM.hardWare.gas_koef_B*1000;
		message->data_buffer[2] = (int16_t)(CO_OD_ROM.hardWare.gas_k1*2048);
		message->data_buffer[3] = (int16_t)(CO_OD_ROM.hardWare.gas_k2*2048);
		message->data_buffer[4] = (int16_t)(CO_OD_ROM.hardWare.gas_k3*2048);
		/*тормоза*/
		message->data_buffer[5] = CO_OD_ROM.hardWare.brake_koef_A*1000;
		message->data_buffer[6] = CO_OD_ROM.hardWare.brake_koef_B*1000;
		message->data_buffer[7] = (int16_t)(CO_OD_ROM.hardWare.brake_k1*2048);
		message->data_buffer[8] = (int16_t)(CO_OD_ROM.hardWare.brake_k2*2048);
		message->data_buffer[9] = (int16_t)(CO_OD_ROM.hardWare.brake_k3*2048);
		message->data_buffer[10]= 0;
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.hardWare.gas_koef_A = message->data_buffer[0]*1e-3;
		CO_OD_ROM.hardWare.gas_koef_B = message->data_buffer[1]*1e-3;
		CO_OD_ROM.hardWare.gas_k1 = (int16_t)message->data_buffer[2]*4.8828125e-4;
		CO_OD_ROM.hardWare.gas_k2 = (int16_t)message->data_buffer[3]*4.8828125e-4;
		CO_OD_ROM.hardWare.gas_k3 = (int16_t)message->data_buffer[4]*4.8828125e-4;
		/*Тормоз*/
		CO_OD_ROM.hardWare.brake_koef_A = (uint16_t)message->data_buffer[5]*1e-3;
		CO_OD_ROM.hardWare.brake_koef_B = (uint16_t)message->data_buffer[6]*1e-3;
		CO_OD_ROM.hardWare.brake_k1 = (int16_t)message->data_buffer[7]*4.8828125e-4;
		CO_OD_ROM.hardWare.brake_k2 = (int16_t)message->data_buffer[8]*4.8828125e-4;
		CO_OD_ROM.hardWare.brake_k3 = (int16_t)message->data_buffer[9]*4.8828125e-4;
		message->cmd = OK;
	}
	message->screen_num = ANALOG_SCREEN; /*Analog screen*/
}

inline void Functions_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CO_OD_ROM.hardWare.IN_functions;
		message->data_buffer[1] = CO_OD_ROM.hardWare.IN_inversion;
		/*ПРофиль 1*/
		message->data_buffer[2] = CO_OD_ROM.driveProfiles.S1_torque_current;
		message->data_buffer[3] = CO_OD_ROM.driveProfiles.S1_brake_current;
		message->data_buffer[4] = CO_OD_ROM.driveProfiles.S1_max_speed;
		message->data_buffer[5] = CO_OD_ROM.driveProfiles.S1_max_discharge;
		message->data_buffer[6] = CO_OD_ROM.driveProfiles.S1_autoBrakeCurrent;
		/*Профиль 2*/
		message->data_buffer[7] = CO_OD_ROM.driveProfiles.S2_torque_current;
		message->data_buffer[8] = CO_OD_ROM.driveProfiles.S2_brake_current;
		message->data_buffer[9] = CO_OD_ROM.driveProfiles.S2_max_speed;
		message->data_buffer[10]= CO_OD_ROM.driveProfiles.S2_max_discharge;
		message->data_buffer[11]= CO_OD_ROM.driveProfiles.S2_autoBrakeCurrent;
		message->data_buffer[12]= IO.IN_CAN;
		message->data_buffer[13]= CO_OD_ROM.hardWare.CAN_IN_emulation;
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.hardWare.IN_functions = message->data_buffer[0];
		CO_OD_ROM.hardWare.IN_inversion = message->data_buffer[1];

		/*Профиль 1*.		 */
		CO_OD_ROM.driveProfiles.S1_torque_current = message->data_buffer[2];
		CO_OD_ROM.driveProfiles.S1_brake_current = message->data_buffer[3];
		CO_OD_ROM.driveProfiles.S1_max_speed = message->data_buffer[4];
		CO_OD_ROM.driveProfiles.S1_max_discharge = message->data_buffer[5];
		CO_OD_ROM.driveProfiles.S1_autoBrakeCurrent = message->data_buffer[6];
		/*Профиль 2*/
		CO_OD_ROM.driveProfiles.S2_torque_current = message->data_buffer[7];
		CO_OD_ROM.driveProfiles.S2_brake_current = message->data_buffer[8];
		CO_OD_ROM.driveProfiles.S2_max_speed = message->data_buffer[9];
		CO_OD_ROM.driveProfiles.S2_max_discharge = message->data_buffer[10];
		CO_OD_ROM.driveProfiles.S2_autoBrakeCurrent = message->data_buffer[11];
		/*ПРофиль по умолчанию*/
		IO.IN_CAN = message->data_buffer[12];
		CO_OD_ROM.hardWare.CAN_IN_emulation = message->data_buffer[13];
		message->cmd = OK;
	}
	message->screen_num = FUNCTIONS_SCREEN;
}
inline void Buttons_screen(Message_st*message){
	if(message->cmd == 0) /*Reading*/{
		message->data_buffer[0] = IO.IN_CAN;
		message->data_buffer[1] = IO.IN_1;
		message->data_buffer[2] = IO.IN_2;
		message->data_buffer[3] = IO.IN_3;
		message->data_buffer[4] = IO.IN_4;
	}
	if(message->cmd == 1) /*Writing */
	{
		IO.IN_CAN = message->data_buffer[0];
		message->cmd = OK;
	}
	message->screen_num = BUTTONS_SCREEN;

}
inline void SensorLess_screen(Message_st*message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] = CO_OD_ROM.sensor_Less_mode.observer_type;
		message->data_buffer[1] = CO_OD_ROM.sensor_Less_mode.observer_freq_rads;
		message->data_buffer[2] = CO_OD_ROM.sensor_Less_mode.align_current_Amp;
		message->data_buffer[3] = CO_OD_ROM.sensor_Less_mode.align_time_ms;
		message->data_buffer[4] = CO_OD_ROM.sensor_Less_mode.derate_speed_start;
		message->data_buffer[5] = CO_OD_ROM.sensor_Less_mode.derate_speed_stop;
		message->data_buffer[6] = CO_OD_ROM.sensor_Less_mode.open_speed_ramp;
		message->data_buffer[7] = CO_OD_ROM.sensor_Less_mode.open_mode_trig_speed;
		message->data_buffer[8] = 0;
		message->data_buffer[9] = 0;
		message->data_buffer[10]= 0;
		message->data_buffer[11]= 0;
		message->data_buffer[12]= 0;
		message->data_buffer[13]= 0;
	}
	if(message->cmd ==1) /*Writing */
	{
		CO_OD_ROM.sensor_Less_mode.observer_type = (uint16_t)message->data_buffer[0];
		CO_OD_ROM.sensor_Less_mode.observer_freq_rads = (uint16_t)message->data_buffer[1];
		CO_OD_ROM.sensor_Less_mode.align_current_Amp = (uint16_t)message->data_buffer[2];
		CO_OD_ROM.sensor_Less_mode.align_time_ms = (uint16_t)message->data_buffer[3];
		CO_OD_ROM.sensor_Less_mode.derate_speed_start = (uint16_t)message->data_buffer[4];
		CO_OD_ROM.sensor_Less_mode.derate_speed_stop = (uint16_t)message->data_buffer[5];
		CO_OD_ROM.sensor_Less_mode.open_speed_ramp = (uint16_t)message->data_buffer[6];
		CO_OD_ROM.sensor_Less_mode.open_mode_trig_speed = (uint16_t)message->data_buffer[7];
		message->cmd = OK;
	}
	message->screen_num = SENSORLESS_SCREEN; /*Sensorless screen*/
}
inline void Log_settings(Message_st *message)
{

	if(message->cmd == 0) /*Reading*/{

		message->data_buffer[0] =(uint16_t) CO_OD_ROM.logSettings.log_reject;
		message->data_buffer[1] =(uint16_t) CO_OD_ROM.logSettings.log_postavar;
		message->data_buffer[2] =(uint16_t) CO_OD_ROM.log_Vars.var2_Index*256|CO_OD_ROM.log_Vars.var1_Index;
		message->data_buffer[3] =(uint16_t) CO_OD_ROM.log_Vars.var4_Index*256|CO_OD_ROM.log_Vars.var3_Index;
		message->data_buffer[4] =(uint16_t) CO_OD_ROM.log_Vars.var6_Index*256|CO_OD_ROM.log_Vars.var5_Index;
		message->data_buffer[5] =(uint16_t) CO_OD_ROM.log_Vars.var8_Index*256|CO_OD_ROM.log_Vars.var7_Index;
		message->data_buffer[6] =(uint16_t) CO_OD_ROM.log_Vars.var10_Index*256|CO_OD_ROM.log_Vars.var9_Index;
		message->data_buffer[7] =(uint16_t) CO_OD_ROM.log_Vars.var12_Index*256|CO_OD_ROM.log_Vars.var11_Index;
		message->data_buffer[8] =(uint16_t) CO_OD_ROM.log_Vars.var14_Index*256|CO_OD_ROM.log_Vars.var13_Index;
		message->data_buffer[9] =(uint16_t) CO_OD_ROM.log_Vars.var16_Index*256|CO_OD_ROM.log_Vars.var15_Index;


	}
	if(message->cmd ==1) /*Writing */
	{
	    CO_OD_ROM.logSettings.log_reject = message->data_buffer[0]&0xFF;
	    CO_OD_ROM.logSettings.log_postavar = message->data_buffer[1]&0xFF;
	    CO_OD_ROM.log_Vars.var1_Index = message->data_buffer[2] & 0xFF;
	    CO_OD_ROM.log_Vars.var2_Index = (message->data_buffer[2]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var3_Index = message->data_buffer[3] & 0xFF;
	    CO_OD_ROM.log_Vars.var4_Index = (message->data_buffer[3]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var5_Index = message->data_buffer[4] & 0xFF;
	    CO_OD_ROM.log_Vars.var6_Index = (message->data_buffer[4]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var7_Index = message->data_buffer[5] & 0xFF;
	    CO_OD_ROM.log_Vars.var8_Index = (message->data_buffer[5]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var9_Index = message->data_buffer[6] & 0xFF;
	    CO_OD_ROM.log_Vars.var10_Index = (message->data_buffer[6]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var11_Index = message->data_buffer[7] & 0xFF;
	    CO_OD_ROM.log_Vars.var12_Index = (message->data_buffer[7]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var13_Index = message->data_buffer[8] & 0xFF;
	    CO_OD_ROM.log_Vars.var14_Index = (message->data_buffer[8]>>8) & 0xFF;
	    CO_OD_ROM.log_Vars.var15_Index = message->data_buffer[9] & 0xFF;
	    CO_OD_ROM.log_Vars.var16_Index = (message->data_buffer[9]>>8) & 0xFF;
	    message->cmd = OK;
	}
	message->screen_num = LOGSETINNGS_SCREEN; /*Log screen*/
}
inline void Log_screen(Message_st *message)
{
	unsigned int data = 0; //Номер записи в логе
	if(message->cmd == 0) /*Reading*/{

		data = message->data_buffer[0]; /*установим номер Inta  в логе */
		message->data_buffer[1] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+0));
		message->data_buffer[2] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+1));
		message->data_buffer[3] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+2));
		message->data_buffer[4] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+3));
		message->data_buffer[5] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+4));
		message->data_buffer[6] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+5));
		message->data_buffer[7] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+6));
		message->data_buffer[8] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+7));
		message->data_buffer[9] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+8));
		message->data_buffer[10] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+9));
		message->data_buffer[11] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+10));
		message->data_buffer[12] =(uint16_t) *((uint16_t*)logHeader.flash_adrr+(unsigned int)(data*12+11));
		/*Передадим количество Int в логе Int=2 Byte*/
		message->data_buffer[13] = sizeof(log_struct);
	}
	message->screen_num = LOGREAD_SCREEN; /*Log screen*/
}
inline void Save_screen(Message_st *message)
{	unsigned int rom_saved_flag =0 ;///< flag indicate that data were saved
	if(message->cmd == 1)
	{
		/*Сохранение параметров*/
		if((message->data_buffer[0] =='S') && (message->data_buffer[1] == 'A')
				&& (message->data_buffer[2] == 'V') && (message->data_buffer[3] =='E'))
		{
			/*recived SAVE rom command*/
			uint8_t code[4] = {0x73,0x61,0x76,0x65};
			/*Clear flag*/
			rom_saved_flag = 0;
			if(!BLDC_drive.stat.bit_ENABLED)
			{
				storeRomExtern(code);
				/*Set flag*/
				rom_saved_flag = 1;
			}
			message->data_buffer[0] = rom_saved_flag;
			message->data_buffer[1] = 0;
			message->data_buffer[2] = 0;
			message->data_buffer[3] = 0;
			message->data_buffer[4] = 0;
			message->data_buffer[5] = 0;
			message->data_buffer[6]	= 0;
			message->data_buffer[7]	= 0;
			message->data_buffer[8]	= 0;
			message->data_buffer[9]	= 0;
			message->data_buffer[10] = 0;
			message->data_buffer[11] = 0;
			message->data_buffer[12] = 0;
			message->data_buffer[13] = 0;
			message->cmd  = OK;

		}
		/*Сброс к начальным условиям*/
		if((message->data_buffer[0] =='L') && (message->data_buffer[1] == 'O')
				&& (message->data_buffer[2] == 'A') && (message->data_buffer[3] =='D'))
		{
			/*recived SAVE rom command*/
			uint8_t code[4] = {0x6c,0x6f,0x61,0x64};
			/*Clear flag*/
			rom_saved_flag = 0;
			if(!BLDC_drive.stat.bit_ENABLED)
			{
				clearRomExtern(code);
				/*Set flag*/
				rom_saved_flag = 2;
			}
			message->data_buffer[0] = rom_saved_flag;
			message->data_buffer[1] = 0;
			message->data_buffer[2] = 0;
			message->data_buffer[3] = 0;
			message->data_buffer[4] = 0;
			message->data_buffer[5] = 0;
			message->data_buffer[6]	= 0;
			message->data_buffer[7]	= 0;
			message->data_buffer[8]	= 0;
			message->data_buffer[9]	= 0;
			message->data_buffer[10] = 0;
			message->data_buffer[11] = 0;
			message->data_buffer[12] = 0;
			message->data_buffer[13] = 0;
			message->cmd  = OK;

		}
		if(		 (message->data_buffer[0]=='R')
		      && (message->data_buffer[1]=='E')
			  && (message->data_buffer[2]=='S')
			  && (message->data_buffer[3]=='E')
			  && (message->data_buffer[4]=='T'))
		  {
			//DINT;
			reset_active  = CO_RESET_COMM;
			//while(1);/*Бесконечный цикл для перезагрузки*/
		  }
		if(				 (message->data_buffer[0]=='R')
				      && (message->data_buffer[1]=='E')
					  && (message->data_buffer[2]=='S')
					  && (message->data_buffer[3]=='E')
					  && (message->data_buffer[4]=='T')
					  && (message->data_buffer[5]=='C')
					  && (message->data_buffer[6]=='O')
					  && (message->data_buffer[7]=='N'))
				  {
					//DINT;
					reset_active  = CO_RESET_APP;
					//while(1);/*Бесконечный цикл для перезагрузки*/
				  }
	}
	if(message->cmd==0)
	{
		message->data_buffer[0] = rom_saved_flag;
		message->data_buffer[1] = 0;
		message->data_buffer[2] = 0;
		message->data_buffer[3] = 0;
		message->data_buffer[4] = 0;
		message->data_buffer[5] = 0;
		message->data_buffer[6]	= 0;
		message->data_buffer[7]	= 0;
		message->data_buffer[8]	= 0;
		message->data_buffer[9]	= 0;
		message->data_buffer[10] = 0;
		message->data_buffer[11] = 0;
		message->data_buffer[12] = 0;
		message->data_buffer[13] = 0;
		message->cmd  = 0;
	}
	message->screen_num = SAVE_SCREEN;
}
void SaveByHand(uint16_t ok)
{
	if(ok ==1)
	{
		uint8_t code[4] = {0x73,0x61,0x76,0x65};
		storeRomExtern(code);
	}
}
/*!
 * @brief function return reset command
 * @return reset active state
 */
uint8_t isReset(void)
{
	return reset_active;
}

void setResetCmd(uint8_t cmd )
{
   reset_active = cmd;
}

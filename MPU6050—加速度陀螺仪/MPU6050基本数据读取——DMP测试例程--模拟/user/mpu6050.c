#include "mpu6050.h"
#include "./delay/core_delay.h"  
#include "fsl_debug_console.h"
#include "./i2c/bsp_i2c.h"
/**
  * @brief   初始化MPU芯片
  * @param   
  * @retval  
  */
uint8_t MPU_Init(void)
{ 
	uint8_t res;
	I2C_WriteBytes(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  CPU_TS_Tmr_Delay_MS(100);
	I2C_WriteBytes(MPU_PWR_MGMT1_REG,0X00);	
	MPU_Set_Gyro_Fsr(3);				
	MPU_Set_Accel_Fsr(0);				
	MPU_Set_Rate(50);						
	I2C_WriteBytes(MPU_INT_EN_REG,0X00);	
	I2C_WriteBytes(MPU_USER_CTRL_REG,0X00);	
	I2C_WriteBytes(MPU_FIFO_EN_REG,0X00);	
	I2C_WriteBytes(MPU_INTBP_CFG_REG,0X80);	
	res=I2C_ReadBytes(MPU_DEVICE_ID_REG);
	/*器件地址*/
	if(res==MPU_ADDR)
	{
		I2C_WriteBytes(MPU_PWR_MGMT1_REG,0X01);
		I2C_WriteBytes(MPU_PWR_MGMT2_REG,0X00);	
		MPU_Set_Rate(50);					
 	}else return 1;
	return 0;
}


/**
* @brief  设置陀螺仪的量程范围
* @param  无
* @retval 无
*/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return I2C_WriteBytes(MPU_GYRO_CFG_REG,fsr<<3);
}
/**
* @brief  设置加速度的量程范围
* @param  无
* @retval 无
*/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return I2C_WriteBytes(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
* @brief  设置低通滤波器
* @param  无
* @retval 无
*/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return I2C_WriteBytes(MPU_CFG_REG,data);
}
/**
* @brief  设置采样频率
* @param  无
* @retval 无
*/
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=I2C_WriteBytes(MPU_SAMPLE_RATE_REG,data);	
 	return MPU_Set_LPF(rate/2);
}

/**
* @brief  获取温度
* @param  无
* @retval 无
*/
short MPU_Get_Temperature(void)
{
		uint8_t buf[2]; 
		short raw;
		float temp;
		Sensor_Read_DMP(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
		raw=((uint16_t)buf[0]<<8)|buf[1];  
		temp=36.53+((double)raw)/340;  
		return temp*100;
}
/**
* @brief  获取加速度数据的原始数值
* @param  无
* @retval 无
*/
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
	uint8_t buf[6],res;  
	res=Sensor_Read_DMP(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
/**
* @brief  获取陀螺仪数据的原始数值
* @param  无
* @retval 无
*/
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
	uint8_t buf[6],res;  
	res=Sensor_Read_DMP(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

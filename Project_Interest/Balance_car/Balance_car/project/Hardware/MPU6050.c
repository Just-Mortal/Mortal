#include "MPU6050.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"

u8 mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf)//返回值 0：读成功  -1：读失败
{ 
	unsigned char i;
	addr=addr<<1;                     //注意dma库地址不包含最后一位，需要移位
	MPU6050_IIC_Start();              //启动总线
	MPU6050_IIC_Send_Byte(addr);      //发送器件地址           
	MPU6050_IIC_Send_Byte(reg);       //发送器件子地址
	
	for(i=0;i<len;i++)
		MPU6050_IIC_Send_Byte(*buf++);  //发送数据
	
	MPU6050_IIC_Stop();               //结束总线

	return 0;
}

u8 mpu6050_read(u8 addr, u8 reg, u8 len, u8 *buf)//返回值 0：读成功  -1：读失败
{
	unsigned char i;
    addr=addr<<1;                     //注意dma库地址不包含最后一位，需要移位
	MPU6050_IIC_Start();              //启动总线           
	MPU6050_IIC_Send_Byte(addr);      //发送器件地址            
	MPU6050_IIC_Send_Byte(reg);       //发送器件子地址

	MPU6050_IIC_Start();              //重新启动总线
	MPU6050_IIC_Send_Byte(addr+1);
	for(i=0;i<len-1;i++)  
		*buf++=MPU6050_IIC_Read_Byte(0);//发送数据
		 
	*buf=MPU6050_IIC_Read_Byte(1);
	MPU6050_IIC_Stop();               //结束总线
	
	return 0;
}

void mpu6050_write_reg(u8 reg, u8 dat)
{
	mpu6050_write(SlaveAddress,reg,1,&dat);
}

u8   mpu6050_read_reg (u8 reg)
{
	u8 dat;
	mpu6050_read(SlaveAddress,reg,1,&dat);
	return dat;
}

void MPU6050_Init(void)
{
    MPU6050_IIC_IO_Init(); 
 	
	mpu6050_write_reg(PWR_MGMT_1,	 0X00);	//唤醒MPU6050 
    mpu6050_write_reg(SMPLRT_DIV,  0x07); //陀螺仪采样率125Hz
    mpu6050_write_reg(CONFIG,      0x06); //低通滤波频率5Hz
	mpu6050_write_reg(GYRO_CONFIG, 0x18); //陀螺仪自检及测量范围:不自检，2000deg/s
	mpu6050_write_reg(ACCEL_CONFIG,0x01); //加速计自检、测量范围及高通滤波频率:不自检，2G，5Hz
}

void MPU6050_GetDate(int16_t *ACCX ,int16_t *ACCY ,int16_t *ACCZ,
					 int16_t *GYROX,int16_t *GYROY,int16_t *GYROZ)
{
	uint16_t DateH,DateL;
	
	DateH = mpu6050_read_reg(MPU6050_ACCEL_XOUT_H );
	DateL = mpu6050_read_reg(MPU6050_ACCEL_XOUT_L );
	*ACCX = (DateH << 8) | DateL;
	
	DateH = mpu6050_read_reg(MPU6050_ACCEL_YOUT_H );
	DateL = mpu6050_read_reg(MPU6050_ACCEL_YOUT_L );
	*ACCY = (DateH << 8) | DateL;
	
	DateH = mpu6050_read_reg(MPU6050_ACCEL_ZOUT_H );
	DateL = mpu6050_read_reg(MPU6050_ACCEL_ZOUT_L );
	*ACCZ = (DateH << 8) | DateL;
	
	DateH = mpu6050_read_reg(MPU6050_GYRO_XOUT_H );
	DateL = mpu6050_read_reg(MPU6050_GYRO_XOUT_L );
	*GYROX = (DateH << 8) | DateL;
	
	DateH = mpu6050_read_reg(MPU6050_GYRO_YOUT_H );
	DateL = mpu6050_read_reg(MPU6050_GYRO_YOUT_L );
	*GYROY = (DateH << 8) | DateL;
	
	DateH = mpu6050_read_reg(MPU6050_GYRO_ZOUT_H );
	DateL = mpu6050_read_reg(MPU6050_GYRO_ZOUT_L );
	*GYROZ = (DateH << 8) | DateL;
}

//得到陀螺仪角速度值
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=mpu6050_read(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//得到角加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=mpu6050_read(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}

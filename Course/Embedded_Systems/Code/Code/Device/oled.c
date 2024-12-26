#include "oled.h"

void OLED_WriteCommand(uint8 Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

void OLED_WriteData(uint8 Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

void OLED_SetCursor(uint8 Y, uint8 X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

void OLED_Clear(void)
{  
	uint8 i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

void OLED_Init(void)
{
	uint32 i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
//	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	
	OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//���öԱȶȿ���
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//������ʾ
		
	OLED_Clear();				//OLED����
}

void OLED_DrawDot(uint8 x, uint8 y)
{
	uint8 pos, bx, temp=0, i;
		
	if(x > 127 || y > 63) 
		return;

	y = 63 - y;
	
	pos 	= y / 8;
	bx		=	y % 8;
	temp	=	1<<(bx);
	
	OLED_SetCursor(pos, x);
	OLED_WriteData(temp);
	
	for(i = 0; i < 8; i++ )
	{
		if(i != pos)
		{
			OLED_SetCursor(i, x);
			OLED_WriteData(0x00);
		}
	}
}

void OLED_DrawLine(uint8 x1, uint8 y1, uint8 x2,uint8 y2)
{
	uint8 t; 
	int16 offset_x,offset_y; 
	int16 incx,incy,uRow,uCol;
	float K = 0.0f;
	
	offset_x = x2 - x1;
	offset_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(offset_x>0)
		incx=1;
	else if(offset_x==0)
		incx=0;
	else
	{
		incx			=-1;
		offset_x	=-offset_x;
	}

	if(offset_y>0)
		incy=1;
	else if(offset_y==0)
		incy=0;    //ˮƽ��
	else
	{
		incy			=-1;
		offset_y	=-offset_y;
	}

	if(incx==0)
	{
		for(t=0;t<=offset_y+1;t++ )
		{
			OLED_DrawDot(uRow,	uCol+t*incy);
		}
	}
	else if(incy==0)
	{
		for(t=0;t<=offset_x+1;t++ )
			OLED_DrawDot(uRow+t*incx,uCol);
	}
	else
	{
		K = (float)(((float)y2-(float)y1)*1.000/((float)x2-(float)x1));
		
		for(t=0;t<=offset_x+1;t++ )
			OLED_DrawDot(uRow+t,(u8)(uCol+t*K));
	}
}

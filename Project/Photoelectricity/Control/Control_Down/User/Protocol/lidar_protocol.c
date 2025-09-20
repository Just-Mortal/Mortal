#include "lidar_protocol.h"

volatile uint8_t data_process_flag = 0;
N10P_Packet_t dma_pack_data;
Lidar_t lidar_data = {0};

// �״����ݸ���
static void updateLidarData(Lidar_t *lidar_data, const N10P_Packet_t *packet)
{
    // �����״�ת�� Hz  24��
     lidar_data->speed = 1000000.0f / (((packet->speed_h << 8) | packet->speed_l) * 24.0f);

    // ������ʼ�ǶȺͽ����Ƕ�
    lidar_data->start_angle = ((packet->start_angle_h << 8) | packet->start_angle_l) / 100.0f;
    lidar_data->end_angle = ((packet->end_angle_h << 8) | packet->end_angle_l) / 100.0f;
    if(lidar_data->end_angle < lidar_data->start_angle)
        lidar_data->end_angle += 360.0f;
    
    // ���µ�������
    for (int i = 0; i < 32; i++)
    {
        lidar_data->points[i].angle = lidar_data->start_angle + i * ((lidar_data->end_angle - lidar_data->start_angle) / 32.0f);
        while(lidar_data->points[i].angle >= 360.0f)  lidar_data->points[i].angle -= 360.0f;
        while(lidar_data->points[i].angle < 0.0f)     lidar_data->points[i].angle += 360.0f;
        // ���뵥λת��Ϊ��        
        lidar_data->points[i].distances = ((packet->point[i].dist_h << 8) | packet->point[i].dist_l) / 1000.0f;
        lidar_data->points[i].strong = packet->point[i].strong;
    }
}

void USART2_rxDataHandler(uint8_t *rxBuf)
{
    // 1. У��֡ͷ�ͳ���
    if (rxBuf[0] != 0xA5 || rxBuf[1] != 0x5A || rxBuf[2] != 0x6C)	return; // ֡ͷ�򳤶ȴ���

    // 2. ����У��ͣ�ǰ107�ֽ���ͣ�
    uint8_t crc_calc = 0;
    for (int i = 0; i < 107; i++)
        crc_calc += rxBuf[i];

    // 3. У��ͨ����������
    if (crc_calc == rxBuf[107])
		{
        memcpy(&dma_pack_data, rxBuf, sizeof(N10P_Packet_t));
        updateLidarData(&lidar_data, &dma_pack_data);
        data_process_flag = 1; // �������ݴ����־
    }
}



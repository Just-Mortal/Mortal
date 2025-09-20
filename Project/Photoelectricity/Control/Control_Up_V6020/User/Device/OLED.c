#include "OLED.h"
#include "spi.h"

#include "OLED_font.h"

// �Դ� (128��x8ҳ)
uint8_t OLED_GRAM[128][8] = {0};

// ��OLEDд�뵥�ֽ�
void OLED_WR_Byte(uint8_t dat, uint8_t cmd) {
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, cmd ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &dat, 1, 10);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

// OLED��ʼ�����У��������ؼ����ã�
void OLED_Init(void) {
    // Ӳ����λ
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    
    // ��ʼ����������
    OLED_WR_Byte(0xAE, OLED_CMD); // �ر���ʾ
    OLED_WR_Byte(0xD5, OLED_CMD); // ����ʱ�ӷ�Ƶ
    OLED_WR_Byte(0x80, OLED_CMD); 
    OLED_WR_Byte(0xA8, OLED_CMD); // ���ö�·������
    OLED_WR_Byte(0x3F, OLED_CMD); // 1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); // ������ʾƫ��
    OLED_WR_Byte(0x00, OLED_CMD); // ��ƫ��
    OLED_WR_Byte(0x40, OLED_CMD); // ������ʼ��
    OLED_WR_Byte(0x8D, OLED_CMD); // ��ɱ�����
    OLED_WR_Byte(0x14, OLED_CMD); // ���õ�ɱ�
    OLED_WR_Byte(0x20, OLED_CMD); // �ڴ��ַģʽ
    OLED_WR_Byte(0x00, OLED_CMD); // ˮƽ��ַģʽ���ؼ��޸����룩
    OLED_WR_Byte(0xA1, OLED_CMD); // ����ӳ������
    OLED_WR_Byte(0xC8, OLED_CMD); // COMɨ�跽���������ؼ��޸����µߵ���
    OLED_WR_Byte(0xDA, OLED_CMD); // COMӲ������
    OLED_WR_Byte(0x12, OLED_CMD); 
    OLED_WR_Byte(0x81, OLED_CMD); // �Աȶ�����
    OLED_WR_Byte(0xCF, OLED_CMD); 
    OLED_WR_Byte(0xD9, OLED_CMD); // Ԥ�������
    OLED_WR_Byte(0xF1, OLED_CMD); 
    OLED_WR_Byte(0xDB, OLED_CMD); // VCOMH����
    OLED_WR_Byte(0x40, OLED_CMD); 
    OLED_WR_Byte(0xA4, OLED_CMD); // ��ʾ�ָ�
    OLED_WR_Byte(0xA6, OLED_CMD); // ������ʾ
    OLED_WR_Byte(0xAF, OLED_CMD); // ������ʾ
    
    OLED_Clear();
}

// ����
void OLED_Clear(void) {
    for(uint8_t i = 0; i < 8; i++) {
        for(uint8_t n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0x00;
        }
    }
    OLED_Refresh_Gram();
}

// ˢ���Դ浽OLED
void OLED_Refresh_Gram(void) {
    for(uint8_t i = 0; i < 8; i++) {
        OLED_WR_Byte(0xB0 + i, OLED_CMD);   // ����ҳ��ַ
        OLED_WR_Byte(0x00, OLED_CMD);       // �е͵�ַ
        OLED_WR_Byte(0x10, OLED_CMD);       // �иߵ�ַ
        
        for(uint8_t n = 0; n < 128; n++) {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

// ���㺯�������ģ�
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color) {
    if(x > 127 || y > 63) return; // �߽���
    
    uint8_t page = y / 8;        // ����ҳ��ַ (0-7)
    uint8_t bit_pos = y % 8;     // ҳ��λλ�� (0-7)
    uint8_t mask = 1 << bit_pos;  // ����λ����
    
    if(color)
        OLED_GRAM[x][page] |= mask;  // ��������
    else
        OLED_GRAM[x][page] &= ~mask;  // Ϩ������
}

// ��ʾ�ַ� (ʹ��16x8����)
// x: �ַ���λ�� (0-15)
// y: �ַ���λ�� (0-3)
// chr: Ҫ��ʾ���ַ�
// mode: 0=������ʾ, 1=������ʾ
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t mode) {
    // ֻ֧��16x8����
    const uint8_t char_width = 8;   // �ַ����(����)
    const uint8_t char_height = 16; // �ַ��߶�(����)
    
    // ת��Ϊ��������
    uint8_t pixel_x = x * char_width;
    uint8_t pixel_y = y * char_height;
    
    // �����ַ����ֿ��е�����
    if(chr < ' ' || chr > '~') return; // �Ƿ��ַ�����
    uint8_t c = chr - ' ';  
    
    // ��ȡ��ģ����
    const uint8_t *font_data = oled_asc2_1608[c];  
    
    // ����ÿһ��
    for(uint8_t col = 0; col < char_width; col++) {
        // ��ȡ��ǰ�е���������������
        uint8_t upper_data = font_data[col * 2];
        uint8_t lower_data = font_data[col * 2 + 1];
        
        // �����ϰ벿��(0-7��)
        for(uint8_t bit = 0; bit < 8; bit++) {
            uint8_t pixel = (upper_data >> (7 - bit)) & 0x01;
            uint8_t render_y = pixel_y + bit;
            if(render_y < 64) { // �߽���
                OLED_DrawPoint(pixel_x + col, render_y, pixel ^ !mode);
            }
        }
        
        // �����°벿��(8-15��)
        for(uint8_t bit = 0; bit < 8; bit++) {
            uint8_t pixel = (lower_data >> (7 - bit)) & 0x01;
            uint8_t render_y = pixel_y + 8 + bit;
            if(render_y < 64) { // �߽���
                OLED_DrawPoint(pixel_x + col, render_y, pixel ^ !mode);
            }
        }
    }
}

// ��ʾ�ַ��� (ʹ��16x8����)
// x: ��ʼ�ַ���λ�� (0-15)
// y: ��ʼ�ַ���λ�� (0-3)
// str: Ҫ��ʾ���ַ���
// mode: 0=������ʾ, 1=������ʾ
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t mode) {
    uint8_t start_x = x; // ������ʼ��λ��
    
    while(*str) {
        // ���д����س����򵽴���β
        if(*str == '\n' || x > 15) {
            y++;       // �ƶ�����һ��
            x = start_x; // �ص���ʼ��
            str++;      // �������з�
            if(y > 3) break; // ������Ļ�ײ�
            continue;
        }
        
        OLED_ShowChar(x, y, *str++, mode);
        x++; // �ƶ�����һ��
        
        // �Զ�����
        if(x > 15) {
            y++;
            x = 0;
            if(y > 3) break; // ������Ļ�ײ�
        }
    }
}

// ��ʾ���֣�������
// x: ��ʼ�ַ���λ�� (0-15)
// y: ��ʼ�ַ���λ�� (0-3)
// num: Ҫ��ʾ�����֣�֧�ָ�����
// digits: ��ʾ����λ��������ʱǰ�油0��0��ʾ��ʵ��λ����ʾ��
// mode: 0=������ʾ, 1=������ʾ
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t digits, uint8_t mode) {
    char str[12]; // �㹻�洢32λ�������������ţ�
    char fmt[6];  // ��ʽ�ַ���������
    uint8_t len = 0;
    
    // �������ͼ���ʵ��λ��
    int32_t temp = (num < 0) ? -num : num;
    uint8_t num_digits = 0;
    do {
        num_digits++;
        temp /= 10;
    } while (temp > 0);
    
    if (num < 0) num_digits++; // Ϊ��������λ��
    
    // �Զ�ȷ����ʾλ�������δָ�����㣩
    if (digits == 0 || digits < num_digits) {
        digits = num_digits;
    }
    
    // ������ʽ�ַ��������Ǹ��Ŵ���
    if (num < 0) {
        snprintf(fmt, sizeof(fmt), "%%0%dd", digits);
        // �ֶ��������Ĳ���
        len = snprintf(str, sizeof(str), fmt, -num);
        // �ƶ����ݲ���Ӹ���
        memmove(str + 1, str, len + 1); // +1����������
        str[0] = '-';
    } else {
        snprintf(fmt, sizeof(fmt), "%%0%dd", digits);
        snprintf(str, sizeof(str), fmt, num);
    }
    
    // ��ʾ�ַ���
    OLED_ShowString(x, y, str, mode);
}

// ��ʾ������
// x: ��ʼ�ַ���λ�� (0-15)
// y: ��ʼ�ַ���λ�� (0-3)
// fnum: Ҫ��ʾ�ĸ�����
// int_digits: ����������С��ʾλ��������ʱǰ�油0��0��ʾ��ʵ��λ����ʾ��
// frac_digits: С��������ʾλ����0-5��
// mode: 0=������ʾ, 1=������ʾ
void OLED_ShowFloat(uint8_t x, uint8_t y, float fnum, uint8_t int_digits, uint8_t frac_digits, uint8_t mode) {
    // �߽籣��
    if(frac_digits > 5) frac_digits = 5; // ����С��λ�����5λ
    char buffer[17] = {0}; // ��ʾ�����������16�ַ�+��������
    
    // �������������NaN�������
//    if(__ARM_isnanf(fnum)) {
//        strcpy(buffer, "NaN");
//        OLED_ShowString(x, y, buffer, mode);
//        return;
//    }
//    if(__ARM_isinff(fnum)) {
//        strcpy(buffer, fnum < 0 ? "-INF" : "INF");
//        OLED_ShowString(x, y, buffer, mode);
//        return;
//    }
    
    // �������
    int sign = (fnum < 0) ? -1 : 1;
    if(fnum < 0) fnum = -fnum;
    
    // �������뵽ָ��С��λ
    float factor = 1.0f;
    for(int i = 0; i < frac_digits; i++) factor *= 10.0f;
    uint32_t frac_part = (uint32_t)(fnum * factor + 0.5f) % (uint32_t)factor;
    uint32_t int_part = (uint32_t)(fnum * factor + 0.5f) / (uint32_t)factor;
    
    // �����λ����9.999����������������ֽ�λ��
    if(frac_part >= (uint32_t)factor) {
        int_part++;
        frac_part = 0;
    }
    
    // ��ʽ����������
    char int_buf[12] = {0};
    int32_t signed_int = sign * (int32_t)int_part;
    
    // ������������ʵ��λ��
    uint32_t temp = int_part;
    uint8_t num_digits = 0;
    do {
        num_digits++;
        temp /= 10;
    } while(temp > 0);
    
    // �Զ�ȷ��������ʾλ��
    if(int_digits == 0 || int_digits < num_digits) {
        int_digits = num_digits;
    }
    
    // �������Ĳ���
    if(signed_int < 0) {
        snprintf(int_buf, sizeof(int_buf), "%%0%dd", int_digits);
        int len = snprintf(buffer, sizeof(buffer), int_buf, -signed_int);
        memmove(buffer + 1, buffer, len + 1);
        buffer[0] = '-';
    } else {
        snprintf(int_buf, sizeof(int_buf), "%%0%dd", int_digits);
        snprintf(buffer, sizeof(buffer), int_buf, signed_int);
    }
    
    // ���С������
    if(frac_digits > 0) {
        // ���С����
        strcat(buffer, ".");
        
        // ���С������
        char frac_buf[6] = {0}; // ���5λС��
        snprintf(frac_buf, sizeof(frac_buf), "%%0%du", frac_digits);
        
        char frac_str[6] = {0};
        snprintf(frac_str, sizeof(frac_str), frac_buf, frac_part);
        
        // ȷ��С����������ȷλ�����������λ��̣�
        size_t current_frac_len = strlen(frac_str);
        if(current_frac_len < frac_digits) {
            size_t pad_len = frac_digits - current_frac_len;
            char pad_buf[6] = {0};
            memset(pad_buf, '0', pad_len);
            strcat(pad_buf, frac_str);
            strcat(buffer, pad_buf);
        } else {
            strcat(buffer, frac_str);
        }
    }
    
    // ��ʾ�ַ������Զ������кͱ߽磩
    OLED_ShowString(x, y, buffer, mode);
}

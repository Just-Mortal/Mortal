#include "OLED.h"
#include "spi.h"

#include "OLED_font.h"

// 显存 (128列x8页)
uint8_t OLED_GRAM[128][8] = {0};

// 向OLED写入单字节
void OLED_WR_Byte(uint8_t dat, uint8_t cmd) {
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, cmd ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &dat, 1, 10);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

// OLED初始化序列（解决乱码关键配置）
void OLED_Init(void) {
    // 硬件复位
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    
    // 初始化命令序列
    OLED_WR_Byte(0xAE, OLED_CMD); // 关闭显示
    OLED_WR_Byte(0xD5, OLED_CMD); // 设置时钟分频
    OLED_WR_Byte(0x80, OLED_CMD); 
    OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用率
    OLED_WR_Byte(0x3F, OLED_CMD); // 1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移
    OLED_WR_Byte(0x00, OLED_CMD); // 无偏移
    OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行
    OLED_WR_Byte(0x8D, OLED_CMD); // 电荷泵设置
    OLED_WR_Byte(0x14, OLED_CMD); // 启用电荷泵
    OLED_WR_Byte(0x20, OLED_CMD); // 内存地址模式
    OLED_WR_Byte(0x00, OLED_CMD); // 水平地址模式（关键修复乱码）
    OLED_WR_Byte(0xA1, OLED_CMD); // 段重映射正常
    OLED_WR_Byte(0xC8, OLED_CMD); // COM扫描方向正常（关键修复上下颠倒）
    OLED_WR_Byte(0xDA, OLED_CMD); // COM硬件配置
    OLED_WR_Byte(0x12, OLED_CMD); 
    OLED_WR_Byte(0x81, OLED_CMD); // 对比度设置
    OLED_WR_Byte(0xCF, OLED_CMD); 
    OLED_WR_Byte(0xD9, OLED_CMD); // 预充电周期
    OLED_WR_Byte(0xF1, OLED_CMD); 
    OLED_WR_Byte(0xDB, OLED_CMD); // VCOMH设置
    OLED_WR_Byte(0x40, OLED_CMD); 
    OLED_WR_Byte(0xA4, OLED_CMD); // 显示恢复
    OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
    OLED_WR_Byte(0xAF, OLED_CMD); // 开启显示
    
    OLED_Clear();
}

// 清屏
void OLED_Clear(void) {
    for(uint8_t i = 0; i < 8; i++) {
        for(uint8_t n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0x00;
        }
    }
    OLED_Refresh_Gram();
}

// 刷新显存到OLED
void OLED_Refresh_Gram(void) {
    for(uint8_t i = 0; i < 8; i++) {
        OLED_WR_Byte(0xB0 + i, OLED_CMD);   // 设置页地址
        OLED_WR_Byte(0x00, OLED_CMD);       // 列低地址
        OLED_WR_Byte(0x10, OLED_CMD);       // 列高地址
        
        for(uint8_t n = 0; n < 128; n++) {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

// 画点函数（核心）
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color) {
    if(x > 127 || y > 63) return; // 边界检查
    
    uint8_t page = y / 8;        // 计算页地址 (0-7)
    uint8_t bit_pos = y % 8;     // 页内位位置 (0-7)
    uint8_t mask = 1 << bit_pos;  // 生成位掩码
    
    if(color)
        OLED_GRAM[x][page] |= mask;  // 点亮像素
    else
        OLED_GRAM[x][page] &= ~mask;  // 熄灭像素
}

// 显示字符 (使用16x8字体)
// x: 字符列位置 (0-15)
// y: 字符行位置 (0-3)
// chr: 要显示的字符
// mode: 0=反白显示, 1=正常显示
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t mode) {
    // 只支持16x8字体
    const uint8_t char_width = 8;   // 字符宽度(像素)
    const uint8_t char_height = 16; // 字符高度(像素)
    
    // 转换为像素坐标
    uint8_t pixel_x = x * char_width;
    uint8_t pixel_y = y * char_height;
    
    // 计算字符在字库中的索引
    if(chr < ' ' || chr > '~') return; // 非法字符处理
    uint8_t c = chr - ' ';  
    
    // 获取字模数据
    const uint8_t *font_data = oled_asc2_1608[c];  
    
    // 遍历每一列
    for(uint8_t col = 0; col < char_width; col++) {
        // 获取当前列的上下两部分数据
        uint8_t upper_data = font_data[col * 2];
        uint8_t lower_data = font_data[col * 2 + 1];
        
        // 绘制上半部分(0-7行)
        for(uint8_t bit = 0; bit < 8; bit++) {
            uint8_t pixel = (upper_data >> (7 - bit)) & 0x01;
            uint8_t render_y = pixel_y + bit;
            if(render_y < 64) { // 边界检查
                OLED_DrawPoint(pixel_x + col, render_y, pixel ^ !mode);
            }
        }
        
        // 绘制下半部分(8-15行)
        for(uint8_t bit = 0; bit < 8; bit++) {
            uint8_t pixel = (lower_data >> (7 - bit)) & 0x01;
            uint8_t render_y = pixel_y + 8 + bit;
            if(render_y < 64) { // 边界检查
                OLED_DrawPoint(pixel_x + col, render_y, pixel ^ !mode);
            }
        }
    }
}

// 显示字符串 (使用16x8字体)
// x: 起始字符列位置 (0-15)
// y: 起始字符行位置 (0-3)
// str: 要显示的字符串
// mode: 0=反白显示, 1=正常显示
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t mode) {
    uint8_t start_x = x; // 保存起始列位置
    
    while(*str) {
        // 换行处理：回车符或到达行尾
        if(*str == '\n' || x > 15) {
            y++;       // 移动到下一行
            x = start_x; // 回到起始列
            str++;      // 跳过换行符
            if(y > 3) break; // 超出屏幕底部
            continue;
        }
        
        OLED_ShowChar(x, y, *str++, mode);
        x++; // 移动到下一列
        
        // 自动换行
        if(x > 15) {
            y++;
            x = 0;
            if(y > 3) break; // 超出屏幕底部
        }
    }
}

// 显示数字（整数）
// x: 起始字符列位置 (0-15)
// y: 起始字符行位置 (0-3)
// num: 要显示的数字（支持负数）
// digits: 显示的总位数（不足时前面补0，0表示按实际位数显示）
// mode: 0=反白显示, 1=正常显示
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t digits, uint8_t mode) {
    char str[12]; // 足够存储32位整数（包括负号）
    char fmt[6];  // 格式字符串缓冲区
    uint8_t len = 0;
    
    // 处理负数和计算实际位数
    int32_t temp = (num < 0) ? -num : num;
    uint8_t num_digits = 0;
    do {
        num_digits++;
        temp /= 10;
    } while (temp > 0);
    
    if (num < 0) num_digits++; // 为负号增加位置
    
    // 自动确定显示位数（如果未指定或不足）
    if (digits == 0 || digits < num_digits) {
        digits = num_digits;
    }
    
    // 创建格式字符串（考虑负号处理）
    if (num < 0) {
        snprintf(fmt, sizeof(fmt), "%%0%dd", digits);
        // 手动处理负数的补零
        len = snprintf(str, sizeof(str), fmt, -num);
        // 移动内容并添加负号
        memmove(str + 1, str, len + 1); // +1包含结束符
        str[0] = '-';
    } else {
        snprintf(fmt, sizeof(fmt), "%%0%dd", digits);
        snprintf(str, sizeof(str), fmt, num);
    }
    
    // 显示字符串
    OLED_ShowString(x, y, str, mode);
}

// 显示浮点数
// x: 起始字符列位置 (0-15)
// y: 起始字符行位置 (0-3)
// fnum: 要显示的浮点数
// int_digits: 整数部分最小显示位数（不足时前面补0，0表示按实际位数显示）
// frac_digits: 小数部分显示位数（0-5）
// mode: 0=反白显示, 1=正常显示
void OLED_ShowFloat(uint8_t x, uint8_t y, float fnum, uint8_t int_digits, uint8_t frac_digits, uint8_t mode) {
    // 边界保护
    if(frac_digits > 5) frac_digits = 5; // 限制小数位数最大5位
    char buffer[17] = {0}; // 显示缓冲区（最大16字符+结束符）
    
    // 处理特殊情况：NaN和无穷大
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
    
    // 处理符号
    int sign = (fnum < 0) ? -1 : 1;
    if(fnum < 0) fnum = -fnum;
    
    // 四舍五入到指定小数位
    float factor = 1.0f;
    for(int i = 0; i < frac_digits; i++) factor *= 10.0f;
    uint32_t frac_part = (uint32_t)(fnum * factor + 0.5f) % (uint32_t)factor;
    uint32_t int_part = (uint32_t)(fnum * factor + 0.5f) / (uint32_t)factor;
    
    // 处理进位（如9.999四舍五入后整数部分进位）
    if(frac_part >= (uint32_t)factor) {
        int_part++;
        frac_part = 0;
    }
    
    // 格式化整数部分
    char int_buf[12] = {0};
    int32_t signed_int = sign * (int32_t)int_part;
    
    // 计算整数部分实际位数
    uint32_t temp = int_part;
    uint8_t num_digits = 0;
    do {
        num_digits++;
        temp /= 10;
    } while(temp > 0);
    
    // 自动确定整数显示位数
    if(int_digits == 0 || int_digits < num_digits) {
        int_digits = num_digits;
    }
    
    // 处理负数的补零
    if(signed_int < 0) {
        snprintf(int_buf, sizeof(int_buf), "%%0%dd", int_digits);
        int len = snprintf(buffer, sizeof(buffer), int_buf, -signed_int);
        memmove(buffer + 1, buffer, len + 1);
        buffer[0] = '-';
    } else {
        snprintf(int_buf, sizeof(int_buf), "%%0%dd", int_digits);
        snprintf(buffer, sizeof(buffer), int_buf, signed_int);
    }
    
    // 添加小数部分
    if(frac_digits > 0) {
        // 添加小数点
        strcat(buffer, ".");
        
        // 添加小数部分
        char frac_buf[6] = {0}; // 最多5位小数
        snprintf(frac_buf, sizeof(frac_buf), "%%0%du", frac_digits);
        
        char frac_str[6] = {0};
        snprintf(frac_str, sizeof(frac_str), frac_buf, frac_part);
        
        // 确保小数部分有正确位数（可能因进位变短）
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
    
    // 显示字符串（自动处理换行和边界）
    OLED_ShowString(x, y, buffer, mode);
}

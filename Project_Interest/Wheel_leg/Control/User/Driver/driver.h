#ifndef __DRIVER_H
#define __DRIVER_H

#include "can_drv.h"
#include "spi_drv.h"
#include "gpio_drv.h"
#include "uart_drv.h"

#include "BMI088driver.h"
#include "BMI088reg.h"
#include "BMI088Middleware.h"

void Driver_Init(void);

#endif

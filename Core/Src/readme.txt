* 启动文件需要更具单片机类型选择 `startup_stm32f101xb.s` 



配置：
* 配置单片机：修改`system_stm32f10x.c`。 需要定义 STM32F103xB （才能包含头文件 stm32f103xb.h）,  定义USE_HAL_DRIVER（才能包含stm32f1xx_hal_conf.h）
* 配置HAL库：修改`stm32f1xx_hal_conf.h` ，使能你需要使用的外设
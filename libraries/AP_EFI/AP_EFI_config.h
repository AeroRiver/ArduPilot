#pragma once

#include <AP_HAL/AP_HAL_Boards.h>
#include <AP_CANManager/AP_CANSensor.h>

#ifndef HAL_EFI_ENABLED
#define HAL_EFI_ENABLED BOARD_FLASH_SIZE > 1024
#endif

#ifndef AP_EFI_BACKEND_DEFAULT_ENABLED
#define AP_EFI_BACKEND_DEFAULT_ENABLED HAL_EFI_ENABLED
#endif

#ifndef AP_EFI_CURRAWONG_ECU_ENABLED
#define AP_EFI_CURRAWONG_ECU_ENABLED AP_EFI_BACKEND_DEFAULT_ENABLED && HAL_MAX_CAN_PROTOCOL_DRIVERS
#endif

#ifndef AP_EFI_DRONECAN_ENABLED
#define AP_EFI_DRONECAN_ENABLED AP_EFI_BACKEND_DEFAULT_ENABLED && HAL_ENABLE_DRONECAN_DRIVERS
#endif

#ifndef AP_EFI_NWPWU_ENABLED
#define AP_EFI_NWPWU_ENABLED AP_EFI_BACKEND_DEFAULT_ENABLED && HAL_MAX_CAN_PROTOCOL_DRIVERS
#endif

#ifndef AP_EFI_SCRIPTING_ENABLED
#define AP_EFI_SCRIPTING_ENABLED (AP_EFI_BACKEND_DEFAULT_ENABLED && AP_SCRIPTING_ENABLED)
#endif

#ifndef AP_EFI_SERIAL_MS_ENABLED
#define AP_EFI_SERIAL_MS_ENABLED AP_EFI_BACKEND_DEFAULT_ENABLED
#endif

#ifndef AP_EFI_SERIAL_LUTAN_ENABLED
#define AP_EFI_SERIAL_LUTAN_ENABLED AP_EFI_BACKEND_DEFAULT_ENABLED
#endif

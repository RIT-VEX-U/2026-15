#pragma once

#include <stdint.h>
#include "jumptable.h" 
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceGenericSerialEnable, void, (V5_DeviceT device, int32_t options), (device, options), 0xa50)
JUMPTABLE_FUNC(vexDeviceGenericSerialBaudrate, void, (V5_DeviceT device, int32_t baudrate), (device, baudrate), 0xa54)
JUMPTABLE_FUNC(vexDeviceGenericSerialWriteChar, int32_t, (V5_DeviceT device, uint8_t c), (device, c), 0xa58)
JUMPTABLE_FUNC(vexDeviceGenericSerialWriteFree, int32_t, (V5_DeviceT device), (device), 0xa5c)
JUMPTABLE_FUNC(vexDeviceGenericSerialTransmit, int32_t, (V5_DeviceT device, const uint8_t *buffer, int32_t length), (device, buffer, length), 0xa60)
JUMPTABLE_FUNC(vexDeviceGenericSerialReadChar, int32_t, (V5_DeviceT device), (device), 0xa64)
JUMPTABLE_FUNC(vexDeviceGenericSerialPeekChar, int32_t, (V5_DeviceT device), (device), 0xa68)
JUMPTABLE_FUNC(vexDeviceGenericSerialReceiveAvail, int32_t, (V5_DeviceT device), (device), 0xa6c)
JUMPTABLE_FUNC(vexDeviceGenericSerialReceive, int32_t, (V5_DeviceT device, uint8_t *buffer, int32_t length), (device, buffer, length), 0xa70)
JUMPTABLE_FUNC(vexDeviceGenericSerialFlush, void, (V5_DeviceT device), (device), 0xa74)

#ifdef __cplusplus
}
#endif


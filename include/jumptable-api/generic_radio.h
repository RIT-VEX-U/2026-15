#pragma once

#include <stdint.h>
#include "jumptable.h" 
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceGenericRadioConnection, void, (V5_DeviceT device, char *link_id, int type, bool ov), (device, link_id, type, ov), 0xaa4)
JUMPTABLE_FUNC(vexDeviceGenericRadioWriteFree, int32_t, (V5_DeviceT device), (device), 0xaac)
JUMPTABLE_FUNC(vexDeviceGenericRadioTransmit, int32_t, (V5_DeviceT device, const uint8_t *data, uint16_t size), (device, data, size), 0xab0)
JUMPTABLE_FUNC(vexDeviceGenericRadioReceiveAvail, int32_t, (V5_DeviceT device), (device), 0xabc)
JUMPTABLE_FUNC(vexDeviceGenericRadioReceive, int32_t, (V5_DeviceT device, uint8_t *data, uint16_t size), (device, data, size), 0xac0)
JUMPTABLE_FUNC(vexDeviceGenericRadioLinkStatus, bool, (V5_DeviceT device), (device), 0xac8)

#ifdef __cplusplus
}
#endif


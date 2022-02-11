#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>


EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD VMouseDriverEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP VMouseDriverEvtDriverContextCleanup;

EXTERN_C_END

/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#include "Device.h"
#include "Queue.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD VmouseEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP VmouseEvtDriverContextCleanup;

EXTERN_C_END

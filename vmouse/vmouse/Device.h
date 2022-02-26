/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <hidport.h>

#define VENDOR_ID            0x00
#define PRODUCT_ID            0x00
#define VERSION_NUMBER        0x00

typedef UCHAR HID_REPORT_DESCRIPTOR, * PHID_REPORT_DESCRIPTOR;

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    WDFDEVICE               device;
    WDFQUEUE                queueDefault;
    WDFQUEUE                queueManual;

    HID_DEVICE_ATTRIBUTES   hidDeviceAttributes;
    PHID_DESCRIPTOR         hidDescriptor;
    PHID_REPORT_DESCRIPTOR  hidReportDescriptor;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
VmouseCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );
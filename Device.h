#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <hidport.h>

EXTERN_C_START

#define VENDOR_ID            0x00
#define PRODUCT_ID            0x00
#define VERSION_NUMBER        0x00

typedef UCHAR HID_REPORT_DESCRIPTOR, * PHID_REPORT_DESCRIPTOR;

typedef struct _DEVICE_CONTEXT {
    WDFDEVICE               device;
    WDFQUEUE                queueDefault;
    WDFQUEUE                queueManual;

    HID_DEVICE_ATTRIBUTES   hidDeviceAttributes;
    PHID_DESCRIPTOR         hidDescriptor;
    PHID_REPORT_DESCRIPTOR  hidReportDescriptor;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;


WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

NTSTATUS
VMouseDriverCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

EXTERN_C_END

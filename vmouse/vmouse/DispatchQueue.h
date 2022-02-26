#pragma once

#include <ntddk.h>
#include <wdf.h>

#include "Device.h"
#include "Hid.h"

typedef struct _DISPATCH_QUEUE_CONTEXT {
    WDFDEVICE           device;
    WDFQUEUE            queue;
} DISPATCH_QUEUE_CONTEXT, * PDISPATCH_QUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DISPATCH_QUEUE_CONTEXT, DispatchQueueGetContext);

NTSTATUS
DispatchQueueInitialize(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
);

NTSTATUS
DispatchQueueSendReport(    
    _In_ WDFREQUEST OutputRequest,    
    _In_ PDEVICE_CONTEXT DeviceContext
);
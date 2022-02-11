#pragma once

#include <ntddk.h>
#include <wdf.h>

EXTERN_C_START

typedef struct _QUEUE_DEFAULT_CONTEXT {
    WDFDEVICE            device;
    WDFQUEUE             queue;
} QUEUE_DEFAULT_CONTEXT, * PQUEUE_DEFAULT_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_DEFAULT_CONTEXT, QueueDefaultGetContext)

NTSTATUS
VMouseDriverQueueInitialize(
    _In_ WDFDEVICE device,
    _Out_ WDFQUEUE* queue
    );

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL VMouseDriverEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP VMouseDriverEvtIoStop;

EXTERN_C_END

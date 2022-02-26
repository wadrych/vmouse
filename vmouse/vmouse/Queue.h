/*++

Module Name:

    queue.h

Abstract:

    This file contains the queue definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#pragma once

#include <ntddk.h>
#include <wdf.h>

#include "Device.h"

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {
    WDFDEVICE            device;
    WDFQUEUE             queue;

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
vmouseQueueInitialize(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL vmouseEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP vmouseEvtIoStop;


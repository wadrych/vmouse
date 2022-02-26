/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "Queue.h"
#include "DispatchQueue.h"
#include "HelperFunctions.h"
#include "Device.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, vmouseQueueInitialize)
#endif

NTSTATUS
vmouseQueueInitialize(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
    )
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_OBJECT_ATTRIBUTES attributes;
    PQUEUE_CONTEXT context;

    PAGED_CODE();

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchSequential
        );

    queueConfig.EvtIoInternalDeviceControl = vmouseEvtIoDeviceControl;
    queueConfig.EvtIoStop = vmouseEvtIoStop;
	
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, QUEUE_CONTEXT);

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if(!NT_SUCCESS(status)) {
        return status;
    }


    context = QueueGetContext(queue);
    context->device = Device;
    context->queue = queue;
    *Queue = queue;

    return status;
}

VOID
vmouseEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    NTSTATUS status;
    PDEVICE_CONTEXT deviceContext = DeviceGetContext(QueueGetContext(Queue)->device);
	
    switch(IoControlCode) {
	    case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
	        status = CopyToRequestBuffer(
	            Request,
	            deviceContext->hidDescriptor,
	            deviceContext->hidDescriptor->bLength
	        );
	        WdfRequestComplete(Request, status);
	        break;
	    case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
	        status = CopyToRequestBuffer(
	            Request,
	            &deviceContext->hidDeviceAttributes,
	            sizeof(HID_DEVICE_ATTRIBUTES)
	        );
	        WdfRequestComplete(Request, status);
	        break;
	    case IOCTL_HID_GET_REPORT_DESCRIPTOR:
	        status = CopyToRequestBuffer(
	            Request,
	            deviceContext->hidReportDescriptor,
	            deviceContext->hidDescriptor->DescriptorList[0].wReportLength
	        );
	        WdfRequestComplete(Request, status);
	        break;
	    case IOCTL_HID_READ_REPORT:
	    case IOCTL_HID_GET_INPUT_REPORT:
	        status = WdfRequestForwardToIoQueue(
	            Request,
	            deviceContext->queueManual
	        );
	        if(!NT_SUCCESS(status)) {
	            WdfRequestComplete(Request, status);
	        }
	        break;
	    case IOCTL_HID_WRITE_REPORT:
	    case IOCTL_HID_SET_OUTPUT_REPORT:
	        status = DispatchQueueSendReport(
	            Request,
	            deviceContext
	        );
	        WdfRequestComplete(Request, status);
	        break;
	    default:
	        status = STATUS_NOT_IMPLEMENTED;
	        WdfRequestComplete(Request, status);
	        break;
	}

    return;
}

VOID
vmouseEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
{
	UNREFERENCED_PARAMETER(Queue);
	UNREFERENCED_PARAMETER(Request);
	UNREFERENCED_PARAMETER(ActionFlags);
    return;
}

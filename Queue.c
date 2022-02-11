#include "Driver.h"
#include "Queue.h"
#include "Device.h"
#include "Memory.h"
#include "QueueManual.h"



NTSTATUS
VMouseDriverQueueInitialize(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* queueOut
    )
{
    WDFQUEUE queue = NULL;
    NTSTATUS status =STATUS_SUCCESS;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_OBJECT_ATTRIBUTES attributes;
    PQUEUE_DEFAULT_CONTEXT context = NULL;
    
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchSequential
        );

    queueConfig.EvtIoDeviceControl = VMouseDriverEvtIoDeviceControl;
    queueConfig.EvtIoStop = VMouseDriverEvtIoStop;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, QUEUE_DEFAULT_CONTEXT);

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 &attributes,
                 &queue
                 );

    if(!NT_SUCCESS(status)) 
    {
        return status;
    }

    context = QueueDefaultGetContext(queue);
    context->device = Device;
    context->queue = queue;
    *queueOut = queue;
	
    return status;
}

VOID
VMouseDriverEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    NTSTATUS                status = STATUS_SUCCESS;
    PDEVICE_CONTEXT         deviceContext = DeviceGetContext(QueueDefaultGetContext(Queue)->device);

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
        status = QueueManualSendReport(
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
}

VOID
VMouseDriverEvtIoStop(
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

#include "DispatchQueue.h"

#include "HelperFunctions.h"

_Use_decl_annotations_
NTSTATUS
DispatchQueueInitialize(
    _In_  WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue)
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_OBJECT_ATTRIBUTES queueAttributes;
    PDISPATCH_QUEUE_CONTEXT context;

    PAGED_CODE();

    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttributes, DISPATCH_QUEUE_CONTEXT);

    status = WdfIoQueueCreate(Device, &queueConfig, &queueAttributes, &queue);
	
    if(!NT_SUCCESS(status)) {
        return status;
    }

    context = DispatchQueueGetContext(queue);
    context->device = Device;
    context->queue = queue;

    *Queue = queue;

    return status;
}


_Use_decl_annotations_
NTSTATUS
DispatchQueueSendReport(
    _In_ WDFREQUEST OutputRequest,
    _In_ PDEVICE_CONTEXT DeviceContext)
{
    NTSTATUS status;
    WDFQUEUE queue = DeviceContext->queueManual;
    size_t inputReportRequiredSize = 0;

    WDFREQUEST inputRequest;
    WDF_REQUEST_PARAMETERS outputRequestParams;
    HID_XFER_PACKET hidXferPacket;

    WDF_REQUEST_PARAMETERS_INIT(&outputRequestParams);
    WdfRequestGetParameters(OutputRequest, &outputRequestParams);

    if(outputRequestParams.Parameters.DeviceIoControl.InputBufferLength < sizeof(HID_XFER_PACKET)) {
        status = STATUS_BUFFER_TOO_SMALL;
        return status;
    }

    RtlCopyMemory(&hidXferPacket, WdfRequestWdmGetIrp(OutputRequest)->UserBuffer, sizeof(HID_XFER_PACKET));

    switch(hidXferPacket.reportId) {
    case REPORT_ID_MOUSE_OUTPUT:
        hidXferPacket.reportId = REPORT_ID_MOUSE_INPUT;
        hidXferPacket.reportBuffer[0] = hidXferPacket.reportId;
        inputReportRequiredSize = sizeof(HID_MOUSE_INPUT_REPORT);
        break;
    default:
        status = STATUS_INVALID_PARAMETER;
        return status;
    }

    if(hidXferPacket.reportBufferLen < inputReportRequiredSize) {
        status = STATUS_INVALID_BUFFER_SIZE;
        return status;
    }

    status = WdfIoQueueRetrieveNextRequest(queue, &inputRequest);
    if(!NT_SUCCESS(status)) {
        return status;
    }
    status = CopyToRequestBuffer(inputRequest, hidXferPacket.reportBuffer, inputReportRequiredSize);

    WdfRequestComplete(inputRequest, status);

    return status;
}
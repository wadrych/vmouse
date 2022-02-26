#include "HelperFunctions.h"

_Use_decl_annotations_

NTSTATUS
CopyToRequestBuffer(
	_In_ WDFREQUEST       Request,
	_In_ PVOID            SourceBuffer,
	_In_ size_t           NumBytesToCopy)
{
	NTSTATUS    status;
	WDFMEMORY   memory;
	size_t      outputBufferLength;

	if(NumBytesToCopy <= 0) {
		status = STATUS_INVALID_BUFFER_SIZE;
		return status;
	}

	status = WdfRequestRetrieveOutputMemory(Request, &memory);
	if(!NT_SUCCESS(status)) {
		return status;
	}

	WdfMemoryGetBuffer(memory, &outputBufferLength);
	if(outputBufferLength < NumBytesToCopy) {
		status = STATUS_INVALID_BUFFER_SIZE;
		return status;
	}

	status = WdfMemoryCopyFromBuffer(memory, 0, SourceBuffer, NumBytesToCopy);
	if(!NT_SUCCESS(status)) {
		return status;
	}

	WdfRequestSetInformation(Request, NumBytesToCopy);
	return status;
}
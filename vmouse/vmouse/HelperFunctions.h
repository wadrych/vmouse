#pragma once
#include <ntddk.h>
#include <wdf.h>

NTSTATUS
CopyToRequestBuffer(
    _In_ WDFREQUEST       Request,
    _In_ PVOID            SourceBuffer,
    _In_ size_t           NumBytesToCopy
);
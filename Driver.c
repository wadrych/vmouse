#include "Driver.h"
#include "Device.h"


NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status = STATUS_SUCCESS;

    WDF_DRIVER_CONFIG_INIT(&config,
                           VMouseDriverEvtDeviceAdd
                           );

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
						     WDF_NO_OBJECT_ATTRIBUTES,
                             &config,
                             WDF_NO_HANDLE
                             );

    return status;
}

NTSTATUS
VMouseDriverEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    UNREFERENCED_PARAMETER(Driver);
	
    NTSTATUS status = STATUS_SUCCESS;

    status = VMouseDriverCreateDevice(DeviceInit);

    return status;
}

VOID
VMouseDriverEvtDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
    )
{
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE();
}

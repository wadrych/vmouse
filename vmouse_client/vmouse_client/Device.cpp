#pragma comment(lib, "Cfgmgr32")
#pragma comment(lib, "hid")

#include "Device.h"

#include <stdexcept>

#include <cfgmgr32.h>
#include "hidsdi.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>

Device::Device(PCWSTR device_interface)
	: p_device_interface_{ device_interface } {
}

void Device::Initialize() {
	if(IsRunning()) throw std::runtime_error{ "ERROR_DOUBLE_INITIALIZATION" };

	GUID guid;
	HidD_GetHidGuid(&guid);

	ULONG device_interface_list_length = 0;
	CONFIGRET config_return = CM_Get_Device_Interface_List_Size(
		&device_interface_list_length,
		&guid,
		nullptr,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	);

	if(CR_SUCCESS != config_return) throw std::runtime_error{ "ERROR_GETTING_DEVICE_INTERFACE_LIST_SIZE" };

	if(device_interface_list_length <= 1) throw std::runtime_error{ "ERROR_EMPTY_DEVICE_INTERFACE_LIST" };

	const PWSTR device_interface_list = static_cast<PWSTR>(malloc(device_interface_list_length * sizeof(WCHAR)));
	if(nullptr == device_interface_list) throw std::runtime_error{ "ERROR_ALLOCATING_DEVICE_INTERFACE_LIST" };
	ZeroMemory(device_interface_list, device_interface_list_length * sizeof(WCHAR));

	config_return = CM_Get_Device_Interface_List(
		&guid,
		nullptr,
		reinterpret_cast<PZZWSTR>(device_interface_list),
		device_interface_list_length,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	);

	if(CR_SUCCESS != config_return) {
		free(device_interface_list);
		throw std::runtime_error("ERROR_GETTING_DEVICE_INTERFACE_LIST");
	}

	const size_t device_interface_length = wcslen(p_device_interface_);
	HANDLE file = INVALID_HANDLE_VALUE;
	for(PWSTR current_interface = device_interface_list; *current_interface; current_interface += wcslen(current_interface) + 1) {
		if(0 != wcsncmp(current_interface, p_device_interface_, device_interface_length)) {
			continue;
		}

		file = CreateFileW(current_interface, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
		if(INVALID_HANDLE_VALUE == file) {
			continue;
		}

		break;
	}
	free(device_interface_list);
	if(INVALID_HANDLE_VALUE == file) throw std::runtime_error{ "ERROR_INVALID_HANDLE_VALUE" };

	p_device_handle_ = file;
	is_running_ = true;
}


bool Device::IsRunning() const {
	return is_running_;
}

bool Device::IsStopped() const {
	return is_stopped_;
}

void Device::Stop() {
	if(nullptr != p_device_handle_ && INVALID_HANDLE_VALUE != p_device_handle_) {
		CloseHandle(p_device_handle_);
	}
	is_stopped_ = true;
}

void Device::SetOutputReport(PVOID data, DWORD size) const {
	if(!HidD_SetOutputReport(p_device_handle_, data, size)) {
		std::cout << GetLastError() << std::endl;
		throw std::runtime_error{ "ERROR_SET_OUTPUT_REPORT" };
	}
}
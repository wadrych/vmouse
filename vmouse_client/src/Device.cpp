#include <vmouse_client/Device.hpp>

#include <stdexcept>

#include <cfgmgr32.h>
#include "hidsdi.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>

Device::Device(PCWSTR deviceInterface)
	: mp_deviceInterface{ deviceInterface } {
}

void Device::initialize() {
	if(isInitialized()) throw std::runtime_error{ "ERROR_DOUBLE_INITIALIZATION" };

	GUID guid;
	HidD_GetHidGuid(&guid);

	ULONG deviceInterfaceListLength = 0;
	CONFIGRET configret = CM_Get_Device_Interface_List_Size(
		&deviceInterfaceListLength,
		&guid,
		nullptr,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	);

	if(CR_SUCCESS != configret) throw std::runtime_error{ "ERROR_GETTING_DEVICE_INTERFACE_LIST_SIZE" };

	if(deviceInterfaceListLength <= 1) throw std::runtime_error{ "ERROR_EMPTY_DEVICE_INTERFACE_LIST" };

	PWSTR deviceInterfaceList = static_cast<PWSTR>(malloc(deviceInterfaceListLength * sizeof(WCHAR)));
	if(nullptr == deviceInterfaceList) throw std::runtime_error{ "ERROR_ALLOCATING_DEVICE_INTERFACE_LIST" };
	ZeroMemory(deviceInterfaceList, deviceInterfaceListLength * sizeof(WCHAR));

	configret = CM_Get_Device_Interface_List(
		&guid,
		nullptr,
		reinterpret_cast<PZZSTR>(deviceInterfaceList),
		deviceInterfaceListLength,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	);

	if(CR_SUCCESS != configret) {
		free(deviceInterfaceList);
		throw std::runtime_error("ERROR_GETTING_DEVICE_INTERFACE_LIST");
	}

	size_t deviceInterfaceLength = wcslen(mp_deviceInterface);
	HANDLE file = INVALID_HANDLE_VALUE;
	for(PWSTR currentInterface = deviceInterfaceList; *currentInterface; currentInterface += wcslen(currentInterface) + 1) {
		std::wstring s1(currentInterface);
		std::wstring s2(mp_deviceInterface);

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string conv_s1 = converter.to_bytes(s1);
		std::string conv_s2 = converter.to_bytes(s2);

		std::cout << "curr: " << conv_s1 << std::endl;
		std::cout << "desired: " << conv_s2 << std::endl;
		if(0 != wcsncmp(currentInterface, mp_deviceInterface, deviceInterfaceLength)) {
			std::cout << "strings doesn't match going to the next" << std::endl;
			continue;
		}

		file = CreateFileW(currentInterface, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
		if(INVALID_HANDLE_VALUE == file) {
			std::cout << "INVALID_HANDLE_VALUE going to the next" << std::endl;
			continue;
		}

		std::cout << "breaked from the loop " << std::endl;
		break;
	}
	std::cout << "achieved end of the loop" << std::endl;
	free(deviceInterfaceList);
	if(INVALID_HANDLE_VALUE == file) throw std::runtime_error{ "ERROR_INVALID_HANDLE_VALUE" };

	mp_deviceHandle = file;
	m_isInitialized = true;
}


bool Device::isInitialized() const {
	return m_isInitialized;
}

bool Device::isAborted() const {
	return m_isAborted;
}

void Device::abort() {
	if(nullptr != mp_deviceHandle && INVALID_HANDLE_VALUE != mp_deviceHandle) {
		CloseHandle(mp_deviceHandle);
	}
	m_isAborted = true;
}

void Device::setOutputReport(PVOID data, DWORD size) const {
	if(!HidD_SetOutputReport(mp_deviceHandle, data, size)) {
		std::cout << GetLastError() << std::endl;
		throw std::runtime_error{ "ERROR_SET_OUTPUT_REPORT" };
	}
}
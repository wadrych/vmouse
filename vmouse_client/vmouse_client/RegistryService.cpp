#include "RegistryService.h"

RegistryService& RegistryService::Get() {
    static RegistryService instance;
    return instance;
}

int RegistryService::GetMouseSensitivity() {
    DWORD size = 3;
    char buffer[3];

    const LONG get_value_result = RegGetValueA(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSensitivity", RRF_RT_REG_SZ, nullptr, &buffer, &size);
    if(ERROR_SUCCESS != get_value_result) {
        return -1;
    }

    return strtol(buffer, nullptr, 0);
}

int RegistryService::GetMouseSpeed() {
    DWORD size = 2;
    char buffer[2];

    const LONG get_value_result = RegGetValueA(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", RRF_RT_REG_SZ, nullptr, &buffer, &size);
    if(ERROR_SUCCESS != get_value_result) {
        return -1;
    }

    return strtol(buffer, nullptr, 0);
}

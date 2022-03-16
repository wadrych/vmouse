#pragma once

#include <Windows.h>
#include "winreg.h"

class RegistryService final {

public:
    static RegistryService& Get();

    static int GetMouseSensitivity();
    static int GetMouseSpeed();

private:
    explicit RegistryService() = default;
    RegistryService(const RegistryService&) = delete;
    void operator =(const RegistryService&) = delete;
    virtual ~RegistryService() = default;
};

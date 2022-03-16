#pragma once

#include <Windows.h>

class Device {
public:
	explicit Device(PCWSTR device_interface);
	Device(const Device&) = delete;
	void operator =(const Device&) = delete;
	virtual ~Device() = default;

	virtual void Initialize();
	virtual void Stop();

	bool IsRunning() const;
	bool IsStopped() const;

protected:
	void SetOutputReport(PVOID data, DWORD size) const;

private:
	bool is_running_{ false };
	bool is_stopped_{ false };
	PCWSTR p_device_interface_{ nullptr };
	HANDLE p_device_handle_{ nullptr };
};
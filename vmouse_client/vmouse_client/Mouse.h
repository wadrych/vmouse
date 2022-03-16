#pragma once

#include "Device.h"
#include "RegistryService.h"

#include <Windows.h>
#include <vector>
#include <stdexcept>
#include <iostream>

class Mouse : public Device {
public:
	explicit Mouse();
	Mouse(const Mouse&) = delete;
	void operator =(const Mouse&) = delete;
	virtual ~Mouse() override = default;

	virtual void Initialize() override;
	virtual void Stop() override;

	virtual void LeftBtnDown();
	virtual void LeftBtnUp();
	virtual void LeftBtnClick();

	virtual void RightBtnDown();
	virtual void RightBtnUp();
	virtual void RightBtnClick();

	virtual void MiddleBtnDown();
	virtual void MiddleBtnUp();
	virtual void MiddleBtnClick();


	virtual void MoveCursor(POINT);
	virtual void MoveCursor(LONG x, LONG y);

protected:
	enum button {
		button_none = 0x00,
		button_left = 0x01,
		button_right = 0x02,
		button_middle = 0x04
	};

	void PopulateRangeSpeedVector();
	int GetSpeedByRange(int range) const;

	static POINT GetCurrentCursorPosition(LPDWORD error);

	BYTE buttons_status_{ button_none };
	std::vector<int> range_speed_vector_;

	static const LONG max_absolute_speed{ 100 };
	static const int  distance_tolerance{ 4 };
	static const BYTE report_id{ 0x02 };

private:
#pragma pack(1)
	struct Report {
		BYTE report_id;
		BYTE buttons;
		CHAR x;
		CHAR y;
	};
#pragma pack()

	virtual void SendReport(CHAR horizontal_speed, CHAR vertical_speed);
};
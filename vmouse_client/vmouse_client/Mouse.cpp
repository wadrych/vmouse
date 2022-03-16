#include "Mouse.h"

Mouse::Mouse()
    : Device{ L"\\\\?\\HID#MOUSE_DEVICE&Col02#1" } {
}

void Mouse::Initialize() {
    if(IsRunning()) throw std::runtime_error{ "ERROR_DOUBLE_INITIALIZATION" };
    PopulateRangeSpeedVector();
    Device::Initialize();
}

void Mouse::LeftBtnDown() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ |= button_left;
    SendReport(0, 0);
}


void Mouse::LeftBtnUp() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ &= ~button_left;
    SendReport(0, 0);
}

void Mouse::LeftBtnClick() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    LeftBtnDown();
    LeftBtnUp();
}

void Mouse::MiddleBtnDown() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ |= button_middle;
    SendReport(0, 0);
}

void Mouse::MiddleBtnUp() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ &= ~button_middle;
    SendReport(0, 0);
}

void Mouse::MiddleBtnClick() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    MiddleBtnDown();
    MiddleBtnUp();
}

void Mouse::RightBtnDown() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ |= button_right;
    SendReport(0, 0);
}

void Mouse::RightBtnUp() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    buttons_status_ &= ~button_right;
    SendReport(0, 0);
}

void Mouse::RightBtnClick() {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    RightBtnDown();
    RightBtnUp();
}

void Mouse::MoveCursor(POINT point) {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    MoveCursor(point.x, point.y);
}

void Mouse::MoveCursor(LONG x, LONG y) {
    if(!IsRunning()) throw std::runtime_error{ "ERROR_NOT_INITIALIZED" };
    std::cout << "Entered moveCursor" << std::endl;

    while(!IsStopped()) {
        DWORD get_current_cursor_position_error = ERROR_SUCCESS;
        const POINT current_point = GetCurrentCursorPosition(&get_current_cursor_position_error);
        if(ERROR_SUCCESS != get_current_cursor_position_error) throw std::runtime_error{ "ERROR_CURSOR_PROCESSING" };

        const LONG distance = static_cast<LONG>(sqrt(pow(x - current_point.x, 2) + pow(y - current_point.y, 2)));

        if(distance <= distance_tolerance) {
            return;
        }

        CHAR horizontal_speed = static_cast<CHAR>(GetSpeedByRange(abs(x - current_point.x)));
        horizontal_speed = (x > current_point.x ? horizontal_speed : -horizontal_speed);

        CHAR vertical_speed = static_cast<CHAR>(GetSpeedByRange(abs(y - current_point.y)));
        vertical_speed = (y > current_point.y ? vertical_speed : -vertical_speed);

        std::cout << "Speed " << static_cast<int>(horizontal_speed) << " " << static_cast<int>(vertical_speed) << std::endl;
        SendReport(horizontal_speed, vertical_speed);
        Sleep(1);
    }
}

void Mouse::Stop() {
    Device::Stop();
}

void Mouse::PopulateRangeSpeedVector() {
    range_speed_vector_.clear();
    int range_index = 0;
    for(int speed{ 0 }; speed < max_absolute_speed + 1; ++speed) {
	    const int range{ speed };
        while(range_index <= range) {
            range_speed_vector_.push_back(speed);
            ++range_index;
        }
    }
}

int Mouse::GetSpeedByRange(int range) const {
    if(range_speed_vector_.empty()) {
        return static_cast<int>(sqrt(range));
    }
    if(static_cast<size_t>(range) > range_speed_vector_.size() - 1) {
        range = static_cast<int>(range_speed_vector_.size()) - 1;
    }
    return range_speed_vector_.at(range);
}

POINT Mouse::GetCurrentCursorPosition(const LPDWORD error) {
    POINT current_position{ 0, 0 };
    const BOOL get_cursor_pos_result = GetCursorPos(&current_position);
    if(!get_cursor_pos_result && error != nullptr) {
        *error = GetLastError();
    }
	
    return current_position;
}

void Mouse::SendReport(const CHAR horizontal_speed, const CHAR vertical_speed) {
    Report report{};
    report.report_id = report_id;
    report.buttons = buttons_status_;
    report.x = horizontal_speed;
    report.y = vertical_speed;

    SetOutputReport(&report, static_cast<DWORD>(sizeof(Report)));
}
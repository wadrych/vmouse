#pragma once

#include <ntddk.h>
#include <wdf.h>

#define REPORT_ID_MOUSE_INPUT       0x01
#define REPORT_ID_MOUSE_OUTPUT      0x02


#pragma pack(1)
typedef struct _HID_MOUSE_INPUT_REPORT {
    BYTE                reportId;
    BYTE                buttons;
    CHAR                x;
    CHAR                y;
} HID_MOUSE_INPUT_REPORT, * PHID_MOUSE_INPUT_REPORT;

typedef struct _HID_MOUSE_OUTPUT_REPORT {
    BYTE                reportId;
    BYTE                buttons;
    CHAR                x;
    CHAR                y;
} HID_MOUSE_OUTPUT_REPORT, * PHID_MOUSE_OUTPUT_REPORT;

#pragma pack()
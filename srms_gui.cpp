
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
const std::string REPORT_FILE = "C:\\Users\\Asus\\OneDrive\\Desktop\\yass\\student.txt";
HWND hRoll, hName, hMarks[5], hOutput;
HFONT hAppFont = NULL;
HBRUSH hMainBgBrush = NULL;
HBRUSH hGroupBrush = NULL;
HBRUSH hPanelBrush = NULL;
HFONT hMonoFont = NULL;
#define IDC_HEADER_LEFT 1001
#define IDC_HEADER_RIGHT 1002
HWND hBtnSave = NULL, hBtnShow = NULL, hBtnClear = NULL, hBtnExit = NULL;
HICON hIconSave = NULL, hIconShow = NULL, hIconClear = NULL, hIconExit = NULL;
char gradeFromAverage(double avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 50) return 'D';
    if (avg >= 35) return 'E';
    return 'F';
}
void SaveRecord(HWND hwnd) {
    char buf[256];
    GetWindowTextA(hRoll, buf, sizeof(buf));
    int roll = atoi(buf);
    GetWindowTextA(hName, buf, sizeof(buf));
    std::string name = buf;
    double marks[5];
    double total = 0;
    for (int i = 0; i < 5; ++i) {
        GetWindowTextA(hMarks[i], buf, sizeof(buf));
        marks[i] = atof(buf);
        total += marks[i];
    }
    double average = total / 5.0;
    char grade = gradeFromAverage(average);
    std::ofstream fout(REPORT_FILE, std::ios::app);
    if (!fout) {
        MessageBoxA(hwnd, "Failed to open report file for writing.", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    fout << "\n----------------------------------" << "\n";
    fout << "Roll No: " << roll << "\n";
    fout << "Name   : " << name << "\n";
    fout << "Total  : " << total << "\n";
    fout << std::fixed << std::setprecision(2);
    fout << "Average: " << average << "\n";
    fout << "Grade  : " << grade << "\n";
    fout << "Subject-wise Result:" << "\n";
    for (int i = 0; i < 5; ++i) {
        fout << "  Subject " << i+1 << ": " << marks[i] << " (" << (marks[i] >= 35 ? "Pass" : "Fail") << ")\n";
    }
    fout << "----------------------------------\n";
    fout.close();

    MessageBoxA(hwnd, "Record saved successfully.", "Saved", MB_OK | MB_ICONINFORMATION);
}
void ShowAllRecords() {
    std::ifstream fin(REPORT_FILE);
    std::ostringstream oss;
    if (!fin) {
        oss << "No records found (file missing).";
    } else {
        std::string line;
        while (std::getline(fin, line)) {
            oss << line << "\r\n";
        }
    }
    std::string out = oss.str();
    SetWindowTextA(hOutput, out.c_str());
}
void ClearAllRecords(HWND hwnd) {
    std::ofstream fout(REPORT_FILE, std::ios::trunc);
    if (!fout) {
        MessageBoxA(hwnd, "Failed to clear records.", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    fout.close();
    SetWindowTextA(hOutput, "");
    MessageBoxA(hwnd, "All records cleared.", "Cleared", MB_OK | MB_ICONINFORMATION);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hAppFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                               ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, "Segoe UI");
        CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 8, 8, 380, 320, hwnd, NULL, NULL, NULL);
        CreateWindowA("STATIC", "Student Info", WS_VISIBLE | WS_CHILD, 16, 10, 200, 20, hwnd, (HMENU)IDC_HEADER_LEFT, NULL, NULL);

        CreateWindowA("STATIC", "Roll No:", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 40, 100, 24, hwnd, NULL, NULL, NULL);
        hRoll = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP, 125, 40, 140, 24, hwnd, NULL, NULL, NULL);

        CreateWindowA("STATIC", "Name:", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 76, 100, 24, hwnd, NULL, NULL, NULL);
        hName = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP, 125, 76, 260, 24, hwnd, NULL, NULL, NULL);

        CreateWindowA("STATIC", "Marks (out of 100):", WS_VISIBLE | WS_CHILD, 20, 110, 200, 20, hwnd, NULL, NULL, NULL);
        for (int i = 0; i < 5; ++i) {
            std::string label = "Subject " + std::to_string(i+1) + ":";
            CreateWindowA("STATIC", label.c_str(), WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 140 + i*28, 100, 24, hwnd, NULL, NULL, NULL);
            hMarks[i] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_TABSTOP, 125, 140 + i*28, 100, 24, hwnd, NULL, NULL, NULL);
        }
        hBtnSave = CreateWindowA("BUTTON", "Save", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP, 240, 140, 120, 38, hwnd, (HMENU)1, NULL, NULL);
        hBtnShow = CreateWindowA("BUTTON", "Show All", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP, 240, 186, 120, 38, hwnd, (HMENU)2, NULL, NULL);
        hBtnClear = CreateWindowA("BUTTON", "Clear All", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP, 240, 232, 120, 38, hwnd, (HMENU)3, NULL, NULL);
        hBtnExit = CreateWindowA("BUTTON", "Exit", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | WS_TABSTOP, 240, 278, 120, 38, hwnd, (HMENU)4, NULL, NULL);

        hIconSave = LoadIconA(NULL, IDI_APPLICATION);
        hIconShow = LoadIconA(NULL, IDI_INFORMATION);
        hIconClear = LoadIconA(NULL, IDI_WARNING);
        hIconExit = LoadIconA(NULL, IDI_ERROR);

        CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 398, 8, 360, 560-16, hwnd, NULL, NULL, NULL);
        CreateWindowA("STATIC", "Records", WS_VISIBLE | WS_CHILD, 406, 10, 200, 20, hwnd, (HMENU)IDC_HEADER_RIGHT, NULL, NULL);

        hOutput = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL | ES_WANTRETURN, 406, 40, 336, 520, hwnd, NULL, NULL, NULL);

        hMonoFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");

        hMainBgBrush = CreateSolidBrush(RGB(243, 249, 255));
        hPanelBrush = CreateSolidBrush(RGB(230, 245, 255)); 

        HWND controls[] = { hRoll, hName, hMarks[0], hMarks[1], hMarks[2], hMarks[3], hMarks[4], hBtnSave, hBtnShow, hBtnClear, hBtnExit };
        for (HWND c : controls) {
            if (c) SendMessageA(c, WM_SETFONT, (WPARAM)hAppFont, TRUE);
        }
        if (hOutput) SendMessageA(hOutput, WM_SETFONT, (WPARAM)hMonoFont, TRUE);

        if (hOutput) SendMessageA(hOutput, EM_SETLIMITTEXT, (WPARAM)0x7FFFFFF0, 0);
        break;
    }
    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        if (!pdis) break;

        HDC hdc = pdis->hDC;
        RECT rc = pdis->rcItem;
        HWND hBtn = pdis->hwndItem;
        UINT state = pdis->itemState;

        COLORREF c1 = RGB(0, 150, 60), c2 = RGB(0, 200, 90);
        HICON hI = hIconSave;
        const char* text = "Save";
        if (hBtn == hBtnShow) { c1 = RGB(10, 120, 220); c2 = RGB(60, 160, 240); hI = hIconShow; text = "Show All"; }
        else if (hBtn == hBtnClear) { c1 = RGB(230, 120, 20); c2 = RGB(250, 160, 60); hI = hIconClear; text = "Clear All"; }
        else if (hBtn == hBtnExit) { c1 = RGB(180, 30, 30); c2 = RGB(230, 60, 60); hI = hIconExit; text = "Exit"; }

        if (state & ODS_SELECTED) {
            c1 = RGB(GetRValue(c1)/2, GetGValue(c1)/2, GetBValue(c1)/2);
            c2 = RGB(GetRValue(c2)/2, GetGValue(c2)/2, GetBValue(c2)/2);
        }

        TRIVERTEX tv[2];
        tv[0].x = rc.left; tv[0].y = rc.top; tv[0].Red = GetRValue(c1) << 8; tv[0].Green = GetGValue(c1) << 8; tv[0].Blue = GetBValue(c1) << 8; tv[0].Alpha = 0;
        tv[1].x = rc.right; tv[1].y = rc.bottom; tv[1].Red = GetRValue(c2) << 8; tv[1].Green = GetGValue(c2) << 8; tv[1].Blue = GetBValue(c2) << 8; tv[1].Alpha = 0;
        GRADIENT_RECT gRect = {0,1};
        GradientFill(hdc, tv, 2, &gRect, 1, GRADIENT_FILL_RECT_V);

        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(120,120,120));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        int iconX = rc.left + 8;
        int iconY = rc.top + (rc.bottom-rc.top-20)/2;
        if (hI) DrawIconEx(hdc, iconX, iconY, hI, 20, 20, 0, NULL, DI_NORMAL);

        SetTextColor(hdc, RGB(255,255,255));
        SetBkMode(hdc, TRANSPARENT);
        RECT tr = rc;
        tr.left += 36;
        DrawTextA(hdc, text, -1, &tr, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        return TRUE;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, hMainBgBrush ? hMainBgBrush : (HBRUSH)GetStockObject(WHITE_BRUSH));

        RECT left = {8, 8, 388, 328};
        FillRect(hdc, &left, hGroupBrush);
        RECT inner = {12, 36, 384, 324};
        FillRect(hdc, &inner, hPanelBrush);
        RECT right = {398, 8, 758, 544};
        FillRect(hdc, &right, hGroupBrush);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;
        SetTextColor(hdcStatic, RGB(40, 40, 40));
        SetBkMode(hdcStatic, TRANSPARENT);
        int id = GetDlgCtrlID(hCtrl);
        if (id == IDC_HEADER_LEFT || id == IDC_HEADER_RIGHT) {
            SetTextColor(hdcStatic, RGB(10, 70, 120)); // header blue
            SetBkMode(hdcStatic, TRANSPARENT);
        }
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 1) {
            SaveRecord(hwnd);
        } else if (id == 2) {
            ShowAllRecords();
        } else if (id == 3) {
            ClearAllRecords(hwnd);
        } else if (id == 4) {
            PostQuitMessage(0);
        }
        break;
    }
    case WM_DESTROY:
        if (hAppFont) DeleteObject(hAppFont);
        if (hMainBgBrush) DeleteObject(hMainBgBrush);
        if (hGroupBrush) DeleteObject(hGroupBrush);
        if (hPanelBrush) DeleteObject(hPanelBrush);
        if (hIconSave) DestroyIcon(hIconSave);
        if (hIconShow) DestroyIcon(hIconShow);
        if (hIconClear) DestroyIcon(hIconClear);
        if (hIconExit) DestroyIcon(hIconExit);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SRMS_GUI_Class";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowExA(0, wc.lpszClassName, "Student Report Card System - GUI",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
int main() {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}

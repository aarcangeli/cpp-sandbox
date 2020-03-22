#include <d3d12.h>

const char *PROG_NAME = "D3D12-DXR";

[[noreturn]] void die(const char *error);

HWND initWindow();

void initD3D12(HWND hwnd);

const int width = 1280;
const int height = 720;
ID3D12Device *pDevice;
ID3D12CommandAllocator *pCmdAllocator;
ID3D12GraphicsCommandList *pCmdList;

int main() {
    HWND pHwnd = initWindow();
    initD3D12(pHwnd);
}

HWND initWindow() {
    const char *g_szClassName = "myWindowClass";

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    wc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam,
                        LPARAM lParam) -> LRESULT {
        switch (msg) {
            case WM_CLOSE:
                DestroyWindow(hwnd);
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    };

    if (!RegisterClassEx(&wc)) {
        die("Window Registration Failed!");
    }

    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, style, false);
    LONG actualWidth = rect.right - rect.left;
    LONG actualHeight = rect.bottom - rect.top;

    int left = (GetSystemMetrics(SM_CXFULLSCREEN) / 2) - (actualWidth / 2);
    int top = (GetSystemMetrics(SM_CYFULLSCREEN) / 2) - (actualHeight / 2);

    HWND hwnd = CreateWindowA(g_szClassName, "The title of my window", style,
                              left, top, actualWidth, actualHeight, nullptr,
                              nullptr, GetModuleHandle(nullptr), nullptr);

    if (!hwnd) {
        die("Window Creation Failed!");
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

void initD3D12(HWND hwnd) {
    // Create D3D12
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice));
    if (FAILED(hr)) {
        die("Cannot create D3D12");
    }

    // Check if ray is supported by the system
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 caps = {};
    hr = pDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &caps, sizeof(caps));
    if (FAILED(hr) || caps.RaytracingTier < D3D12_RAYTRACING_TIER_1_0) {
        die("DXR is not supported in this device");
    }

    // Creates command list allocator
    hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));
    if (FAILED(hr)) {
        die("Cannot create command list allocator 0x%lx");
    }

    // Create command list that support ray tracing
    hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, nullptr, IID_PPV_ARGS(&pCmdList));
    if (FAILED(hr)) {
        die("Cannot create command list 0x%lx");
    }

}

void die(const char *error) {
    MessageBox(nullptr, error, PROG_NAME, MB_ICONEXCLAMATION | MB_OK);
    exit(1);
}

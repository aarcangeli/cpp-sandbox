#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdio>

const char *PROG_NAME = "D3D12-DXR";

[[noreturn]] void die(const char *error);

HWND initWindow();

void initD3D12();

void enableDebugLayer();

bool CheckTearingSupport();

ID3D12DescriptorHeap *CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, int numDescriptors);

void UpdateRenderTargetViews();

void Update();

void Render();

void Resize(int width, int height);

const int width = 1280;
const int height = 720;
const int g_NumFrames = 3;

HWND g_hWnd;
IDXGIFactory4 *pFactory;
ID3D12CommandQueue *pCommandQueue;
ID3D12Device *pDevice;
ID3D12CommandAllocator *pCmdAllocator;
ID3D12GraphicsCommandList *pCmdList;
IDXGISwapChain1 *pSwapChain;
ID3D12Resource *g_BackBuffers[g_NumFrames];
ID3D12CommandAllocator *g_CommandAllocators[g_NumFrames];
ID3D12DescriptorHeap *g_RTVDescriptorHeap;

int main() {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    enableDebugLayer();
    g_hWnd = initWindow();
    initD3D12();

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

}

void enableDebugLayer() {
    // Always enable the debug layer before doing anything DX12 related
    // so all possible errors generated while creating DX12 objects
    // are caught by the debug layer.
    ID3D12Debug *debugInterface;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)))) {
        debugInterface->EnableDebugLayer();
    }
}

HWND initWindow() {
    const char *className = "myWindowClass";

    // Create window class
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    wc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        switch (msg) {
            case WM_CLOSE:
                DestroyWindow(hwnd);
                break;
            case WM_PAINT:
                Update();
                Render();
                break;
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
                if (wParam == VK_ESCAPE) {
                    PostQuitMessage(0);
                    break;
                }
                break;
            case WM_SIZE: {
                RECT clientRect = {};
                GetClientRect(g_hWnd, &clientRect);
                int width = clientRect.right - clientRect.left;
                int height = clientRect.bottom - clientRect.top;
                Resize(width, height);
                break;
            }
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

    // Create window
    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, style, false);

    LONG actualWidth = rect.right - rect.left;
    LONG actualHeight = rect.bottom - rect.top;
    int left = (GetSystemMetrics(SM_CXFULLSCREEN) / 2) - (actualWidth / 2);
    int top = (GetSystemMetrics(SM_CYFULLSCREEN) / 2) - (actualHeight / 2);

    HWND hwnd = CreateWindow(className, "The title of my window", style,
                             left, top, actualWidth, actualHeight, nullptr,
                             nullptr, GetModuleHandle(nullptr), nullptr);

    if (!hwnd) {
        die("Window Creation Failed!");
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

void Resize(int width, int height) {
}

ID3D12CommandAllocator *CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type) {
    ID3D12CommandAllocator *commandAllocator;
    if (FAILED(pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)))) {
        die("Cannot create command allocator");
    }
    return commandAllocator;
}

void initD3D12() {
    // Create D3D12
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice));
    if (FAILED(hr)) {
        die("Cannot create D3D12");
    }

#if defined(_DEBUG)
    ID3D12InfoQueue *pInfoQueue;
    if (SUCCEEDED(pDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue)))) {
        printf("Enabling debug messages\n");
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY Severities[] = {D3D12_MESSAGE_SEVERITY_INFO};

//        D3D12_MESSAGE_CATEGORY Categories[] = {};

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID DenyIds[] = {
                D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
                D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
                D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
        };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
//        NewFilter.DenyList.NumCategories = _countof(Categories);
//        NewFilter.DenyList.pCategoryList = Categories;
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        pInfoQueue->PushStorageFilter(&NewFilter);
    }
#endif

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
        die("Cannot create command list");
    }

    // Create command queue
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;
    hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pCommandQueue));
    if (FAILED(hr)) {
        die("Cannot create command queue");
    }

    // Create swap chain
    hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pFactory));
    if (FAILED(hr)) {
        die("Cannot create swap chain factory");
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc = {1, 0};
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    hr = pFactory->CreateSwapChainForHwnd(pCommandQueue, g_hWnd, &swapChainDesc, nullptr, nullptr, &pSwapChain);
    if (FAILED(hr)) {
        die("Cannot create swap chain");
    }

    g_RTVDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);

    UpdateRenderTargetViews();

    for (auto &g_CommandAllocator : g_CommandAllocators) {
        g_CommandAllocator = CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);
    }

}

bool CheckTearingSupport() {
    bool allowTearing = true;

    IDXGIFactory4 *factory4;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4)))) {
        IDXGIFactory5 *factory5;
        if (SUCCEEDED(factory4->QueryInterface(IID_PPV_ARGS(&factory5)))) {
            if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                                                     &allowTearing,
                                                     sizeof(allowTearing)))) {
                allowTearing = FALSE;
            }
        }
    }

    return allowTearing;
}

ID3D12Fence *CreateFence() {
    ID3D12Fence *Fence;
    if (FAILED(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)))) {
        die("Cannot create Fence");
    }
    return Fence;
}

void UpdateRenderTargetViews() {
    HRESULT hr;
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    auto rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (int i = 0; i < g_NumFrames; ++i) {
        ID3D12Resource *backBuffer;
        hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
        if (FAILED(hr)) {
            die("pSwapChain->GetBuffer");
        }
        pDevice->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);
        g_BackBuffers[i] = backBuffer;
        rtvHandle.ptr += rtvDescriptorSize;
    }
}

ID3D12DescriptorHeap *CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, int numDescriptors) {
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = type;

    ID3D12DescriptorHeap *descriptorHeap;
    if (FAILED(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)))) {
        die("Cannot Create Descriptor Heap");
    }

    return descriptorHeap;
}

void Update() {
}

void Render() {
    // present
}

void die(const char *error) {
    MessageBox(nullptr, error, PROG_NAME, MB_ICONEXCLAMATION | MB_OK);
    exit(1);
}

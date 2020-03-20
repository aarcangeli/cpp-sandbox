#include <d3d11.h>
#include <d3dcompiler.h>
#include <utils/string_sprintf.h>
#include <vector>

const char *PROG_NAME = "D3D11";
[[noreturn]] void die(const char *error);
void render();
const int width = 1280;
const int height = 720;
ID3D11Device *device;
IDXGISwapChain *swapChain;
ID3D11RenderTargetView *backbuffer;
ID3D11DeviceContext *context;

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
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

void die(const char *error) {
  MessageBox(nullptr, error, PROG_NAME, MB_ICONEXCLAMATION | MB_OK);
  exit(1);
}

void initD3D11(HWND hwnd) {
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hwnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  D3D_FEATURE_LEVEL level;

  D3D_FEATURE_LEVEL features[] = {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
  };

  HRESULT hr = D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
      D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED, features,
      ARRAYSIZE(features), D3D11_SDK_VERSION, &sd, &swapChain, &device, &level,
      &context);

  if (FAILED(hr)) {
    die("Cannot create D3D11");
  }

  ID3D11Texture2D *pBackbuffer;
  swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackbuffer);
  device->CreateRenderTargetView(pBackbuffer, nullptr, &backbuffer);
  pBackbuffer->Release();
}

int main() {
  HWND pHwnd = initWindow();
  initD3D11(pHwnd);

  MSG msg;
  while (true) {

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        return 0;
    }

    float clearColor[4] = {0, 0, 0, 0};
    context->ClearRenderTargetView(backbuffer, clearColor);
    context->OMSetRenderTargets(1, &backbuffer, nullptr);

    D3D11_VIEWPORT viewport{0, 0, width, height, 0, 1};
    context->RSSetViewports(1, &viewport);

    render();

    swapChain->Present(1, 0);
  }
}

enum class SHADER_TYPE : uint8_t {
  VS,
  PS,
};

class MyShader {
  SHADER_TYPE type;
  ID3DBlob *shader;
  union {
    ID3D11VertexShader *vertexShader = nullptr;
    ID3D11PixelShader *pixelShader;
  };

public:
  MyShader(SHADER_TYPE type, const char *sourceCode) : type(type) {

    const char *target;
    switch (type) {
    case SHADER_TYPE::VS:
      target = "vs_5_0";
      break;
    case SHADER_TYPE::PS:
      target = "ps_5_0";
      break;
    default:
      die("???");
    }

    ID3DBlob *error;
    HRESULT hr = D3DCompile(sourceCode, strlen(sourceCode), "shader.txt",
                            nullptr, nullptr, "main", target, D3DCOMPILE_DEBUG,
                            0, &shader, &error);

    if (FAILED(hr)) {
      printf("Cannot compile shader:\n %s\n", (char*) error->GetBufferPointer());
      die(string_sprintf("Cannot compile shader: %s", error->GetBufferPointer())
              .c_str());
    }

    switch (type) {
    case SHADER_TYPE::VS:
      hr = device->CreateVertexShader(shader->GetBufferPointer(),
                                      shader->GetBufferSize(), nullptr,
                                      &vertexShader);
      break;
    case SHADER_TYPE::PS:
      hr = device->CreatePixelShader(shader->GetBufferPointer(),
                                     shader->GetBufferSize(), nullptr,
                                     &pixelShader);
      break;
    default:
      die("???");
    }

    if (FAILED(hr)) {
      die("Cannot create shader");
    }
  }

  void *GetBufferPointer() { return shader->GetBufferPointer(); }
  size_t GetBufferSize() { return shader->GetBufferSize(); }

  void bind() {
    switch (type) {
    case SHADER_TYPE::VS:
      context->VSSetShader(vertexShader, nullptr, 0);
      break;
    case SHADER_TYPE::PS:
      context->PSSetShader(pixelShader, nullptr, 0);
      break;
    default:
      die("???");
    }
  }
};

struct MyLayout {
  std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
  ID3D11InputLayout *inputLayout;

public:
  MyLayout(MyShader &shader, std::vector<D3D11_INPUT_ELEMENT_DESC> elements)
      : elements(elements) {

    HRESULT hr = device->CreateInputLayout(
        elements.data(), elements.size(), shader.GetBufferPointer(),
        shader.GetBufferSize(), &inputLayout);

    if (FAILED(hr)) {
      die("Cannot create layout");
    }
  }

  void bind() { context->IASetInputLayout(inputLayout); }
};

template <class V> class MyBuffer {
  ID3D11Buffer *buffer;

public:
  MyBuffer(std::vector<V> elements) {

    D3D11_BUFFER_DESC desc{};
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = sizeof(V) * 3;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iniData{};
    iniData.pSysMem = elements.data();

    device->CreateBuffer(&desc, &iniData, &buffer);

  };

  void bind() {
    UINT stride = sizeof(V);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
  }
};

void render() {
  static MyShader vertexShader{SHADER_TYPE::VS, R"(
    void main(
      float4 position : POSITION,
      float4 color : COLOR,
      out float4 out_pos : SV_POSITION,
      out float4 out_color : COLOR
    ) {
      out_pos = position;
      out_color = color;
    }
  )"};

  static MyShader fragmentShader{SHADER_TYPE::PS, R"(
    void main(
      float4 position : SV_POSITION,
      float4 color : COLOR,
      out float4 out_color : SV_TARGET
    ) {
      out_color = color;
    }
  )"};

  // clang-format off
  static MyLayout layout{vertexShader, {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
  }};
  // clang-format on

  struct Vertex {
    float x, y, z;
    float r, g, b;
  };

  static MyBuffer<Vertex> buffer{{
      {-1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f},
      { 0.0f,  1.0f, 0.5f, 0.0f, 1.0f, 0.0f},
      { 1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f},
  }};

  context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  vertexShader.bind();
  fragmentShader.bind();
  layout.bind();
  buffer.bind();

  context->Draw(3, 0);
}

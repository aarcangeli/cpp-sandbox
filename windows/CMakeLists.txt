add_executable(win-d3d11 win-d3d11.cpp)
add_executable(win-d3d12 win-d3d12.cpp)
add_executable(win-dxr-test win-dxr-test.cpp)
add_executable(win-misc win-misc.cpp)
add_executable(win-virtual-dump win-virtual-dump.cpp)

add_library(namingConvention SHARED naming-convention.cpp)

target_link_libraries(win-d3d11 "D3D11.lib")
target_link_libraries(win-d3d11 "d3dcompiler.lib")

target_link_libraries(win-d3d12 "D3D12.lib")
target_link_libraries(win-d3d12 "DXGI.lib")

target_link_libraries(win-dxr-test "D3D12.lib")
target_link_libraries(win-dxr-test "DXGI.lib")

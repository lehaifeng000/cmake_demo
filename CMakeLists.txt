# CMakeList.txt: cmake_demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "D:/tool/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project ("cmake_demo")

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:/tool/vcpkg/installed/x64-windows")

# 查找 Qt 5 模块
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

set(CMAKE_AUTOMOC ON) # 启用自动 moc 处理 (推荐)

# 将源代码添加到此项目的可执行文件。
add_executable (cmake_demo "cmake_demo.cpp" "cmake_demo.h" "a_star.hpp" )

target_link_libraries(cmake_demo PRIVATE Qt::Widgets Qt::Core Qt::Gui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cmake_demo PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

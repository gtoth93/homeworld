cmake_minimum_required(VERSION 3.28)
project(homeworld)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/Lib/Smacker;${PROJECT_SOURCE_DIR}/Lib/d3d;${PROJECT_SOURCE_DIR}/src/rgl/3dfx")

find_library(BINK NAMES binkw32 REQUIRED)
find_library(DXGUID NAMES dxguid REQUIRED)
find_library(DDRAW NAMES ddraw REQUIRED)
find_library(D3DIM NAMES d3dim REQUIRED)
find_library(GLIDE2X NAMES glide2x REQUIRED)

add_executable(Homeworld WIN32)

add_subdirectory(src/Win32)
add_subdirectory(src/Game)
add_subdirectory(src/Ships)
add_subdirectory(src/rgl)
add_subdirectory(src/JPG)
add_subdirectory(src/Generated)
add_subdirectory(src/Titan/common)
add_subdirectory(src/Titan/crypt)
add_subdirectory(src/Titan/auth)
add_subdirectory(src/Titan/msg)
add_subdirectory(src/Titan/EasySocket)

target_sources(Homeworld PRIVATE ${Win32_sources} ${Generated_sources} ${Game_sources} ${Ships_sources} ${JPG_sources})
target_compile_features(Homeworld PRIVATE cxx_std_23 c_std_17)
set_target_properties(Homeworld PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_definitions(Homeworld PRIVATE $<$<CONFIG:Debug>:HW_Debug> $<$<CONFIG:Release>:HW_Release> HW_English __MSC__=yes)
target_compile_options(Homeworld PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_include_directories(Homeworld PRIVATE src/Win32 src/Generated src/Game src/Ships src/JPG src/Titan src/rgl src/rgl/3dfx src/rgl/D3D)
target_link_libraries(Homeworld
        PRIVATE dsound.lib winmm.lib wsock32.lib ddraw.lib vfw32.lib
        PRIVATE Titan_common Titan_msg Titan_crypt Titan_auth Titan_EasySocket
        PRIVATE ${BINK}
        PRIVATE ${DXGUID})
target_link_options(Homeworld PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

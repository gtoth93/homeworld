add_library(Titan_EasySocket STATIC
        EasySocket.cpp
        EasySocketEngine.cpp
        EasySocketError.cpp
        EasyTitanSocket.cpp
        PipeCmd.cpp
        SocketPipe.cpp
)
target_include_directories(Titan_EasySocket PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_compile_features(Titan_EasySocket PRIVATE cxx_std_23 c_std_17)
set_target_properties(Titan_EasySocket PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(Titan_EasySocket PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(Titan_EasySocket PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

add_library(Titan_common STATIC
        auto_obj.cpp
        CRC16.cpp
        CRC32.cpp
        DataObject.cpp
        EventLog.cpp
        RawList.cpp
        RegKey.cpp
        Threadbase.cpp
        UTF8String.cpp
        Utils.cpp
        WONDebug.cpp
        WONException.cpp
        WONString.cpp
)
target_compile_features(Titan_common PRIVATE cxx_std_23 c_std_17)
set_target_properties(Titan_common PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(Titan_common PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(Titan_common PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

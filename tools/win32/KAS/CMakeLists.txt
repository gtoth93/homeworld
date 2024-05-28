add_executable(KAS2C
        KAS2C.c
        kas2c_tab.c
        lex.yy.c
)

target_compile_features(KAS2C PRIVATE cxx_std_23 c_std_17)
set_target_properties(KAS2C PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(KAS2C PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)

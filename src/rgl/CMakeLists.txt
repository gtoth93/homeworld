add_executable(fxclose
        fxclose.c
)
target_compile_definitions(fxclose PRIVATE __MSC__=yes)
target_include_directories(fxclose PRIVATE 3dfx glide3/include D3D)
target_compile_features(fxclose PRIVATE cxx_std_23 c_std_17)
set_target_properties(fxclose PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(fxclose PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(fxclose PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(fxclose PRIVATE ${GLIDE2X})

add_library(rgl SHARED
        maths.c
        kgl.c
        kvb.c
        clip.c
        asm.c
        invert.c
        hash.c
        rglext.c
        wgl.c
        draw.def
)
target_compile_definitions(rgl PRIVATE __MSC__=yes)
target_include_directories(rgl PRIVATE 3dfx glide3/include D3D)
target_compile_features(rgl PRIVATE cxx_std_23 c_std_17)
set_target_properties(rgl PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(rgl PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(rgl PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(rgl PRIVATE oldnames.lib ws2_32.lib mswsock.lib ddraw.lib)

add_library(glu32 SHARED
        kglu.c
        maths.c
        invert.c
)
target_compile_definitions(glu32 PRIVATE __MSC__=yes)
target_include_directories(glu32 PRIVATE 3dfx glide3/include D3D)
target_compile_features(glu32 PRIVATE cxx_std_23 c_std_17)
set_target_properties(glu32 PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(glu32 PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(glu32 PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(glu32 PRIVATE oldnames.lib ws2_32.lib mswsock.lib ddraw.lib rgl)

add_library(rglsw SHARED
        draw.cpp
        asm.c
        newtriangle.c
        newtextri.c
        newrgbatextri.c
        newblendtextri.c
        newpersptri.c
        newrgbapersptri.c
        lines.c
        scan.c
        span.c
        swdriver.c
        stipple.c
)
target_compile_definitions(rglsw PRIVATE __MSC__=yes)
target_include_directories(rglsw PRIVATE 3dfx glide3/include D3D)
target_compile_features(rglsw PRIVATE cxx_std_23 c_std_17)
set_target_properties(rglsw PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(rglsw PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(rglsw PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(rglsw PRIVATE oldnames.lib ws2_32.lib mswsock.lib ddraw.lib rgl)

add_library(rglfx SHARED
        fxdriver.c
        fxtmg.c
)
target_compile_definitions(rglfx PRIVATE __MSC__=yes)
target_include_directories(rglfx PRIVATE 3dfx glide3/include D3D)
target_compile_features(rglfx PRIVATE cxx_std_23 c_std_17)
set_target_properties(rglfx PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(rglfx PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(rglfx PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(rglfx PRIVATE oldnames.lib ws2_32.lib mswsock.lib ddraw.lib rgl ${GLIDE2X})

add_library(rgld3d SHARED
        d3driver.cpp
        d3dblt.cpp
        d3dtex.cpp
        d3denum.cpp
        d3dinit.cpp
)
target_compile_definitions(rgld3d PRIVATE __MSC__=yes)
target_include_directories(rgld3d PRIVATE 3dfx glide3/include D3D)
target_compile_features(rgld3d PRIVATE cxx_std_23 c_std_17)
set_target_properties(rgld3d PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(rgld3d PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(rgld3d PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)
target_link_libraries(rgld3d PRIVATE oldnames.lib ws2_32.lib mswsock.lib ddraw.lib rgl ${DDRAW} ${DXGUID} ${D3DIM})

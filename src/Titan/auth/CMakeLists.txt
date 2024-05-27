add_library(Titan_auth STATIC
        Auth1Certificate.cpp
        Auth1PrivateKeyBlock.cpp
        Auth1PublicKeyBlock.cpp
        Auth2Certificate.cpp
        AuthCertificateBase.cpp
        AuthFamilyBuffer.cpp
        AuthPublicKeyBlockBase.cpp
        Session.cpp
)
target_include_directories(Titan_auth PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_compile_features(Titan_auth PRIVATE cxx_std_23 c_std_17)
set_target_properties(Titan_auth PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(Titan_auth PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(Titan_auth PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

add_subdirectory(cryptoFiles)

add_library(Titan_crypt STATIC
        BFSymmetricKey.cpp
        BigInteger.cpp
        BigNum.cpp
        CryptKeyBase.cpp
        DiscreteLog.cpp
        EGPrivateKey.cpp
        EGPublicKey.cpp
        IntegerExtractor.cpp
        IntegerInserter.cpp
        MD5Digest.cpp
        MD5Hash.cpp
        MiniBlowfish.cpp
        MiniElGamal.cpp
        Random.cpp
        Randomizer.cpp)
target_sources(Titan_crypt PRIVATE ${cryptoFiles_Sources})
target_include_directories(Titan_crypt PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_compile_features(Titan_crypt PRIVATE cxx_std_23 c_std_17)
set_target_properties(Titan_crypt PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(Titan_crypt PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(Titan_crypt PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

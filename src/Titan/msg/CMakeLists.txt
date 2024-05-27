add_subdirectory(Auth)
add_subdirectory(Comm)
add_subdirectory(Contest)
add_subdirectory(Dir)
add_subdirectory(Event)
add_subdirectory(Fact)
add_subdirectory(Firewall)
add_subdirectory(Obs)
add_subdirectory(Profile)
add_subdirectory(Routing)

add_library(Titan_msg STATIC
        BadMsgException.cpp
        TMessage.cpp
)
target_sources(Titan_msg
        PRIVATE ${Auth_sources} ${Comm_sources} ${Contest_sources} ${Dir_sources} ${Event_sources} ${Fact_sources}
        PRIVATE ${Firewall_sources} ${Obs_sources} ${Profile_sources} ${Routing_sources})
target_include_directories(Titan_msg PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_compile_features(Titan_msg PRIVATE cxx_std_23 c_std_17)
set_target_properties(Titan_msg PROPERTIES CXX_EXTENSIONS OFF C_EXTENSIONS OFF MSVC_RUNTIME_LIBRARY MultiThreaded)
target_compile_options(Titan_msg PRIVATE /Wall /permissive- /arch:AVX2 $<$<CONFIG:Release>:/GL> $<$<CONFIG:Release>:/Gw> /MP)
target_link_options(Titan_msg PRIVATE $<$<CONFIG:Release>:/LTCG> $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF> /VERBOSE)

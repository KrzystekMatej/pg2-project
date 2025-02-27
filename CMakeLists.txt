cmake_minimum_required(VERSION 3.8)

project(RayTracer)

if (WIN32)
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
            "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>, 
            $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>, 
            $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

add_subdirectory(Source)
cmake_minimum_required(VERSION 3.8)

project(RayTracer)

include(FetchContent)

if (WIN32)
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
            "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>, 
            $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>, 
            $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

set(OIDN_INSTALL_DIR "${CMAKE_SOURCE_DIR}/External/oidn")

if (NOT EXISTS "${OIDN_INSTALL_DIR}/lib/OpenImageDenoise.lib")
    set(OIDN_VERSION "2.3.2")
    set(OIDN_FILENAME "oidn-${OIDN_VERSION}.x64.windows.zip")
    set(OIDN_URL "https://github.com/OpenImageDenoise/oidn/releases/download/v${OIDN_VERSION}/${OIDN_FILENAME}")
    set(OIDN_ZIP_PATH "${CMAKE_SOURCE_DIR}/External/${OIDN_FILENAME}")
    set(OIDN_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/External/oidn_tmp")

    if (NOT EXISTS ${OIDN_ZIP_PATH})
        message(STATUS "Downloading OIDN from ${OIDN_URL}")
        file(DOWNLOAD ${OIDN_URL} ${OIDN_ZIP_PATH} SHOW_PROGRESS)
    endif()

    file(ARCHIVE_EXTRACT INPUT ${OIDN_ZIP_PATH} DESTINATION ${OIDN_EXTRACT_DIR})

    file(GLOB OIDN_EXTRACTED_DIR "${OIDN_EXTRACT_DIR}/oidn-${OIDN_VERSION}.x64.windows")

    if (EXISTS "${OIDN_EXTRACTED_DIR}")
        file(RENAME "${OIDN_EXTRACTED_DIR}" "${OIDN_INSTALL_DIR}")
    else()
        message(FATAL_ERROR "OIDN extraction failed! Expected folder not found: ${OIDN_EXTRACTED_DIR}")
    endif()

    file(REMOVE_RECURSE ${OIDN_EXTRACT_DIR})

    file(REMOVE ${OIDN_ZIP_PATH})

    message(STATUS "OIDN successfully extracted to ${OIDN_INSTALL_DIR}")
endif()

set(OIDN_INCLUDE_DIR "${OIDN_INSTALL_DIR}/include")
set(OIDN_LIB_DIR "${OIDN_INSTALL_DIR}/lib")

include_directories(${OIDN_INCLUDE_DIR})
link_directories(${OIDN_LIB_DIR})

add_subdirectory(Source)
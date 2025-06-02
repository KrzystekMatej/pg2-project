file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/Include/*.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(RayTracer ${SOURCES} ${HEADERS})

target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/Include)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC "${CMAKE_SOURCE_DIR}/External/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/External/glad/include")

find_package(freeimage CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

if (WIN32)
    set(OIDN_LIB_NAME "${CMAKE_SOURCE_DIR}/External/oidn/lib/OpenImageDenoise.lib")
else()
    # Linux verzi pøidáme pozdìji
    set(OIDN_LIB_NAME "")
endif()

target_link_libraries(RayTracer PRIVATE 
    glfw
    OpenGL::GL
    glad
    freeimage::FreeImage 
    freeimage::FreeImagePlus 
    glm::glm-header-only
    assimp::assimp
    ${OIDN_LIB_NAME}
    spdlog::spdlog
    EnTT::EnTT
    yaml-cpp::yaml-cpp
    tinyobjloader::tinyobjloader
)

if (WIN32)
    file(GLOB ASSIMP_DLLS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*.dll")
    foreach(DLL ${ASSIMP_DLLS})
        add_custom_command(TARGET RayTracer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
    endforeach()
endif()
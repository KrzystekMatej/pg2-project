set(SOURCES
    Main.cpp
)

set(HEADERS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(RayTracer ${SOURCES} ${HEADERS})

target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/Include)

# ?? Najdi a pøidej knihovny
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(freeimage CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# ?? Pøidání OIDN
if (WIN32)
    set(OIDN_LIB_NAME "${CMAKE_SOURCE_DIR}/External/oidn/lib/OpenImageDenoise.lib")
else()
    # Linux verzi pøidáme pozdìji
    set(OIDN_LIB_NAME "")
endif()

# ?? Pøipojení OIDN statické knihovny
target_link_libraries(RayTracer PRIVATE 
    glfw 
    glad::glad 
    OpenGL::GL 
    freeimage::FreeImage 
    freeimage::FreeImagePlus 
    glm::glm-header-only
    ${OIDN_LIB_NAME}
)
cmake_minimum_required(VERSION 3.16)
if (MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include)
include_directories(${PROJECT_NAME}-static ${CMAKE_SOURCE_DIR}/include)
include_directories(test ${CMAKE_SOURCE_DIR}/include)

set(SFML_DIR C:/Code_Libraries/SFML-2.5.1/VS2019-64bit/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS system graphics window REQUIRED)

set(BOX2D_DIR C:/Code_Libraries/box2d/box2d-2.4.1/build/bin/Release)

set(SRC_FILES
    lic.cpp
    Engine.cpp
    IOBus.cpp
    cast.cpp
    Color.cpp
    VideoSettings.cpp
    global.cpp
    Velocity.cpp
    Scale.cpp
    Position.cpp
    Entity.cpp
    default_functions.cpp
    PCNode.cpp
    Range.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_NAME}-static STATIC ${SRC_FILES})

set(SFML_LIBS
    sfml-graphics
    sfml-window
    sfml-system
)

set(BOX2D_LIB ${BOX2D_DIR}/box2d.lib)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} ${BOX2D_LIB})
target_link_libraries(${PROJECT_NAME}-static ${SFML_LIBS} ${BOX2D_LIB})

add_executable(test
    main.cpp
)

target_link_libraries(test ${PROJECT_NAME}-static ${SFML_LIBS})
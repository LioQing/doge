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
find_package(SFML 2.5 COMPONENTS system graphics window audio REQUIRED)

set(BOX2D_DIR C:/Code_Libraries/box2d/box2d-2.4.1/build/bin/Debug)

set(SRC_FILES
    utils/lic.cpp
    core/Engine.cpp
    core/io/Window.cpp
    utils/cast.cpp
    utils/Color.cpp
    core/Window.cpp
    utils/global.cpp
    components/Scale.cpp
    components/Position.cpp
    core/Entity.cpp
    utils/default_functions.cpp
    core/PCNode.cpp
    core/Range.cpp
    extensions/physics/physics.cpp
    utils/math.cpp
    core/Texture.cpp
    core/Assets.cpp
    core/io/Image.cpp
    core/io/Texture.cpp
    core/io/Keyboard.cpp
    core/io/Mouse.cpp
    core/io/Controller.cpp
    core/io/Touch.cpp
    core/io/Sensor.cpp
    core/io/Event.cpp
    core/Events.cpp
    extensions/physics/PhysicsBody.cpp
    extensions/physics/PhysicsCollider.cpp
    core/io/SoundBuffer.cpp
    core/io/Sound.cpp
    core/io/Music.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_NAME}-static STATIC ${SRC_FILES})

set(SFML_LIBS
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

set(BOX2D_LIB ${BOX2D_DIR}/box2d.lib)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} ${BOX2D_LIB})
target_link_libraries(${PROJECT_NAME}-static ${SFML_LIBS} ${BOX2D_LIB})

add_executable(test
    main.cpp
)

target_link_libraries(test ${PROJECT_NAME}-static ${SFML_LIBS})

add_custom_command(TARGET test PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:test>/assets)
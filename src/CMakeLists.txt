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

if (MSVC)

    set(SFML_DIR C:/Code_Libraries/SFML-2.5.1/VS2019-64bit/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML 2.5 COMPONENTS system graphics window audio REQUIRED)

    set(BOX2D_DIR C:/Code_Libraries/box2d-2.4.1/VS2019-64bit/bin/Debug)

else()

    set(BOX2D_DIR /mnt/c/Code_Libraries/box2d-2.4.1/GCC11/bin)

endif()

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
    core/PCNode.cpp
    core/Range.cpp
    utils/math.cpp
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
    core/io/SoundBuffer.cpp
    core/io/Sound.cpp
    core/io/Music.cpp
    core/io/Cursor.cpp
    core/io/Font.cpp
    utils/custom_sf/Text.cpp
    utils/custom_sf/DrawableVertices.cpp
    components/Sprite.cpp

    # physics
    extensions/physics/Physics.cpp
    extensions/physics/Body.cpp
    extensions/physics/Collider.cpp

    # gui
    extensions/gui/GUI.cpp
    extensions/gui/Element.cpp
    extensions/gui/Button.cpp
    extensions/gui/Window.cpp
    extensions/gui/WindowEx.cpp
    extensions/gui/Text.cpp
    extensions/gui/Image.cpp
    extensions/gui/NSImage.cpp
    extensions/gui/Draggable.cpp
    extensions/gui/Clickable.cpp
    extensions/gui/Resizable.cpp
    extensions/gui/Camera.cpp

    # nine_slice
    extensions/nine_slice/NineSlice.cpp
    extensions/nine_slice/Texture.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_NAME}-static STATIC ${SRC_FILES})


if (MSVC)

    set(SFML_LIBS
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
    )

    set(BOX2D_LIB ${BOX2D_DIR}/box2d.lib)

else()

    set(SFML_LIBS
        -lsfml-system
        -lsfml-graphics
        -lsfml-window
        -lsfml-audio
    )

    set(BOX2D_LIB ${BOX2D_DIR}/libbox2d.a)

    set(CMAKE_CXX_FLAGS "-O3")

    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

endif()

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} ${BOX2D_LIB})
target_link_libraries(${PROJECT_NAME}-static ${SFML_LIBS} ${BOX2D_LIB})

add_executable(test
    main.cpp
)

target_link_libraries(test ${PROJECT_NAME}-static)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
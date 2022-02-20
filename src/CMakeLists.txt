cmake_minimum_required(VERSION 3.16)
if (MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include)
include_directories(test ${CMAKE_SOURCE_DIR}/include)

if (MSVC)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    set(SFML_DIR C:/Code_Libraries/SFML-2.5.1/VC++15-64bit/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML 2.5 COMPONENTS system graphics window audio REQUIRED)

    include_directories(${PROJECT_NAME} C:/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)
    include_directories(${PROJECT_NAME}-shared C:/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)
    include_directories(test C:/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)

    set(BOX2D_DIR C:/Code_Libraries/box2d-2.4.1/VC++16-64bit/bin/Debug)

else()

    include_directories(${PROJECT_NAME} /mnt/c/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)
    include_directories(${PROJECT_NAME}-shared /mnt/c/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)
    include_directories(test /mnt/c/Code_Libraries/box2d-2.4.1/box2d-2.4.1/include)

    set(BOX2D_DIR /mnt/c/Code_Libraries/box2d-2.4.1/GCC11/bin/Debug)

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
    components/CustomShape.cpp
    utils/polypartition/polypartition.cpp
    utils/triangulation.cpp

    # physics
    extensions/physics/Physics.cpp
    extensions/physics/Body.cpp
    extensions/physics/Collider.cpp
    extensions/physics/CollisionEvents.cpp

    # nine_slice
    extensions/nine_slice/NineSlice.cpp
    extensions/nine_slice/Texture.cpp
    extensions/nine_slice/Sprite.cpp

    # anim
    extensions/anim/Anim.cpp
)

add_library(${PROJECT_NAME}-shared SHARED ${SRC_FILES})
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

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

endif()

target_link_libraries(${PROJECT_NAME}-shared ${SFML_LIBS} ${BOX2D_LIB})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} ${BOX2D_LIB})

add_executable(test
    main.cpp
)

target_link_libraries(test ${PROJECT_NAME})
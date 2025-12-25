# ============================================
# МИНИМАЛЬНЫЙ CMakeLists.txt для SFML маятника
# ============================================


cmake_minimum_required(VERSION 3.16)

project(PendulumSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MINGW)
    add_compile_options(-Wall -Wextra)
endif()


set(SFML_PATH "C:\\Degree_1\\SFML_lib\\SFML-3.0.2")


include_directories(${SFML_PATH}/include)

# Ищем библиотеки SFML
find_library(SFML_GRAPHICS_LIB 
    NAMES sfml-graphics-3 sfml-graphics
    PATHS ${SFML_PATH}/lib
    REQUIRED
)

find_library(SFML_WINDOW_LIB
    NAMES sfml-window-3 sfml-window
    PATHS ${SFML_PATH}/lib
    REQUIRED
)

find_library(SFML_SYSTEM_LIB
    NAMES sfml-system-3 sfml-system
    PATHS ${SFML_PATH}/lib
    REQUIRED
)

# Указываем исходные файлы проекта
add_executable(pendulum
    main.cpp
    physics_engine.cpp
)

target_link_libraries(pendulum
    ${SFML_GRAPHICS_LIB}
    ${SFML_WINDOW_LIB}
    ${SFML_SYSTEM_LIB}
)

# ============================================
# 8. КОПИРОВАНИЕ DLL
# ============================================

if(WIN32)
    add_custom_command(TARGET pendulum POST_BUILD
        COMMENT "Копируем DLL файлы SFML..."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_PATH}/bin/sfml-graphics-3.dll"
            "$<TARGET_FILE_DIR:pendulum>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_PATH}/bin/sfml-window-3.dll"
            "$<TARGET_FILE_DIR:pendulum>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_PATH}/bin/sfml-system-3.dll"
            "$<TARGET_FILE_DIR:pendulum>"
        VERBATIM
    )
endif()
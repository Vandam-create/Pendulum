# Универсальный CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(PendulumSFML)

set(CMAKE_CXX_STANDARD 17)

# Автоматический поиск SFML
set(SFML_PATH "C:/Degree_1/SFML_lib/SFML-3.0.2")

# Функция для поиска библиотек
function(find_sfml_component COMPONENT OUTPUT_VAR)
    set(DEBUG_NAMES
        "sfml-${COMPONENT}-d"
        "sfml-${COMPONENT}-d-3"
        "sfml-${COMPONENT}-3-d"
    )
    set(RELEASE_NAMES
        "sfml-${COMPONENT}"
        "sfml-${COMPONENT}-3"
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(NAMES ${DEBUG_NAMES})
    else()
        set(NAMES ${RELEASE_NAMES})
    endif()
    
    foreach(NAME ${NAMES})
        set(LIB_PATH "${SFML_PATH}/lib/${NAME}.lib")
        if(EXISTS "${LIB_PATH}")
            set(${OUTPUT_VAR} "${LIB_PATH}" PARENT_SCOPE)
            message(STATUS "Найдена ${COMPONENT}: ${LIB_PATH}")
            return()
        endif()
    endforeach()
    
    message(FATAL_ERROR "Библиотека ${COMPONENT} не найдена!")
endfunction()

# Ищем компоненты
find_sfml_component(graphics SFML_GRAPHICS_LIB)
find_sfml_component(window SFML_WINDOW_LIB)
find_sfml_component(system SFML_SYSTEM_LIB)

# Исполняемый файл
add_executable(pendulum
    src/main.cpp
    src/physics_engine.cpp
)

# Подключаем
target_include_directories(pendulum PRIVATE "${SFML_PATH}/include")
target_link_libraries(pendulum
    ${SFML_GRAPHICS_LIB}
    ${SFML_WINDOW_LIB}
    ${SFML_SYSTEM_LIB}
    "${SFML_PATH}/lib/freetype.lib"
)

# Автокопирование DLL
if(WIN32)
    # Функция для копирования DLL
    function(copy_sfml_dll COMPONENT)
        set(DEBUG_DLLS
            "sfml-${COMPONENT}-d.dll"
            "sfml-${COMPONENT}-d-3.dll"
        )
        set(RELEASE_DLLS
            "sfml-${COMPONENT}.dll"
            "sfml-${COMPONENT}-3.dll"
        )
        
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(DLL_NAMES ${DEBUG_DLLS})
        else()
            set(DLL_NAMES ${RELEASE_DLLS})
        endif()
        
        foreach(DLL_NAME ${DLL_NAMES})
            set(DLL_PATH "${SFML_PATH}/bin/${DLL_NAME}")
            if(EXISTS "${DLL_PATH}")
                add_custom_command(TARGET pendulum POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${DLL_PATH}"
                        "$<TARGET_FILE_DIR:pendulum>"
                    VERBATIM
                )
                message(STATUS "Будет скопирован: ${DLL_NAME}")
                return()
            endif()
        endforeach()
        
        message(WARNING "DLL для ${COMPONENT} не найден")
    endfunction()
    
    copy_sfml_dll(graphics)
    copy_sfml_dll(window)
    copy_sfml_dll(system)
    
    # Копируем зависимости
    if(EXISTS "${SFML_PATH}/bin/freetype.dll")
        add_custom_command(TARGET pendulum POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_PATH}/bin/freetype.dll"
                "$<TARGET_FILE_DIR:pendulum>"
        )
    endif()
endif()
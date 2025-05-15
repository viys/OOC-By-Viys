cmake_minimum_required(VERSION 3.15)

set(KCONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MERGE_CONFIG "${CMAKE_SOURCE_DIR}/scripts/kconfig/merge_config.py")
set(KCONFIG_CONFIG_H "my_config.h")

set (PROJECT_NAME "tamplate")
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/project/app/main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -O2
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

target_link_options(${PROJECT_NAME} PRIVATE
)

# 添加一个自定义目标来运行 menuconfig
if(CMAKE_HOST_WIN32)
  # Windows
  add_custom_target(
        menuconfig
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            menuconfig.exe ${KCONFIG_DIR}/Kconfig
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            python ${MERGE_CONFIG} --handwritten-input-configs
            ${KCONFIG_DIR}/Kconfig
            .config
            ${CMAKE_BINARY_DIR}/${KCONFIG_CONFIG_H}
            ${CMAKE_BINARY_DIR}/kconfig_files.txt
            ${CMAKE_BINARY_DIR}/.config
            ${CMAKE_CURRENT_SOURCE_DIR}/prj.config
        COMMENT "Running menuconfig"
    )
elseif(CMAKE_HOST_APPLE)
    # macOS
    add_custom_target(
        menuconfig
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            menuconfig.exe ${KCONFIG_DIR}/Kconfig
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            python3 ${MERGE_CONFIG} --handwritten-input-configs
            ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig
            .config
            ${CMAKE_BINARY_DIR}/${KCONFIG_CONFIG_H}
            ${CMAKE_BINARY_DIR}/kconfig_files.txt
            ${CMAKE_BINARY_DIR}/.config
            ${CMAKE_CURRENT_SOURCE_DIR}/prj.config
        COMMENT "Running menuconfig"
    )
elseif(CMAKE_HOST_UNIX)
    # UNIX
    add_custom_target(
        menuconfig
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            kconfig-mconf ${KCONFIG_DIR}/Kconfig
        COMMAND ${CMAKE_COMMAND} -E env srctree=${CMAKE_SOURCE_DIR}
            python3 ${MERGE_CONFIG} --handwritten-input-configs
            ${KCONFIG_DIR}/Kconfig
            .config
            ${CMAKE_BINARY_DIR}/${KCONFIG_CONFIG_H}
            ${CMAKE_BINARY_DIR}/kconfig_files.txt
            ${CMAKE_BINARY_DIR}/.config
            ${CMAKE_CURRENT_SOURCE_DIR}/prj.config
        COMMENT "Running menuconfig"
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND size.exe --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
    COMMAND size.exe --format=berkeley $<TARGET_FILE:${PROJECT_NAME}> > ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.size
)

cmake_minimum_required(VERSION 3.14)
set(ARM_CPU ${TARGET_CPU})
string(REGEX REPLACE "^cortex-m([0-9]+)$" "ARMCM\\1" CMSIS_CPU ${ARM_CPU})
if(${ARM_CPU} STREQUAL "cortex-m33")
    set(ARM_FEATURES "_DSP_FP")
elseif(${ARM_CPU} STREQUAL "cortex-m4")
    set(ARM_FEATURES "_FP")
elseif(${ARM_CPU} STREQUAL "cortex-m7")
    set(ARM_FEATURES "_DP")
else()
    set(ARM_FEATURES "")
endif()
add_library(cmsis_startup STATIC)
target_sources(cmsis_startup PRIVATE
    ${CMSIS_PATH}/Device/ARM/${CMSIS_CPU}/Source/startup_${CMSIS_CPU}.c
    ${CMSIS_PATH}/Device/ARM/${CMSIS_CPU}/Source/system_${CMSIS_CPU}.c)
target_include_directories(cmsis_startup PUBLIC
    ${CMSIS_PATH}/Device/ARM/${CMSIS_CPU}/Include
    ${CMSIS_PATH}/CMSIS/Core/Include)
target_compile_options(cmsis_startup INTERFACE -include${CMSIS_CPU}${ARM_FEATURES}.h)
target_compile_definitions(cmsis_startup PRIVATE ${CMSIS_CPU}${ARM_FEATURES})


set(ethos_dir "${CMAKE_CURRENT_SOURCE_DIR}/ethos_test")
set(nn_dir "${CMAKE_CURRENT_SOURCE_DIR}/nn_test")

add_executable(main_nn main.c)
add_executable(main_ethos main.c)


# Compile for cmsis-nn test
add_library(retarget_nn STATIC
"${nn_dir}/retarget.c"
"${nn_dir}/uart.c"
)
target_include_directories(retarget_nn PUBLIC ${nn_dir})

# Linker file settings.
set(LINK_FILE "${nn_dir}/linker.ld")
set(LINK_FILE_OPTION "-T")
set(LINK_ENTRY_OPTION "")
set(LINK_ENTRY "")

# Link in FVP dependencies to every unit test.
target_link_libraries(main_nn  retarget_nn)
target_link_libraries(main_nn $<TARGET_OBJECTS:cmsis_startup> cmsis_startup)

add_dependencies(main_nn retarget_nn cmsis_startup)

target_link_options(main_nn PRIVATE ${LINK_FILE_OPTION} ${LINK_FILE} ${LINK_ENTRY_OPTION} ${LINK_ENTRY})
set_target_properties(main_nn PROPERTIES LINK_DEPENDS ${LINK_FILE})

# Compile for ethos test

add_library(retarget_ethos STATIC
"${ethos_dir}/retarget.c"
"${ethos_dir}/uart.c"
)
target_include_directories(retarget_ethos PUBLIC ${ethos_dir})

# Linker file settings.
set(LINK_FILE "${ethos_dir}/linker.ld")
set(LINK_FILE_OPTION "-T")
set(LINK_ENTRY_OPTION "")
set(LINK_ENTRY "")

# Link in FVP dependencies to every unit test.
target_link_libraries(main_ethos  retarget_ethos)
target_link_libraries(main_ethos $<TARGET_OBJECTS:cmsis_startup> cmsis_startup)

add_dependencies(main_ethos retarget_ethos cmsis_startup)

target_link_options(main_ethos PRIVATE ${LINK_FILE_OPTION} ${LINK_FILE} ${LINK_ENTRY_OPTION} ${LINK_ENTRY})
set_target_properties(main_ethos PROPERTIES LINK_DEPENDS ${LINK_FILE})

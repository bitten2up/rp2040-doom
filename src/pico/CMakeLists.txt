add_library(common_pico INTERFACE)
target_sources(common_pico INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/i_glob.c
        ${CMAKE_CURRENT_LIST_DIR}/i_input.c
        ${CMAKE_CURRENT_LIST_DIR}/i_system.c
        ${CMAKE_CURRENT_LIST_DIR}/piconet.c
        ${CMAKE_CURRENT_LIST_DIR}/i_timer.c
        ${CMAKE_CURRENT_LIST_DIR}/i_video.c
        ${CMAKE_CURRENT_LIST_DIR}/stubs.c

        ${CMAKE_CURRENT_LIST_DIR}/i_picosound.c
)
if (PICO_ON_DEVICE)
    target_sources(common_pico INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/blit.S
            ${CMAKE_CURRENT_LIST_DIR}/picoflash.c
            ${CMAKE_CURRENT_LIST_DIR}/mipi/mipi_display.c
            )
    target_include_directories(common_pico INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/mipi/include
            )
    pico_wrap_function(common_pico malloc)
    pico_wrap_function(common_pico calloc)
    pico_wrap_function(common_pico free)
    target_compile_definitions(common_pico INTERFACE
            PICO_HEAP_SIZE=0
            USE_ZONE_FOR_MALLOC=1
    )
    target_link_libraries(common_pico INTERFACE hardware_i2c)
endif()
target_include_directories(common_pico INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${CMAKE_CURRENT_LIST_DIR}/../../textscreen
        )

target_compile_definitions(common_pico INTERFACE
        PICO_AUDIO_I2S_PIO=1
        PICO_AUDIO_I2S_DMA_IRQ=1
        HAGL_HAL_DEBUG=1
        HAGL_HAL_USE_DMA=1
        )

target_link_libraries(common_pico INTERFACE pico_stdlib pico_multicore hardware_spi hardware_dma)

add_library(pico_cd INTERFACE)
if (TARGET tinyusb_host)
    target_link_libraries(pico_cd INTERFACE tinyusb_host)
endif()

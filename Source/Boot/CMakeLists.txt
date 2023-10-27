file(GLOB BOOT_SOURCES *.c)

add_executable(efimain.efi ${BOOT_SOURCES})

set_target_properties(efimain.efi PROPERTIES LINK_FLAGS "-nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -Wl,--image-base -Wl,0x10000000 -e efi_main")

target_include_directories(efimain.efi PRIVATE
    ${CMAKE_SOURCE_DIR}/Source
)

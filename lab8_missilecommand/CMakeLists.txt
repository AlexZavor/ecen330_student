add_executable(lab8_m1.elf main_m1.c gameControl.c missile.c plane.c)
target_link_libraries(lab8_m1.elf ${330_LIBS} intervalTimer interrupts touchscreen)
set_target_properties(lab8_m1.elf PROPERTIES LINKER_LANGUAGE CXX)

add_executable(lab8_m2.elf main_m2.c gameControl.c missile.c plane.c)
target_link_libraries(lab8_m2.elf ${330_LIBS} intervalTimer buttons_switches interrupts touchscreen)
set_target_properties(lab8_m2.elf PROPERTIES LINKER_LANGUAGE CXX)

add_executable(lab8_m3.elf main_m3.c gameControl.c missile.c plane.c)
target_link_libraries(lab8_m3.elf ${330_LIBS} intervalTimer buttons_switches interrupts touchscreen)
set_target_properties(lab8_m3.elf PROPERTIES LINKER_LANGUAGE CXX)

target_compile_definitions(lab8_m3.elf PUBLIC LAB8_M3)
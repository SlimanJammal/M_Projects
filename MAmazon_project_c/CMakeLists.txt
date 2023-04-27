cmake_minimum_required(VERSION 3.15)
project(hw1 C)

set(CMAKE_C_STANDARD 99)

add_executable(hw1 matamazom.c products.c products.h orders.c orders.h
        matamazom_print.c matamazom_print.h matamazom.h matamazom.c amount_set.c amount_set.h matamazom_tests.h
        matamazom_tests.c test_matamazom_yonatan_and_hen.c )

target_link_libraries(hw1 "C:/Users/slima/Documents/mtam 234124/exs/hw1/libmtm.a")
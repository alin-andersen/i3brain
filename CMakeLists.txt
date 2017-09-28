cmake_minimum_required(VERSION 3.7)
project("i3brain")

set(CMAKE_CC_COMPILER gcc)

include_directories(${CMAKE_SOURCE_DIR})

set(I3BRAIN_SOURCE_FILES

  global.c

  battery.c
  cpu.c
  io.c
  ram.c
  time2.c

  i3brain.c

)

add_executable(i3brain ${I3BRAIN_SOURCE_FILES})

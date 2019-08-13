cmake_minimum_required (VERSION 2.6)
project (iogen)
add_executable(iogen main.cpp parseargs.cpp utils.cpp parser.cpp generator.cpp)
install (TARGETS iogen DESTINATION /usr/bin)

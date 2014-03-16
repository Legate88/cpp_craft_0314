cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set( CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "Configurations" FORCE )

project( ${SOLUTION_NAME} )

include( environment.cmake required )


binaries(lab1_1 lab1_2 lab1_3)


compile()

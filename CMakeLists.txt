cmake_minimum_required(VERSION 3.10)
project(Button)

# Create a static library
add_library(Button STATIC
    src/button.c
    src/io.c
)

# Specify the include directory for the library
target_include_directories(Button PUBLIC
    include
)
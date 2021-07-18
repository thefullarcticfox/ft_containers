cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(ft_containers)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_compile_options(-Wall -Wextra -Werror)

add_executable(ft_containers
        adapted_traits.hpp
        algorithm.hpp
        allocator.hpp
        functional.hpp
        list.hpp
        main.cpp
        map.hpp
        priority_queue.hpp
        queue.hpp
        reverse_iterator.hpp
        stack.hpp
        vector.hpp)

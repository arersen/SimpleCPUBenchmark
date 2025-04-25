cmake_minimum_required(VERSION 3.10)

project(CPU_Benchmark)


set(CMAKE_C_STANDARD 99)
add_library(network_lib SHARED
        network/network.c
        network/network.h
        network/server.c
        network/server.h
        network/client.c
        network/client.h
)

add_executable(cpu_benchmark main.c
        cpu_info/cpu_info.h
        cpu_info/cpu_info.c
        benchmark/benchmark.c
        benchmark/benchmark.h
        results/results.c
        results/results.h
)
target_link_libraries(cpu_benchmark m network_lib)

add_executable(server server.c)
target_link_libraries(server m network_lib)

add_executable(client client.c)
target_link_libraries(client m network_lib)




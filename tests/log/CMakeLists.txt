macro(l name)
add_executable(${name} ${name}.cpp)
target_link_libraries(${name} PRIVATE leptlog)
endmacro()

l(log_basic)
l(log_advance)

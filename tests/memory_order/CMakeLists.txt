macro(t name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE bench)
endmacro()

t(introduction)


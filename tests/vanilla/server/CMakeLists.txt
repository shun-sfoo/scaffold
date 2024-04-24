macro(s name)
  add_executable(${name} ${name}.cpp util.cpp)
  target_link_libraries(${name} PRIVATE leptlog)
endmacro()

s(server)
s(client)

add_library(actor INTERFACE)
target_include_directories(actor INTERFACE include)

add_executable(atm atm.cpp)
target_link_libraries(atm PRIVATE actor)


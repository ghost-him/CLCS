include_directories(../CLCS-server)
include_directories(../public)

aux_source_directory(Epoll_Reactor_Module modules)
aux_source_directory(Initializer_Module modules)
aux_source_directory(Group_Control_Module modules)

add_executable(CLCS-server ${modules} main.cc)

target_link_libraries(CLCS-server public)
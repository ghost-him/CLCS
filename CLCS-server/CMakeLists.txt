include_directories(include)
include_directories(../public/include)

aux_source_directory(include includes)
aux_source_directory(resource resources)

add_executable(CLCS-server ${includes} ${resources})

target_link_libraries(CLCS-server public)
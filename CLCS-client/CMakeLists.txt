include_directories(../CLCS-client)
include_directories(../public)

aux_source_directory(Message_Process_Module modules)
aux_source_directory(Initializer_Module modules)
aux_source_directory(Business_Module modules)
aux_source_directory(Server_Connector_Module modules)
aux_source_directory(Request_Management_Module modules)

add_executable(CLCS-client ${modules} main.cc)

target_link_libraries(CLCS-client public)
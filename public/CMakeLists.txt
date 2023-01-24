include_directories(../public)

aux_source_directory(Command_Module modules)
aux_source_directory(File_Manager_Module modules)
aux_source_directory(Global_Variables modules)
aux_source_directory(Initializer_Module modules)
aux_source_directory(Language_Module modules)
aux_source_directory(Logger_Module modules)
aux_source_directory(Message_IO_Module modules)
aux_source_directory(Message_Module modules)
aux_source_directory(Security_Module modules)
aux_source_directory(ThreadPool_Module modules)
aux_source_directory(User_Manager_Module modules)
aux_source_directory(Command_Line_Parameter_Analysis_Module modules)
aux_source_directory(Functional_Auxiliary_Module modules)
aux_source_directory(Functional_Auxiliary_Module/More_Ctrl_C modules)

set(lib_public_var ${LIBRARY_OUTPUT_PATH} CACHE INTERNAL "公共代码")

add_library(public STATIC ${modules})



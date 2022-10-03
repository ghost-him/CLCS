include_directories(include)
aux_source_directory(include includes)
aux_source_directory(resource resources)

set(lib_public_var ${LIBRARY_OUTPUT_PATH} CACHE INTERNAL "公共代码")

add_library(public STATIC ${includes} ${resources})



aux_source_directory(. DIR_COMMON)
include_directories(../../include)
add_compile_options(-O2)
add_library(common_obj ${DIR_COMMON})
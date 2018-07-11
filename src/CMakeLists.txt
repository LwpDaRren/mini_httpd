PROJECT(mini_httpd_project)

add_subdirectory(common)
add_subdirectory(core)
include_directories(../include)
aux_source_directory(. DIR_SRC)

add_executable(mini_httpd ${DIR_SRC})
target_link_libraries(mini_httpd core_obj common_obj pthread)
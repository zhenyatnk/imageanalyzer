cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(_project_name threadpoolex)

#
# Parameters repository
#
set(${_project_name}_repo "https://github.com/zhenyatnk/threadpoolex.git")
#set(${_project_name}_tag "v0.1.1")
set(${_project_name}_tag "master")

set(${_project_name}_main_dir ${CMAKE_CURRENT_BINARY_DIR})

set(${_project_name}_source_dir   ${_project_name}_src)
set(${_project_name}_build_dir    ${_project_name}_build)
set(${_project_name}_download_dir ${_project_name}_download)

# Configure package
configure_file(CMakeLists.txt.in
               ${${_project_name}_download_dir}/CMakeLists.txt)

#
# Download package
#
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${${_project_name}_main_dir}/${${_project_name}_download_dir} )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${${_project_name}_main_dir}/${${_project_name}_download_dir} )

 # Package also has to use one.

add_subdirectory(${${_project_name}_main_dir}/${${_project_name}_source_dir}
                 ${${_project_name}_main_dir}/${${_project_name}_build_dir})

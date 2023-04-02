# 添加头文件的查找目录 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include) 
## 生成各种目标(target)文件: 可执行文件、动态库、静态库 
# 指定可执行文件的输出目录，输出到bin下面 
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
# 指定可执行文件名(hello)和相关源文件  
ADD_EXECUTABLE(mysever main.cpp ) 
target_link_libraries(mysever pthread)

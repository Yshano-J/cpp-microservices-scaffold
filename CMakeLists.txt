# 最低版本要求
cmake_minimum_required(VERSION 3.1.3)
# 项目名称
project(aries_scaffold VERSION 1.0)
# 生成库文件
file(GLOB_RECURSE BASE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc) 
add_library(${PROJECT_NAME} STATIC ${BASE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# 设置库文件属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib # 库文件输出目录
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION} # 库版本号
    SOVERSION ${PROJECT_VERSION_MAJOR} # 库主版本号
)
# 依赖组件
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Finddepends)

find_my_depends()
# 安装头文件
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)
# 安装库文件
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib # 库文件输出目录
    INCLUDES DESTINATION include/${PROJECT_NAME} # 头文件输出目录
)
# 子模块安装
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/
    DESTINATION lib/cmake/${PROJECT_NAME}/modules
    FILES_MATCHING PATTERN "Find*.cmake"
)
# 设置导出Targets文件
install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
)
# 配置文件
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)
# ⽣成版本配置⽂件，这个其实可有可⽆
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
# 安装配置⽂件
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)
# 设置需要依赖的库名称
set(libs jsoncpp protobuf amqpcpp ev brpc dl ssl crypto gflags gtest pthread cpr fmt elasticlient cpprest etcd-cpp-api 
         fdfsclient fastcommon avcodec avformat avutil curl odb-mysql odb odb-boost redis++ hiredis spdlog boost_system)
# 先通过find_library看看能不能找到依赖库，找到了就设置⼀下库信息
macro(find_libraries lib)
        find_library(${lib}_LIBRARY ${lib} PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)
        if(${lib}_LIBRARY)
            set(${lib}_FOUND TRUE)
            if(NOT TARGET aries_scaffold::${lib})
                add_library(aries_scaffold::${lib} INTERFACE IMPORTED)
                set_target_properties(aries_scaffold::${lib} PROPERTIES
                    INTERFACE_LINK_LIBRARIES "${lib}_LIBRARY"
                )
                message(STATUS "找到依赖库: " ${lib} " - " ${${lib}_LIBRARY})
            endif()
        endif()
endmacro()

function(find_my_depends)
    # 遍历需要依赖检查的库，逐个进⾏库的查找
    foreach(lib ${libs})
        find_libraries(${lib})
        if(NOT ${lib}_FOUND)
            message(FATAL_ERROR "丢失关键依赖库: ${lib}")
        endif()
    endforeach()
    # 找到了库，则将库添加到项⽬链接属性中，这样使⽤者就不⽤额外添加这些⼦依赖库的链接了
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})
endfunction()
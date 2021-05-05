add_executable(managing_thread managing_thread.cc)
target_link_libraries(managing_thread pthread)

add_executable(sharing_data_between_thread sharing_data_between_thread.cc)
target_link_libraries(sharing_data_between_thread pthread)

add_executable(synchronizing_concurrent_operation synchronizing_concurrent_operation.cc)
target_link_libraries(synchronizing_concurrent_operation pthread)

add_executable(ATM ATM.cc)
target_link_libraries(ATM pthread)

add_executable(mem_model_and_atomic_type mem_model_and_atomic_type.cc)
target_link_libraries(mem_model_and_atomic_type pthread atomic)


# 如何为有多级子目录的代码添加 CMakeLists.txt https://www.jb51.cc/c/120176.html 
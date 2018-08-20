cmake_minimum_required(VERSION 3.4.1)

# 把系统的log库导入进来
find_library( log-lib
              log )

set(distribution_DIR ../../../../libs)

# 把libfmod.so预加载进来
add_library( fmod
             SHARED
             IMPORTED)
set_target_properties( fmod
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/${ANDROID_ABI}/libfmod.so)

# 把libfmodL.so预加载进来
add_library( fmodL
             SHARED
             IMPORTED)
set_target_properties( fmodL
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/${ANDROID_ABI}/libfmodL.so)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11"


add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp )

include_directories(src/main/cpp/inc)

target_link_libraries( native-lib
                       fmod
                       fmodL
                       ${log-lib} )
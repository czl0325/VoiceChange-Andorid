# VoiceChange-Andorid
采用Android studio 3.1.4开发，使用fmod库实现变声技巧


## 1.下载fmode库

前往  https://www.fmod.com/ 下载fmod库，上方有个download，点击进去，选择安卓版本。

![](https://upload-images.jianshu.io/upload_images/2587882-00c864f7e8487b4b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 2.加入库

Android studio3.1.4，新建一个工程，勾选include c/c++ support，把异常检测和安全转换也勾选上。

![](https://upload-images.jianshu.io/upload_images/2587882-036360cebefc3ac9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

打开下载之后解压的fmod目录，我们使用目录 api/lowlevel的库，先把inc的文件都拷贝到cpp文件夹里进去，再把so库和jar包也加载到libs文件夹里。

![](https://upload-images.jianshu.io/upload_images/2587882-3f500e1207caa57b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

jar文件要右键 ，add as library

## 3. 配置CMakeLists.txt文件

```
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
```

需要先把两个动态库预加载进来，然后修改build.gradle的文件，红色部分

![](https://upload-images.jianshu.io/upload_images/2587882-5d1784e455c77606.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

编译时候发现提示不支持armeabi库，把libs文件夹的armeabi删除了。
至此编译全部成功。

## 4.编写代码

在MainActivity里面创建一个native的方法

```
public native static int playSound(String path,int type);
```

有两个参数，一个是播放的音频所在的路径，一个是type用萝莉声音或者大叔声音来播放。

在native-lib里面实现方法。

播放声音分5个步骤：

1.System_Create创建一个system
2.init初始化
3.createSound创建一个声音
4.playSound播放声音
5.system->update();执行后声音才能播放出去。
第四步->第五步之间可以添加一些声音的特殊处理。
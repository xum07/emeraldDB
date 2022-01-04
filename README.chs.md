本项目是EmeraldDB的一个重构练习作品

> 项目中所有代码均遵守GUN-AGPL-3.0开源协议

# 背景

本项目基于一门NOSQL数据库的课程，其链接为：[http://c.biancheng.net/view/1640.html](http://c.biancheng.net/view/1640.html)。在本链接的最下方，有对应的网盘链接，如果感兴趣可以直接下载查看源课程中的视频、PPT

有别于源课程，本项目中有一些不同之处：

1. 使用cmake作为项目组织工具
2. 基于TDD（测试驱动开发）的策略，因此项目尽可能的补充上了必要的自测试代码
3. 在源课程的基础上，本项目做了部分少量调整，如类的设计，引入了开源的更为方便的glog流日志等
4. 在代码的书写上，采用了更多的cpp语言新特性，更便于阅读和理解

# 项目结构

所有的源代码均位于 `src`目录下，所有的测试代码均位于 `test`目录下，同时在 `doc`目录下有一些帮助文档，比如类图说明

`src`目录下的子文件功能说明如下

```shell
├── client  客户端
├── dms     数据管理服务
├── driver  驱动
├── ixm     索引管理
├── mon     监控
├── msg     消息
├── oss     操作系统服务
├── pd      问题诊断
├── pmd     处理模块
├── rtn     运行时组件
└── utils   工具类
```

# 开源组件

为了尽可能降低项目规模和最终可执行文件的大小，某些可以通过系统或者其它比较方便的方式下载到的第三方库，并不会包含在本项目目录中

## 未包含项目中的

1. **boost**. 执行 `sudo apt install libboost1.71-all-dev`可以在Ubuntu系统中直接下载，其它Linux系统有类似命令
   > 注意，本项目基于boost 1.71版本
   >

## 包含在项目中的

1. **bson**. 可以直接从github下载源码：[bson-cpp](https://github.com/jbenet/bson-cpp)

   > bson项目仅有6个 `.cpp`文件，而有一大堆的 `.h`文件会被外部引用，所以为了方便起见，直接将该项目以源代码的方式包含在内。
   > 项目中修复了一些bson中的bug
   >
2. **log4j**. 为了方便起见，直接使用1.2.17版本，可以从官方地址下载：[https://logging.apache.org/log4j/1.2/download.html](https://logging.apache.org/log4j/1.2/download.html)
3. **googltest**. 可以从github下载源码，需要本地编译后使用：[googletest github](https://github.com/google/googletest)
4. **glog**. 可以从github下载源码，需要本地编译后使用：[glog github](https://github.com/google/glog)

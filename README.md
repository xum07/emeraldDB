A practice of EmeraldDB

> ALL codes in this project obey the GUN-AGPL-3.0 opensource protocol

# Background

This project is a NOSQL database cpp practice, based on a lecture from [http://c.biancheng.net/view/1640.html](http://c.biancheng.net/view/1640.html). You can get the original lecture video and ppt at the end of the page.

The implements of this project have some differences from the lecture, such as:

1. use cmake as project organize tool
2. base on TDD(Test drive development), thus lots of tests added
3. some adjustments will be added, such as the design of class, log module and so on
4. some improvement of the ways to using CPP

# project structure

All source codes are in `src` folder, all test code are in `test` folder, and there are some help information about this project in `doc` folder.

The function of all folder in `src` blew:

```shell
├── client
├── dms     data manage service
├── driver
├── ixm     index manager
├── mon     monitor
├── msg     message
├── oss     operate system service
├── pd      problem diagnose
├── pmd     progress module
├── rtn     runtime
└── utils   utilities
```

# opensource

To reduce project size, some third-party libraries which can download through system or other convenient way will not be included in project folder.

## Not include

1. **boost**. In ubuntu like this: `sudo apt install libboost1.71-all-dev`
   > Yes, this project base on boost version 1.71
   >

## Include

1. **bson**. Get from [bson-cpp](https://github.com/jbenet/bson-cpp)

   > Project bson only have 6 `.cpp` source files, but lots of `.h` head files need to be included as interface. It's not wise to compile it as an external static library. Thus, directly include all files in this emeraldDB project
   > Some bugs in bson have been fixed
   >
2. **log4j**. Just use old version 1.2.17 for convenient. You can get it from here: [https://logging.apache.org/log4j/1.2/download.html](https://logging.apache.org/log4j/1.2/download.html)
3. **googltest**. Get from [googletest github](https://github.com/google/googletest) and compile it in local machine
4. **glog**. Get from [glog github](https://github.com/google/glog) and compile it in local machine

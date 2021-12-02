A practice of emeralddb

# Background
This project is a NOSQL database cpp practice, based on a lecture from [http://c.biancheng.net/view/1640.html](http://c.biancheng.net/view/1640.html). You can get the original lecture video and ppt at the end of the page.

The implements of this project have some differences from the lecture, such as:
1. use cmake as project organize tools
2. base on TDD(Test drive development)
3. some improvement of the ways to using CPP

# project structure
The function of all folder in `src`
```shell
├── client
├── dms     data manage service
├── driver
├── include
├── ixm     index manager
├── mon     monitor
├── msg     message
├── oss     operate system service
├── pd      problem diagnose
├── pmd     progress module
└── rtn     runtime
└── utils   utilities
```

# opensource
To reduce project size, some third-party libraries which can download through system or other convenient way will not be included in project folder.

## Not include
1. **boost**. In ubuntu like this: "sudo apt install libboost1.71-all-dev"
   > Yes, this project base on boost version 1.71

2. **gtest/gmock**. In ubuntu like this: "sodu apt install libgtest-dev libgmock-dev", no version restrictions

## Include
1. **bson**. Get from [bson-cpp](https://github.com/jbenet/bson-cpp)
   > Project bson only have 6 `.cpp` source files, but lots of `.h` head files need to be included as interface. It's not wise to compile it as an external static library. Thus, directly include all files in this emeraldDB project

2. **log4j**. Just use old version 1.2.17 for convenient. You can get it from here: [https://logging.apache.org/log4j/1.2/download.html](https://logging.apache.org/log4j/1.2/download.html)
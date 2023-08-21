# CrashDemo

CrashDemo是集成Google的Crashpad崩溃报告库以及Breakpad崩溃工具的一个演示示例

基于CMake构建工具，以跨平台的方式集成Crashpad。

目前已测试Linux平台下的使用

## 参考链接

[Qt开发之抓取崩溃信息](https://blog.51cto.com/u_15854865/5811230)


## 注意事项

编译程序时，Linux平台下需要追加`-g -Wl,--build-id`使得可执行程序中包含符号，以及函数名及行号等必要信息

崩溃工具目录：`ThirdParty/Crashpad/bin/`，根据不同平台，将目录中的所有可执行程序拷贝到CrashDemo的可执行程序同级目录下
如Linux平台下，拷贝`crashpad_handler`、`dump_syms`、`minidump_stackwalk`到`Build/**/`。

每次修改源码编译程序后，使用dump_syms程序生成.sym文件
如Linux平台下，在CrashDemo可执行程序目录下，使用命令`./dump_syms ./CrashDemo > CrashDemo.sym`

CrashDemo程序运行崩溃后，会在可执行程序同级目录的Crashes文件夹中，生成.dmp文件。


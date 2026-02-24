# 1.设置cmake最低版本要求
cmake_minimum_required(VERSION 3.18)

# 2.设置项目名称
project(testCMakeTools)

# 3.添加可执行目标
add_executable(main main.cc)

# 4.开启测试功能
include(CTest)
add_test(
    NAME test_add
    COMMAND main
)

# 5.本机安装
include(GNUInstallDirs)
install(TARGETS main)

# 6.打包
# CPack会默认收集install对应的安装目标，然后会将收集到的目标 打包放在压缩包里
include(CPack)

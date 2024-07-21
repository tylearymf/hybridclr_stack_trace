# hybridclr_stack_trace

## 背景

我自己在集成了 hybridclr 的一个测试项目中，当代码发生异常时，看不到对应的源码文件和行号，调试起来有点麻烦，所以我尝试对hybridclr库进行了修改，以添加此功能。

## 概述

捕获异常时，输出包含异常的源码文件和行号的错误消息。

### 注意事项

请注意，该库可能会出现不稳定崩溃，只能用来测试哈，而且对性能和稳定性也有影响的，别拿来跑正式包，出问题别找我。

## 使用步骤

1. **挂载 IL2CppBridge 脚本**

   在 Unity 场景中，先挂载 `IL2CppBridge` 脚本。该脚本负责给IL2Cpp提供接口调用

2. **热更代码出现异常时**

   当你在热更代码中出现异常时，你会看到两份异常日志：

   - 一份是来自原有的异常处理机制。
   - 另一份是我们新添加的日志，其中包含了异常发生的行号。

## 环境说明

- **Unity 版本**: 2021.3.24
- **操作系统**: Windows
- **编译目标**: Android（正常运行）

注意：目前这些修改已在 Windows 环境下编译并验证 Android 包的正常运行。Mac 环境下的编译尚未测试，但理论上修改应与 Windows 下相同。如果在 Mac 环境下编译，请注意检查任何平台特有的问题。

## 日志对比

```c#
原来的日志如下
2024/07/21 19:43:48.152 7347 7427 Error Unity NullReferenceException: Object reference not set to an instance of an object.
2024/07/21 19:43:48.152 7347 7427 Error Unity   at InstantiateByAddComponent.<Test1>g__Test1_3|4_2 () [0x00000] in <00000000000000000000000000000000>:0 
2024/07/21 19:43:48.152 7347 7427 Error Unity   at InstantiateByAddComponent.<Test1>g__Test1_2|4_1 () [0x00000] in <00000000000000000000000000000000>:0 
2024/07/21 19:43:48.152 7347 7427 Error Unity   at InstantiateByAddComponent.<Test1>g__Test1_1|4_0 () [0x00000] in <00000000000000000000000000000000>:0 
2024/07/21 19:43:48.152 7347 7427 Error Unity   at InstantiateByAddComponent.Test1 () [0x00000] in <00000000000000000000000000000000>:0 
2024/07/21 19:43:48.152 7347 7427 Error Unity   at InstantiateByAddComponent.Update () [0x00000] in <00000000000000000000000000000000>:0 

```

```c#
新加的带行号日志如下
2024/07/21 19:43:48.147 7347 7427 Error Unity Object reference not set to an instance of an object.
2024/07/21 19:43:48.147 7347 7427 Error Unity InstantiateByAddComponent.<Test1>g__Test1_3|4_2() (at F:\Unity Projects\hybridclr_trial\Assets\HotUpdate\InstantiateByAddComponent.cs:46)
2024/07/21 19:43:48.147 7347 7427 Error Unity InstantiateByAddComponent.<Test1>g__Test1_2|4_1() (at F:\Unity Projects\hybridclr_trial\Assets\HotUpdate\InstantiateByAddComponent.cs:49)
2024/07/21 19:43:48.147 7347 7427 Error Unity InstantiateByAddComponent.<Test1>g__Test1_1|4_0() (at F:\Unity Projects\hybridclr_trial\Assets\HotUpdate\InstantiateByAddComponent.cs:52)
2024/07/21 19:43:48.147 7347 7427 Error Unity InstantiateByAddComponent.Test1() (at F:\Unity Projects\hybridclr_trial\Assets\HotUpdate\InstantiateByAddComponent.cs:55)
2024/07/21 19:43:48.147 7347 7427 Error Unity InstantiateByAddComponent.Update() (at F:\Unity Projects\hybridclr_trial\Assets\HotUpdate\InstantiateByAddComponent.cs:28)

```


>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# 第一章
## 1.1添加自定义的架构
1. 修改llvm-project/llvm/include/llvm/TargetParser/Triple.h文件，在 Triple class 的 enum ArchType中添加day，再添加isDAY()函数，格式参见isMIPS().
2. 修改llvm-project/llvm/lib/TargetParser/Triple.cpp文件，在getArchTypeName(),getArchTypePrefix(),getDefaultFormat(),getArchPointerBitWidth(),get32BitArchVariant()函数下添加对应的switch case条件，格式可以参照对应函数中其它架构的写法，注意一下位宽就行了。
3. 在llvm-project/llvm/lib/Target文件夹下创建Day文件夹，在这个文件夹中，需要添加**CMakeLists.txt文件**，需要添加**多个.cpp/.h文件**，需要添加**多个.td文件**，需要添加**多个子文件夹**及其**子文件夹中的CMakeList.txt和cpp/h/td文件**。首次必须要添加的文件有：

|要添加的文件 |说明|
| :----: | :---- |
|**DayTargetMachine.h/.cpp**|DayTargetMachine.h: 声明 DayTargetMachine 类。这个类是你的后端的核心，它负责管理目标相关的各种组件，比如指令信息、目标数据布局、目标文件类型等等。DayTargetMachine.cpp: 实现 DayTargetMachine 类，需要在这里初始化并返回各种目标相关的组件，例如 TargetLowering、InstrInfo、Subtarget 等。|
|**DayTargetInfo.h/.cpp**   |声明 DayTargetInfo 类，这是一个非常简单的类，主要用于注册目标。DayTargetInfo.cpp: 实现 DayTargetInfo 类，你会使用 RegisterTarget 宏来注册你的 "day" 目标。这是LLVM能够识别你的目标架构的关键。|
|**Day.td**                 |这是一个 TableGen 文件，用于定义你的架构的通用属性，例如目标名称、目标描述等。|

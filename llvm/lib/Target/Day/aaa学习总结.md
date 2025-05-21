>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# 第一章
## 1.1添加自定义的架构
1. 修改llvm-project/llvm/include/llvm/TargetParser/Triple.h文件，在 Triple class 的 enum ArchType中添加day，再添加isDAY()函数，格式参见isMIPS().
2. 修改llvm-project/llvm/lib/TargetParser/Triple.cpp文件，在getArchTypeName(),getArchTypePrefix(),getDefaultFormat(),getArchPointerBitWidth(),get32BitArchVariant()函数下添加对应的switch case条件，格式可以参照对应函数中其它架构的写法，注意一下位宽就行了。
3. 在llvm-project/llvm/lib/Target文件夹下创建Day文件夹，在这个文件夹中，需要添加**CMakeLists.txt文件**，需要添加**多个.cpp/.h文件**，需要添加**多个.td文件**，需要添加**多个子文件夹**及其**子文件夹中的CMakeList.txt和cpp/h/td文件**。首次必须要添加的文件有：
|要添加的文件 |说明|
| :----: | :----: |
|**DayTargetMachine.h/.cpp**|
|**DayTargetInfo.h/.cpp**   |
|**Day.td**                 |

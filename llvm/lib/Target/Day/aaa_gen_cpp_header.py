

import sys
import os
import socket
import datetime

original_stdout = sys.stdout  # 保存原来的 stdout

try:
    file_name = sys.argv[1]
except:
    print(f"\033[1;31m Error: No File Name, please add your file name, as follow:\
        \n\033[34m $python aaa_gen_cpp_header.py YourFileName \033[0m")

company_name = "LightStandard"

try:
    with open(f'{file_name}.cpp', 'x') as f:  # 'x' 模式：文件存在则报错
        sys.stdout = f  # 重定向 stdout 到文件
        print(f"/*******************************************************")
        print(f"** File: {file_name}.cpp")
        print(f"** Author: {socket.gethostname()}")
        print(f"** Date: {datetime.datetime.now().date()}")
        print(f"** Company: {company_name}")
        print(f"*******************************************************/\n\n")
        print(f"#include \"{file_name}.h\"\n\n")
except FileExistsError:
    print(f"Error: '{file_name}.cpp' already exists. Not overwriting.")
finally:
    sys.stdout = original_stdout  # 确保恢复 stdout


try:
    with open(f'{file_name}.h', 'x') as f:  # 'x' 模式：文件存在则报错
        sys.stdout = f  # 重定向 stdout 到文件
        print(f"/*******************************************************")
        print(f"** File: {file_name}.h")
        print(f"** Author: {socket.gethostname()}")
        print(f"** Date: {datetime.datetime.now().date()}")
        print(f"** Company: {company_name}")
        print(f"*******************************************************/\n\n")
        
        define_str = f"{file_name.upper()}_H"
        print(f"#ifndef {define_str}")
        print(f"#define {define_str}\n\n\n")
        
        print(f"class {file_name}: public/*Add Parent Class*/ ParentClass")
        print("{\n\n\n\n\n\n\n")
        
        print("};\n\n")
        print(f"#endif // {file_name}\n")
        
except FileExistsError:
    print(f"Error: '{file_name}.h' already exists. Not overwriting.")
finally:
    sys.stdout = original_stdout  # 确保恢复 stdout


# 之后的 print 会正常输出到控制台
print("Back to normal stdout.")



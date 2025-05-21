# 使用LLVM工具链
CXX := ../build/bin/clang
LLVM_LINK := ../build/bin/llvm-link
LLVM_OPT := ../build/bin/opt
LLC = ../build/bin/llc
MAIN1 = main1
MAIN = main

# 项目目录结构
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
INC_DIR := inc
SRC_DIR := src

# 源文件和目标文件
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LLVM_BCS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.bc,$(SRCS))
LLVM_LL := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.ll,$(SRCS))

# 可执行文件
TARGET := $(BIN_DIR)/program

# 编译选项
CXXFLAGS := -I$(INC_DIR) -Wall -Wextra -O0
LDFLAGS := -fuse-ld=$(shell pwd)/../build/bin/ld.lld

# 默认目标
all: $(TARGET) bitcode ir

bitcode: $(LLVM_BCS)  # 生成所有 .bc 文件
ir: $(LLVM_LL)  # 生成所有 .ll 文件

run:$(TARGET)
	@./$(TARGET)

# 链接可执行文件
$(TARGET): $(OBJS) | $(BIN_DIR)
		$(CXX) $(LDFLAGS) -o $@ $^

# 编译源文件到对象文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CXX) $(CXXFLAGS) -c $< -o $@

# 可选：生成LLVM bitcode（用于分析等）
$(OBJ_DIR)/%.bc: $(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CXX) -I$(INC_DIR) -emit-llvm -c $< -o $@

$(OBJ_DIR)/%.ll: $(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CXX) -target riscv32-unknown-elf -march=rv32imafdc -mabi=ilp32d -O0 -S -emit-llvm $< -o $@ #

# 创建必要的目录
$(BIN_DIR):
		mkdir -p $@

$(OBJ_DIR):
		mkdir -p $@

riscv32-build1:
	riscv32-unknown-elf-clang -mabi=ilp32d build/obj/$(MAIN1).s -o build/bin/$(MAIN1)

riscv32-run1:
	qemu-riscv32 build/bin/$(MAIN1)

riscv32-build:
	riscv32-unknown-elf-clang -mabi=ilp32d build/obj/$(MAIN).s -o build/bin/$(MAIN)

riscv32-run:
	qemu-riscv32 build/bin/$(MAIN)

gendag:
	mkdir -p ./build/dag
	rm -f /tmp/dag.main-*.dot
	export LLVM_DOT_FILE_PRINT_PREFIX=$(pwd)/build/dag/  # 设置环境变量
	# $(LLC) -view-dag-combine1-dags build/obj/$(MAIN).ll
	$(LLC) -view-sched-dags build/obj/$(MAIN).ll
	dot -Tsvg /tmp/dag.main-*.dot -o build/dag/$(MAIN).svg
	firefox build/dag/$(MAIN).svg # 需要安装火狐浏览器

llc:
	$(LLC) 


# 清理
clean:
	rm -rf $(BUILD_DIR)/bin
	rm -rf $(BUILD_DIR)/obj/$(MAIN).*

# 显示帮助信息
help:
		@echo "可用目标:"
		@echo "  all       - 构建项目（默认）"
		@echo "  clean     - 清理构建文件"
		@echo "  help      - 显示此帮助信息"

.PHONY: all clean help ir bitcode run

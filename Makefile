# 当前目录找不到依赖/目标文件时,去指定路径查找
VPATH ?= .

SRC := wavfile.c
TEST_SRC := test.c

OBJECTS := $(SRC:.c=.o) # 加了:前面的变量不能使用后面的变量
TEST_OBJECTS := $(TEST_SRC:.c=.o) # 加了:前面的变量不能使用后面的变量

CC := gcc
CXX := g++
RM := -rm -rf # -表示命令出错.继续执行,忽略错误.
STRIP := strip
AR := ar

CPPFLAGS := -I./ # C预处理参数,一般设置I
CFLAGS := -Wall -Wall -Wno-variadic-macros -Wno-format-zero-length -O2 -std=c99 -fPIC -DNDEBUG -D_GNU_SOURCE -fvisibility=hidden # 编译器参数,C使用
CXXFLAGS := $(CFLAGS) # C++使用
LDFLAGS :=  # 链接器参数,如ld ,最好放在源文件之后.

TARGET := wavfile
EXE := $(TARGET)
STATIC := lib$(TARGET).a
SHARE := lib$(TARGET).so

ALL := $(EXE) $(STATIC) $(SHARE)

.PHONY: $(ALL) $(EXE) $(SHARE) $(STATIC)
all: $(ALL)

$(EXE): $(OBJECTS) $(TEST_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(SHARE): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -shared -o $@
	$(STRIP) --strip-unneeded $@

$(STATIC): $(OBJECTS)
	$(AR) crv $@ $^

.PHONY: clean 
clean:
	$(RM) $(OBJECTS) $(TEST_OBJECTS) $(ALL)

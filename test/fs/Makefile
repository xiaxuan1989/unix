PROJECT_ROOT ?= $(abspath ../..)
BUILD_DIR ?= $(PROJECT_ROOT)/build/test

TARGET := $(BUILD_DIR)/fs/TestFileSystem.o

.PHONY: all

all: $(TARGET)

$(TARGET): TestFileSystem.cpp TestFileSystem.h ../KernelInclude.h ../TestUtility.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

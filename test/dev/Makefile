PROJECT_ROOT ?= $(abspath ../..)
BUILD_DIR ?= $(PROJECT_ROOT)/build/test

TARGET := $(BUILD_DIR)/dev/TestBufferManager.o

.PHONY: all

all: $(TARGET)

$(TARGET): TestBufferManager.cpp TestBufferManager.h ../KernelInclude.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

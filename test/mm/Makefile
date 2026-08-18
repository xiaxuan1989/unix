PROJECT_ROOT ?= $(abspath ../..)
BUILD_DIR ?= $(PROJECT_ROOT)/build/test

TARGET := $(BUILD_DIR)/mm/TestSwapperManager.o

.PHONY: all

all: $(TARGET)

$(TARGET): TestSwapperManager.cpp TestSwapperManager.h ../KernelInclude.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

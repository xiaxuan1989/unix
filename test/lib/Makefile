PROJECT_ROOT ?= $(abspath ../..)
BUILD_DIR ?= $(PROJECT_ROOT)/build/test

TARGETS := $(BUILD_DIR)/lib/Lib.o $(BUILD_DIR)/lib/TestLib.o

.PHONY: all

all: $(TARGETS)

$(BUILD_DIR)/lib/Lib.o: Lib.cpp Lib.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/lib/TestLib.o: TestLib.cpp TestLib.h ../KernelInclude.h ../TestInclude.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

PROJECT_ROOT := $(abspath ..)
BUILD_DIR ?= $(PROJECT_ROOT)/build/test

UNAME_S := $(shell uname -s)
ifeq ($(origin CXX), default)
  ifeq ($(UNAME_S),Darwin)
    CXX := $(shell command -v x86_64-elf-g++)
  else
    CXX := $(shell command -v g++)
  endif
endif

CPPFLAGS += -I$(PROJECT_ROOT)/src -I$(PROJECT_ROOT)/src/include -I$(PROJECT_ROOT)/test
CXXFLAGS += -std=gnu++20 -Wall -g -m32 -fno-builtin -fno-pic -fno-pie
CXXFLAGS += -fno-rtti -fno-stack-protector -fno-threadsafe-statics
CXXFLAGS += -fno-exceptions -fno-use-cxa-atexit -mno-red-zone
CXXFLAGS += -mno-mmx -mno-sse -mno-sse2

export PROJECT_ROOT BUILD_DIR CXX CPPFLAGS CXXFLAGS

TEST_DIRS := dev fs lib mm
TEST_UTILITY_OBJ := $(BUILD_DIR)/TestUtility.o

.PHONY: all check-toolchain clean $(TEST_DIRS)

all: check-toolchain $(TEST_UTILITY_OBJ) $(TEST_DIRS)
	@echo "test sources compiled successfully: $(BUILD_DIR)"

check-toolchain:
	@if [ -z "$(CXX)" ] || ! command -v "$(CXX)" >/dev/null 2>&1; then \
		echo "C++ compiler not found. On macOS, install x86_64-elf-g++." >&2; \
		exit 1; \
	fi

$(TEST_DIRS):
	$(MAKE) --directory=$@

$(TEST_UTILITY_OBJ): TestUtility.cpp TestUtility.h KernelInclude.h
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

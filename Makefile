# Unix 顶层构建脚本

.DEFAULT_GOAL := all
FILE_SYS_TOOLS_BIN_DIR:=tools/filesystem-editor/bin
TOOLS:=$(sort $(wildcard $(FILE_SYS_TOOLS_BIN_DIR)/*))
UNAME_S := $(shell uname -s)
X86_64_ELF_GCC := $(shell command -v x86_64-elf-gcc)
X86_64_ELF_GXX := $(shell command -v x86_64-elf-g++)
X86_64_ELF_AR := $(shell command -v x86_64-elf-ar)
X86_64_ELF_RANLIB := $(shell command -v x86_64-elf-ranlib)
X86_64_ELF_LD := $(shell command -v x86_64-elf-ld)
X86_64_ELF_OBJCOPY := $(shell command -v x86_64-elf-objcopy)
X86_64_ELF_OBJDUMP := $(shell command -v x86_64-elf-objdump)

ifeq ($(UNAME_S),Darwin)
  CMAKE_TOOLCHAIN_ARGS := -DCMAKE_C_COMPILER=$(X86_64_ELF_GCC) -DCMAKE_CXX_COMPILER=$(X86_64_ELF_GXX) -DCMAKE_ASM_NASM_COMPILER=nasm -DCMAKE_AR=$(X86_64_ELF_AR) -DCMAKE_RANLIB=$(X86_64_ELF_RANLIB) -DCMAKE_LINKER=$(X86_64_ELF_LD) -DCMAKE_OBJCOPY=$(X86_64_ELF_OBJCOPY) -DCMAKE_OBJDUMP=$(X86_64_ELF_OBJDUMP) -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
  OBJDUMP := x86_64-elf-objdump
  QEMU_ACCEL := -accel tcg
  QEMU_CPU := qemu32
else
  CMAKE_TOOLCHAIN_ARGS :=
  OBJDUMP := objdump
  QEMU_ACCEL := -enable-kvm
  QEMU_CPU := Icelake-Server
endif


ifeq ($(word 1, $(TOOLS)),)
$(error "filescanner not found. please run 'bash init.sh' first")
endif
ifeq ($(word 2, $(TOOLS)),)
$(error "fsedit not found. please run 'bash init.sh' first")
endif


.PHONY: help
help:
	@echo "unix makefile"
	@echo "---------------"
	@echo "commands available:"
	@echo "- make bochs"
	@echo "    build and launch unix using Bochs"
	@echo "- make qemu"
	@echo "    build and launch unix using QEMU"
	@echo "- make qemug"
	@echo "    build and launch unix using QEMU (with GDB)"
	@echo "- make"
	@echo "    alias for \"make all\""


.PHONY: prepare
prepare:
	mkdir -p target/objs/asm-dump


.PHONY: build-programs
build-programs: prepare
	mkdir -p target/objs/apps
	mkdir -p target/objs/apps-elf
	mkdir -p build/apps && cd build/apps \
	&& cmake -G"Ninja" ../../programs -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_TOOLCHAIN_ARGS) && \
	cmake --build . -- -j 4


.PHONY: build-lib
build-lib: prepare
	mkdir -p build/lib && cd build/lib \
	&& cmake -G"Ninja" ../../lib/src -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_TOOLCHAIN_ARGS) \
	&& cmake --build . -- -j 4
	mkdir -p target/objs
	cp build/lib/libunix_runtime.a target/objs/libunix_runtime.a


.PHONY: build-shell
build-shell: prepare
	mkdir -p build/shell && cd build/shell \
	&& cmake -G"Ninja" ../../shell -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_TOOLCHAIN_ARGS) && \
	cmake --build . -- -j 2
	mkdir -p target/objs
	$(OBJDUMP) -d target/objs/Shell.exe > target/asm-dump/Shell.exe.text.asm  # optional
	$(OBJDUMP) -D target/objs/Shell.exe > target/asm-dump/Shell.exe.full.asm  # optional


.PHONY: build-kernel
build-kernel: prepare
	mkdir -p build/kernel && cd build/kernel \
	&& cmake -G"Ninja" ../../src -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_TOOLCHAIN_ARGS) && \
	cmake --build . -- -j 1


.PHONY: build-full
build-full: prepare build-lib build-programs build-shell build-kernel



.PHONY: deploy-full
deploy-full: build-full
	mkdir -p target/img-workspace
	mkdir -p target/img-workspace/programs/bin
	mkdir -p target/img-workspace/programs/etc
	cp target/objs/kernel.bin target/img-workspace/
	cp target/objs/boot/boot.bin target/img-workspace/
	cp target/objs/apps/* target/img-workspace/programs/bin/
	cp target/objs/Shell.exe target/img-workspace/programs/
	cp tools/filesystem-editor/bin/* target/img-workspace/
	cp tools/splash/splash.bmp target/img-workspace/programs/etc/
	cd target/img-workspace && ./filescanner | ./fsedit c.img c
	cp target/img-workspace/c.img target/



.PHONY: bochs
bochs:
	@echo 'not supported. use \"make qemu\" instead.'



QEMU := qemu-system-i386 
QEMU += -m 64M 
QEMU += -rtc base=localtime 
QEMU += -d cpu_reset -D target/qemu.log 
QEMU += -machine pc 
QEMU += -cpu $(QEMU_CPU) 
QEMU += $(QEMU_ACCEL) 
QEMU_GDB := -chardev socket,path=target/qemu-gdb.sock,server=on,wait=off,id=gdb0 
QEMU_GDB += -gdb chardev:gdb0 -S 

QEMU_DISK := -boot c -drive file=target/c.img,if=ide,index=0,media=disk,format=raw


.PHONY: qemu-no-rebuild
qemu-no-rebuild:
	$(QEMU) $(QEMU_DISK)


.PHONY: qemug-no-rebuild
qemug-no-rebuild:
	$(QEMU) $(QEMU_DISK) $(QEMU_GDB)


.PHONY: qemu
qemu: deploy-full qemu-no-rebuild


.PHONY: qemug
qemug: deploy-full qemug-no-rebuild


.PHONY: clean
clean:
	rm -rf ./target
	rm -rf ./build


.PHONY: all
all: deploy-full
	@echo -e "\033[32mbuild success - unix.\033[0m"


.PHONY: full
full: deploy-full

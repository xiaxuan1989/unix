# Unix V6 C/C++

针对IA32架构开发的Unix V6系统。

## 物理内存占用约定

| 起始地址 | 终止地址（含） | 大小 |        用途和备注        |
| :------: | :------------: | :---: | :-----------------------: |
|    0    |      3FF      |  1KB  |   中断向量（BIOS保留）   |
|   400   |      4FF      | 256B |   BIOS数据（BIOS保留）   |
|   1000   |      1FFF      |  4KB  |       启动时的PML2       |
|    ?    |      7BFF      |   ?   |       启动时核心栈       |
|   7C00   |      7DFF      | 512B | 启动引导程序：一级启动器 |
|   7E00   |      7EFF      | 256KB |       VBE Mode Info       |
|  9FC00  |     9FFFF     |  1KB  | 扩展BIOS数据区（BIOS保留） |

CMake 和 Makefile 混合使用。其中，Makefile 用于处理交互指令，其控制 CMake 完成编译构建等工作。编译系统会为每个子程序同时生成 ELF 和 PE 版本。其中，ELF 版本供 GDB 加载使用，PE 版本供 Unix 内核使用。

VSCode 配置文件，包含 C++ 头文件搜索路径以及调试器启动参数。

虚拟机从 Bochs 换成 QEMU。虚拟机运行内存从 32M 提升到 64M

#### 文件系统模块排布

原来的版本限制内核文件不能超过 99K。事实上，老版本系统的大小已经刚好卡住这个值了。一旦添加新功能，内核大小很容易突破这个上限，导致内核无法被完整加载。通过将文件系统 INode 及以后的区域整体后移，我们将内核大小上限扩大到了 398 个盘块，即 199K，暂时看足够使用。

| 存放内容             | 变更前存放盘块 | 变更后存放盘块 |
| -------------------- | -------------- | -------------- |
| 启动引导（boot.bin） | [0, 0]         | [0, 0]         |
| 内核（kernel.bin）   | [1, 199]       | [1, 399]       |
| SuperBlock           | [200, 201]     | [400, 401]     |
| INode                | [202, 1023]    | [402, 1223]    |
| Data                 | [1024, 17999]  | [1224, 18199]  |
| Swap                 | [18000, 20159] | [18200, 20359] |

变更：

```cpp

// git diff ./src/include/FileSystem.h
-       static const int SUPER_BLOCK_SECTOR_NUMBER = 200; 
+       static const int SUPER_BLOCK_SECTOR_NUMBER = 400;   

-       static const int INODE_ZONE_START_SECTOR = 202; 
-       static const int INODE_ZONE_SIZE = 1024 - 202;  
+       static const int INODE_ZONE_START_SECTOR = 402; 
+       static const int INODE_ZONE_SIZE = 1224 - 402;  
 
-       static const int DATA_ZONE_START_SECTOR = 1024;   
-       static const int DATA_ZONE_END_SECTOR = 18000 - 1;  
-       static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR; 
+       static const int DATA_ZONE_START_SECTOR = 1224;   
+       static const int DATA_ZONE_END_SECTOR = 18200 - 1; 
+       static const int DATA_ZONE_SIZE = 18200 - DATA_ZONE_START_SECTOR; 

// git diff ./src/mm/SwapperManager.cpp
-unsigned int SwapperManager::SWAPPER_ZONE_START_BLOCK = 18000;
+unsigned int SwapperManager::SWAPPER_ZONE_START_BLOCK = 18200;
```

原版PE Parser假定了每个段的名字和位次，但新版编译器不一定遵循该规范，导致可执行程序可能无法正确加载。新PE Parser改用字符串匹配的方式寻找需要的程序段，以解决该问题。此外，原 PE Parser 部分代码存在错误，已对部分问题进行修复。

考虑到 ELF 是 Unix 家族正统的可执行文件格式，且 GNU/Linux 下的 GDB 加载 PE 格式文件会出问题，特实现 ELF 格式加载器。有bug，该功能暂不可用。

开机时闪烁开屏图片 `splash.bmp`。

![img](tools/splash/splash.bmp)

可以在 `src/CMakeLists.txt` 里禁用。

在启动引导过程中，启用 CPU PSE 功能，以支持 4MB 大页映射。参考：[https://wiki.osdev.org/Paging](https://wiki.osdev.org/Paging)

使用 VESA 控制显示屏，并在其上实现一个显示空间更大，色彩更艳丽、支持屏幕滚动的控制台。目前这版的 VESA 驱动仅在 QEMU 平台测试成功，不保证在其他环境下的准确性。VESA 显存映射空间被放置在内核区的 128MB 位置（即 3GB + 128 MB 处），显存总大小约为 2MB。VESA 支持可以在 src/CMakeLists.txt 内手动开关。

| CRT                                      | VESA                                     |
| ---------------------------------------- | ---------------------------------------- |
| ![img](./img/qemu-without-vesa.png) | ![img](./img/qemu-vesa-enabled.png) |

原版并没有手动开启 DMA 功能。可能是因为 Bochs 默认启用了，于是之前的代码一直没出错。QEMU 模拟的芯片组默认关闭 DMA 功能，需要手动开启。

libunixstd 库拥有更强的功能与更好的性能。

1. 使用参考自 `glibc` 的 `strlen` 函数，一次性可以判断 4 个字节。
2. 支持快速内存拷贝的 `memcpy`，在入参整齐时一次性拷贝 4 字节。这个改进可以很大程度提升 VESA Console 的滚屏体验（其实可以考虑使用AVX或SSE指令进一步加速）。

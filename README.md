# LCD2004-Driver-for-STM32

**A simple LCD2004 driver based on HAL library for STM32, the LCD must be attached to an i2C adapter board.**

To Use: the `lcd2004.c/h` is the source code and the `linker.h` is to include necessary
header files. You could manually integrate the `.h` files or just include `linker.h` in 
`main.c` as long as all the files have been added to the project.

Supported Functions：

Display a character / string at (ROWx,COLx). 

Add a custom character in CGRAM and display it.

Blinking characters / string.
Full screen clear.

-------------------------------------------

**基于HAL库的简单 LCD2004 I2C 驱动库文件，可在 STM32x 上运行。LCD本身必须连接到一个 I2C 转接板。**

使用：`lcd2004.c/h` 是程序源码，`linker.h` 整合了必要的头文件，你可以手动将`.h`文件合并，只要保证所有必要的头文件都已经被加入了项目之中。

目前支持的功能：
在 x 行 x 列显示 字符 / 字符串

在 CGRAM 中添加自定义字符并显示

闪烁的字符 / 字符串效果
全屏清除

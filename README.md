# LCD2004-Driver-for-STM32
A simple LCD2004 driver for STM32, the LCD must be attached to an i2C adapter board.
The original source of the code is **@ControllerTech**.

--------------------------------------------------------------------------------------
Edit: - Add comments
      - Customized Functions
      
--------------------------------------------------------------------------------------
To Use: the `lcd2004.c/h` is the source code and the `linker.h` is to include necessary
header files. You could manually integrate the `.h` files or just include `linker.h` in 
`main.c` as long as all the files have been added to the project.

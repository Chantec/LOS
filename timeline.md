### Day1

写了helloos.img。可以用二进制直接编写，也可以用汇编语言 汇编出来。他写了脚本可以直接在qemu上运行，按照书上的步骤即可。我现在先按照书上面来，等之后再自己用qemu。

todo:扇区引导具体这里还是不明白。

bug:Windows下路径得用\不能/。运行imgtol.com显示16位程序不兼容，但是还是可以跑出来。



### Day2

0x00007c00-0x00007dff ：启动区内容的装载地址

ipl  initial program loader



##### Makefile

命令行所在行开头必须是tab 用空格不行 会报 `Makefile:2: *** missing separator.  Stop.`



至于环境，有点复杂，我们之后再细细研究。我们先把重点放在nas上面。


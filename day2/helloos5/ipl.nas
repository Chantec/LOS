; hello-os
; TAB=4

		ORG		0x7c00			; 让程序装载到 0x7c00位置 orginal
                                ; 0x00007c00-0x00007dff ：启动区内容的装载地址

; 以下的记述用于标准FAT12格式的软盘

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; 启动区的名称
		DW		512				; 每个扇区的大小
		DB		1				; cluster的大小（必须为一个扇区）
		DW		1				; fat的起始位置 一般从第一个扇区开始
		DB		2				; fat的个数 必须为2
		DW		224				; 根目录的大小 一般224项
		DW		2880			; 该磁盘的大小 必须是2880扇区
		DB		0xf0			; 磁盘的种类 必须是0xf0
		DW		9				; fat的长度 必须是9扇区
		DW		18				; 一个磁道（track）有几个扇区 必须是18
		DW		2				; 磁头数 必须是2
		DD		0				; 不使用分区 必须是0
		DD		2880			; 重写一次磁盘大小
		DB		0,0,0x29		; 固定
		DD		0xffffffff		; 卷标号码
		DB		"HELLO-OS   "	; 磁盘的名称 11B
		DB		"FAT12   "		; 磁盘格式名称 8B
		RESB	18				; 空出18B

; プログラム本体

entry:
		MOV		AX,0			; 初始化ax
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,15			; 指定字符颜色  
		INT		0x10			; 调用显卡bios
		JMP		putloop
fin:
		HLT						; halt cpu暂时停止
		JMP		fin				;

msg:
		DB		0x0a, 0x0a		; 换行
		DB		"hello, world"
		DB		0x0a			; 换行
		DB		0

		RESB	0x7dfe-$		; 预留0 reserve byte

		DB		0x55, 0xaa

; hello-os
; TAB=4

		ORG		0x7c00			; ?程序装?到 0x7c00位置 orginal
                                ; 0x00007c00-0x00007dff ：??区内容的装?地址

; 以下的?述用于?准FAT12格式的??

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; ??区的名称
		DW		512				; ?个扇区的大小
		DB		1				; cluster的大小（必??一个扇区）
		DW		1				; fat的起始位置 一般从第一个扇区?始
		DB		2				; fat的个数 必??2
		DW		224				; 根目?的大小 一般224?
		DW		2880			; ?磁?的大小 必?是2880扇区
		DB		0xf0			; 磁?的?? 必?是0xf0
		DW		9				; fat的?度 必?是9扇区
		DW		18				; 一个磁道（track）有几个扇区 必?是18
		DW		2				; 磁?数 必?是2
		DD		0				; 不使用分区 必?是0
		DD		2880			; 重写一次磁?大小
		DB		0,0,0x29		; 固定
		DD		0xffffffff		; 卷?号?
		DB		"HELLO-OS   "	; 磁?的名称 11B
		DB		"FAT12   "		; 磁?格式名称 8B
		RESB	18				; 空出18B

; 程序本体

entry:
		MOV		AX,0			; 初始化ax
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ???

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; 柱面 0
		MOV		DH,0			; 磁? 0
		MOV		CL,2			; 扇区 2

		MOV		AH,0x02			; AH=0x02 ??
		MOV		AL,1			; 1个扇区
		MOV		BX,0
		MOV		DL,0x00			; A??器
		INT		0x13			; ?用磁?BIOS
		JC		error

; 読み終わったけどとりあえずやることないので寝る

fin:
		HLT						; halt
		JMP		fin				; 跳到fin

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1		
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; ?示一个文字
		MOV		BX,15			; 指定字符?色  
		INT		0x10			; ?用??bios
		JMP		putloop
msg:
		DB		0x0a, 0x0a		
		DB		"load error"
		DB		0x0a			
		DB		0

		RESB	0x7dfe-$	

		DB		0x55, 0xaa ;

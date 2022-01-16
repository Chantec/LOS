; hello-os
; TAB=4

		ORG		0x7c00			; ?������?�� 0x7c00�ʒu orginal
                                ; 0x00007c00-0x00007dff �F??����e�I��?�n��

; �ȉ��I?�q�p��?�yFAT12�i���I??

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; ??��I����
		DW		512				; ?�����I�召
		DB		1				; cluster�I�召�i�K??�꘢���j
		DW		1				; fat�I�N�n�ʒu ��ʘ���꘢���?�n
		DB		2				; fat�I���� �K??2
		DW		224				; ����?�I�召 ���224?
		DW		2880			; ?��?�I�召 �K?��2880���
		DB		0xf0			; ��?�I?? �K?��0xf0
		DW		9				; fat�I?�x �K?��9���
		DW		18				; �꘢�����itrack�j�L�{����� �K?��18
		DW		2				; ��?�� �K?��2
		DD		0				; �s�g�p���� �K?��0
		DD		2880			; �d�ʈꎟ��?�召
		DB		0,0,0x29		; �Œ�
		DD		0xffffffff		; ��?��?
		DB		"HELLO-OS   "	; ��?�I���� 11B
		DB		"FAT12   "		; ��?�i������ 8B
		RESB	18				; ��o18B

; �����{��

entry:
		MOV		AX,0			; ���n��ax
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ???

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; ���� 0
		MOV		DH,0			; ��? 0
		MOV		CL,2			; ��� 2

		MOV		AH,0x02			; AH=0x02 ??
		MOV		AL,1			; 1�����
		MOV		BX,0
		MOV		DL,0x00			; A??��
		INT		0x13			; ?�p��?BIOS
		JC		error

; �ǂݏI��������ǂƂ肠������邱�ƂȂ��̂ŐQ��

fin:
		HLT						; halt
		JMP		fin				; ����fin

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1		
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; ?���꘢����
		MOV		BX,15			; �w�莚��?�F  
		INT		0x10			; ?�p??bios
		JMP		putloop
msg:
		DB		0x0a, 0x0a		
		DB		"load error"
		DB		0x0a			
		DB		0

		RESB	0x7dfe-$	

		DB		0x55, 0xaa ;

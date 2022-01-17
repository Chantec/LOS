        org 0xc200

        mov al,0x13
        mov ah,0x00
        int 0x10                      ;vga显卡 320 200 8 b 彩色
fin:
        hlt
        jmp fin
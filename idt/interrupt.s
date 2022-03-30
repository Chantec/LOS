%macro ISR_NOERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli 
        push byte 0;为了和push err-code保持stack一致 我们使用一个占位
        push byte %1
        jmp isr_common_stub
%endmacro
%macro ISR_ERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli 
        push byte %1
        jmp isr_common_stub
%endmacro

;定义 isr 8,[10,14]是push err-code的 
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

ISR_NOERRCODE 255




[EXTERN isr_handler]

isr_common_stub:
    pusha ;pushes edi esi ebp esp ebx edx ecx eax

    mov ax,ds 
    push eax ;pushes ds 

    mov ax,0x10;liangtodo
    mov ds,ax 
    mov es,ax 
    mov fs,ax 
    mov gs,ax 

    call isr_handler

    pop eax ;还原segment
    mov ds,ax 
    mov es,ax 
    mov fs,ax 
    mov gs,ax 

    popa 

    add esp,8;stack+2 跳过 error-code,INTR number

    sti ;可以中断了 
    iret; cs eip eflags ss esp 








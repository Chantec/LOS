[global switch_to]

switch_to:
    mov eax,[esp+4];[esp+4]存放的是prev->context的地址

    ;保存现场
    mov [eax],esp 
    mov [eax+4],ebp
    mov [eax+8],ebx
    mov [eax+12],esi 
    mov [eax+16],edi 
    ;eflag
    pushf
    pop ecx 
    mov [eax+20],ecx
    
    ;保存eip的值
    call next
next:
    pop ebx 
    mov [eax+24],ebx

    ;恢复下文现场
    mov eax,[esp+8]
    
    mov esp,[eax]
    mov ebp,[eax+4]
    mov ebx,[eax+8]
    mov esi,[eax+12]
    mov edi,[eax+16]
    ;eflag
    
    mov ecx,[eax+20]
    push ecx 
    popf
  
    jmp [eax+24];给eip赋值



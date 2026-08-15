include Macros.inc

.data

CheckIfOnScreenSaver    DQ      0141AEE990h

IsScreenSaverOn         DQ      01445FEA1Bh

.code 

FUNC_BEGIN DynamicBSS

    xor     rcx, rcx
    mov     rax, [rcx]
    movzx   eax, byte ptr [rax+195h]
    ret

FUNC_END DynamicBSS

FUNC_BEGIN IsOnScreenSaver

    xor         bl, bl
    mov         [rcx+153h], bl
    movzx       ecx, bl
    mov         byte ptr [IsScreenSaverOn], 0
    jmp         loc(0141AE62CDh)

FUNC_END IsOnScreenSaver

end

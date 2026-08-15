
;replacement for MyFunctionName proc ... does alignment and .code correctly.
;usage: FUNC_BEGIN MyFunctionName
;
FUNC_BEGIN macro Name

_TEXT$00 segment para 'CODE'

        align   8
        public  Name
Name    proc

endm
        
;usage: FUNC_END MyFunctionName
;ends function / code ends
;        
FUNC_END macro Name

Name    endp
_TEXT$00 ends

endm


; Usage:
;
;       jmp     loc(123456h)
;
; I know it should be better done using LOCAL, but this way it looks
; nicer when disassembled, and each pointer is only defined once ;)

loc MACRO Address
	SaveSeg textequ @CurSeg
	Pointer textequ <ploc_&Address&>
	IFNDEF ploc_&Address&
		.CONST
		Pointer dq Address
		SaveSeg SEGMENT
	ENDIF
	EXITM <[Pointer]>
ENDM


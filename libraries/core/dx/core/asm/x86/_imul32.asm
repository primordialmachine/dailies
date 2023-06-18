.model flat

_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://www.felixcloutier.com/x86/imul
; int16_t __fastcall _dx_imul32(int16_t x, int16_t y, int16_t *z)
@_dx_imul32@12 PROC
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4
  ; z
  ; --------------

  ; Perform multiplication ecx * edx such that
  ; - the lower word of the product is in eax
  ; - the higher word in edx
  ; - overflow and carry flags are 1 if an overflow occurs, otherwise they are 0.
  MOV ax, cx                    ; ax@1 := cx@1
  IMUL dx                       ; dx@2 := hi(ax@1 * dx@1), ax@2 = lo(ax@1 * dx@1)

  ; Store the HIGHER WORD in ""*z"".
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4
  ; z
  ; --------------
  MOV ecx, DWORD PTR [esp + 4]  ; ecx@1 = z   
  MOV WORD PTR [ecx], dx        ; *z = dx@2
  
  ; "RET 4" pop "z" from the stack:
  ; The stack before the pop is:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4
  ; z
  ; --------------
  ;
  ; The new stack after the pop is:
  ; -------------- ESP + 0
  ; return address
  ; -------------- 
  ; The return address is now at the top of the stack.
  ; "RET 4" then returns to that return address.
  ; The return value is in eax@2.
  RET 4
@_dx_imul32@12 ENDP

_TEXT ENDS

END

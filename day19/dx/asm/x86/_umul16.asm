.model flat

_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://www.felixcloutier.com/x86/mul
; uint8_t __fastcall _dx_umul16(uint8_t x, uint8_t y, uint8_t *z)
@_dx_umul16@12 PROC
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
  MOV al, cl                    ; al@1 := cl@1
  MUL dl                        ; ax@2 := hi(ax@1 * dx@1), al@2 = lo(ax@1 * dx@1)

  ; Store the higher word in *z.
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4
  ; z
  ; --------------
  MOV ecx, DWORD PTR [esp + 4]  ; ecx@1 = z   
  MOV BYTE PTR [ecx], ah        ; *z = ah@2
  
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
  ; The return value is in ax@2.
  RET 4
@_dx_umul16@12 ENDP

_TEXT ENDS

END

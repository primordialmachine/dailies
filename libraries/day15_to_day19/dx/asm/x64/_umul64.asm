_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://www.felixcloutier.com/x86/mul
; uint32_t _dx_umul64(uint32_t x, uint32_t y, uint32_t *z)
_dx_umul64 PROC
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4

  ; Perform multiplication ecx * edx such that
  ; - the lower word of the product is in eax
  ; - the higher word in edx
  ; - overflow and carry flags are 1 if an overflow occurs, otherwise they are 0.
  MOV eax, ecx                  ; eax@1 := ecx@1
  MUL edx                       ; edx@2 := hi(ax@1 * dx@1), eax@2 = lo(ax@1 * dx@1)

  ; Store the higher word in *z. z is stored in r8.
  MOV DWORD PTR [r8], edx       ; *z = edx@2
  
  ; The return address is on top of the stack.
  ; "RET 0" then returns to that return address.
  ; The return value is in eax@2.
  RET 0
_dx_umul64 ENDP

_TEXT ENDS

END

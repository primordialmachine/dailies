_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://www.felixcloutier.com/x86/mul
; uint8_t _dx_umul16(uint8_t x, uint8_t y, uint8_t *z)
_dx_umul16 PROC
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4

  ; Perform multiplication ecx * edx such that
  ; - the lower word of the product is in eax
  ; - the higher word in edx
  ; - overflow and carry flags are 1 if an overflow occurs, otherwise they are 0.
  MOV al, cl                    ; al@1 := cl@1
  MUL dl                        ; ah@2 := hi(al@1 * dl@1), al@2 = lo(al@1 * dl@1)

  ; Store the higher word in *z. z is stored in r8.
  ; To circument the error `error A2218: AH, BH, CH, or DH may not be used with R8-R15, BPL, DIL, SIL, or SPL`,
  ; first move R8 into RCX. Then we can assign ah to *z.
  MOV RCX, R8                   ; rcx@1 = r8@1
  MOV BYTE PTR [RCX], ah        ; *z = dx@1
  
  ; The return address is on top of the stack.
  ; "RET 0" then returns to that return address.
  ; The return value is in al@2.
  RET 0
_dx_umul16 ENDP

_TEXT ENDS

END

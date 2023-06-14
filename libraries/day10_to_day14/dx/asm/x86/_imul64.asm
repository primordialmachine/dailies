.model flat

_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://www.felixcloutier.com/x86/imul
; int32_t __fastcall _dx_imul64(int32_t x, int32_t y, iint32_t *z)
@_dx_imul64@12 PROC
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
  MOV eax, ecx                  ; eax@1 := ecx@1
  IMUL edx                      ; edx@2 := hi(eax@1 * edx@1), eax@2 = lo(eax@1 * edx@1)

  ; Store the higher word in *z.
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4
  ; z
  ; --------------
  MOV ecx, DWORD PTR [esp + 4]  ; ecx@1 = z   
  MOV DWORD PTR [ecx], edx      ; *z = edx@2
  
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
@_dx_imul64@12 ENDP

_TEXT ENDS

END

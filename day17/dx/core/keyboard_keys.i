// Copyright (c) 2018-2023 Michael Heilmann (michaelheilmann@primordialmachine.com). All rights reserved.
DEFINE(backspace, 1, "Backspace")
DEFINE(tabulator, 2, "Tabulator")
DEFINE(clear, 3, "Clear")
DEFINE(return, 4, "Return")
DEFINE(capslock, 6, "Caps Lock")// Capital/"caps lock"
DEFINE(escape, 8, "Escape")

DEFINE(shift, 9, "Shift")
DEFINE(left_shift, 10, "Left Shift")
DEFINE(right_shift, 11, "Right Shift")

DEFINE(control, 12, "Control")
DEFINE(left_control, 13, "Left Control")
DEFINE(right_control, 14, "Right Control")

DEFINE(menu, 15, "Menu")
DEFINE(right_menu, 16, "Right Menu")
DEFINE(left_menu, 17, "Left Menu")

DEFINE(a, 18, "A")
DEFINE(b, 19, "B")
DEFINE(c, 20, "C")
DEFINE(d, 21, "D")
DEFINE(e, 22, "E")
DEFINE(f, 23, "F")
DEFINE(g, 24, "G")
DEFINE(h, 25, "H")
DEFINE(i, 26, "I")
DEFINE(j, 27, "J")
DEFINE(k, 28, "K")
DEFINE(l, 29, "L")
DEFINE(m, 30, "M")
DEFINE(n, 31, "N")
DEFINE(o, 32, "O")
DEFINE(p, 33, "P")
DEFINE(q, 34, "Q")
DEFINE(r, 35, "R")
DEFINE(s, 36, "S")
DEFINE(t, 37, "T")
DEFINE(u, 38, "U")
DEFINE(v, 39, "V")
DEFINE(w, 40, "W")
DEFINE(x, 41, "X")
DEFINE(y, 42, "Y")
DEFINE(z, 43, "Z")

DEFINE(_0,44, "0")
DEFINE(_1,45, "1")
DEFINE(_2,46, "2")
DEFINE(_3,47, "3")
DEFINE(_4,48, "4")
DEFINE(_5,49, "5")
DEFINE(_6,50, "6")
DEFINE(_7,51, "7")
DEFINE(_8,52, "8")
DEFINE(_9,53, "9")

DEFINE(up, 54, "Up")
DEFINE(down, 55, "Down")
DEFINE(left, 56," Left")
DEFINE(right, 57, "Right")

DEFINE(insert, 58, "Insert")
DEFINE(delete, 59, "Delete")
DEFINE(home, 60, "Home")
DEFINE(end, 61, "End")
DEFINE(page_up, 62, "Page Up")
DEFINE(page_down, 63, "Page Down")

DEFINE(Space,64,"Space")

DEFINE(f1, 65, "F1")
DEFINE(f2, 66, "F2")
DEFINE(f3, 67, "F3")
DEFINE(f4, 68, "F4")
DEFINE(f5, 69, "F5")
DEFINE(f6, 70, "F6")
DEFINE(f7, 71, "F7")
DEFINE(f8, 72, "F8")
DEFINE(f9, 73, "F9")
DEFINE(f10, 74, "F10")
DEFINE(f11, 75, "F11")
DEFINE(f12, 76, "F12")

DEFINE(f13, 77, "F13")
DEFINE(f14, 78, "F14")
DEFINE(f15, 79, "F15")
DEFINE(f16, 80, "F16")
DEFINE(f17, 81, "F17")
DEFINE(f18, 82, "F18")
DEFINE(f19, 83, "F19")
DEFINE(f20, 84, "F20")
DEFINE(f21, 85, "F21")
DEFINE(f22, 86, "F22")
DEFINE(f23, 87, "F23")
DEFINE(f24, 88, "F24")

// NumPad
DEFINE(numpad_0, 89, "NumPad 0")
DEFINE(numpad_1, 90, "NumPad 1")
DEFINE(numpad_2, 91, "NumPad 2")
DEFINE(numpad_3, 92, "NumPad 3")
DEFINE(numpad_4, 93, "NumPad 4")
DEFINE(numpad_5, 94, "NumPad 5")
DEFINE(numpad_6, 95, "NumPad 6")
DEFINE(numpad_7, 96, "NumPad 7")
DEFINE(numpad_8, 97, "NumPad 8")
DEFINE(numpad_9, 98, "NumPad 9")
DEFINE(num_lock, 110, "Num Lock")
DEFINE(multiply,99, "Multiply")
DEFINE(divide,100, "Divide")
DEFINE(add, 101, "Add")
DEFINE(subtract, 102, "Subtract")
DEFINE(separator, 103, "Separator") // NumPad Enter
DEFINE(decimal, 104, "Decimal") // Decimal separator on NumPad

DEFINE(left_windows, 105, "Left Windows")
DEFINE(right_windows, 106, "Right Windows")
DEFINE(applications, 107, "Applications")
DEFINE(sleep, 108, "Compute Sleep")

DEFINE(print, 109, "Print")
DEFINE(scroll, 111, "Scroll Lock")
DEFINE(pause, 112, "Pause")

DEFINE(hangul, 113, "IME Hangul mode") // IME Hangul mode: "Hangul" (Korean) writing system.
DEFINE(hanja, 114, "IME Hanja mode")   // IME Hanja mode: Chinese letters in "Hangul".
DEFINE(final, 115, "IME Final mode")   // (Seems 2 be related to "Hangul").
DEFINE(junja, 116, "IME Junja mode")   //    "   " "     "    "     "

ALIAS(hangul, kana)    // IME Kana mode: "Kana" (Japanese) writing system.
ALIAS(hanja, kanji)    // IME Kanji mode: Chinese letters in "Kana".

DEFINE(convert,117,"IME Convert")
DEFINE(nonconvert,118,"IME NonConvert")
DEFINE(accept,119,"IME Accept")
DEFINE(mode_change,120,"IME Mode Change Request")

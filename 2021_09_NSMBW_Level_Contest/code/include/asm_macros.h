// Copyright (C) 2021 RoadrunnerWMC

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef ASM_MACROS_H
#define ASM_MACROS_H


// Full register-file save/restore

#define ASM_DUMP_CONTEXT_TO_STACK \
    stwu  sp,  -0x188(sp) ; \
    stw   r0,  0x08(sp)   ; \
    stw   r3,  0x0C(sp)   ; \
    stw   r4,  0x10(sp)   ; \
    stw   r5,  0x14(sp)   ; \
    stw   r6,  0x18(sp)   ; \
    stw   r7,  0x1C(sp)   ; \
    stw   r8,  0x20(sp)   ; \
    stw   r9,  0x24(sp)   ; \
    stw   r10, 0x28(sp)   ; \
    stw   r11, 0x2C(sp)   ; \
    stw   r12, 0x30(sp)   ; \
    stw   r14, 0x34(sp)   ; \
    stw   r15, 0x38(sp)   ; \
    stw   r16, 0x3C(sp)   ; \
    stw   r17, 0x40(sp)   ; \
    stw   r18, 0x44(sp)   ; \
    stw   r19, 0x48(sp)   ; \
    stw   r20, 0x4C(sp)   ; \
    stw   r21, 0x50(sp)   ; \
    stw   r22, 0x54(sp)   ; \
    stw   r23, 0x58(sp)   ; \
    stw   r24, 0x5C(sp)   ; \
    stw   r25, 0x60(sp)   ; \
    stw   r26, 0x64(sp)   ; \
    stw   r27, 0x68(sp)   ; \
    stw   r28, 0x6C(sp)   ; \
    stw   r29, 0x70(sp)   ; \
    stw   r30, 0x74(sp)   ; \
    stw   r31, 0x78(sp)   ; \
    stfd  f0,  0x80(sp)   ; \
    stfd  f1,  0x88(sp)   ; \
    stfd  f2,  0x90(sp)   ; \
    stfd  f3,  0x98(sp)   ; \
    stfd  f4,  0xA0(sp)   ; \
    stfd  f5,  0xA8(sp)   ; \
    stfd  f6,  0xB0(sp)   ; \
    stfd  f7,  0xB8(sp)   ; \
    stfd  f8,  0xC0(sp)   ; \
    stfd  f9,  0xC8(sp)   ; \
    stfd  f10, 0xD0(sp)   ; \
    stfd  f11, 0xD8(sp)   ; \
    stfd  f12, 0xE0(sp)   ; \
    stfd  f13, 0xE8(sp)   ; \
    stfd  f14, 0xF0(sp)   ; \
    stfd  f15, 0xF8(sp)   ; \
    stfd  f16, 0x100(sp)  ; \
    stfd  f17, 0x108(sp)  ; \
    stfd  f18, 0x110(sp)  ; \
    stfd  f19, 0x118(sp)  ; \
    stfd  f20, 0x120(sp)  ; \
    stfd  f21, 0x128(sp)  ; \
    stfd  f22, 0x130(sp)  ; \
    stfd  f23, 0x138(sp)  ; \
    stfd  f24, 0x140(sp)  ; \
    stfd  f25, 0x148(sp)  ; \
    stfd  f26, 0x150(sp)  ; \
    stfd  f27, 0x158(sp)  ; \
    stfd  f28, 0x160(sp)  ; \
    stfd  f29, 0x168(sp)  ; \
    stfd  f30, 0x170(sp)  ; \
    stfd  f31, 0x178(sp)  ; \
    mfcr  r0              ; \
    stw   r0,  0x180(sp)  ; \
    mfctr r0              ; \
    stw   r0,  0x184(sp)  ; \
    mflr  r0              ; \
    stw   r0,  0x18C(sp)  ; \
    lwz   r0,  0x08(sp)

#define ASM_RESTORE_CONTEXT_FROM_STACK \
    lwz   r0,  0x180(sp) ; \
    mtcr  r0             ; \
    lwz   r0,  0x184(sp) ; \
    mtctr r0             ; \
    lwz   r0,  0x18C(sp) ; \
    mtlr  r0             ; \
    lwz   r0,  0x08(sp)  ; \
    lwz   r3,  0x0C(sp)  ; \
    lwz   r4,  0x10(sp)  ; \
    lwz   r5,  0x14(sp)  ; \
    lwz   r6,  0x18(sp)  ; \
    lwz   r7,  0x1C(sp)  ; \
    lwz   r8,  0x20(sp)  ; \
    lwz   r9,  0x24(sp)  ; \
    lwz   r10, 0x28(sp)  ; \
    lwz   r11, 0x2C(sp)  ; \
    lwz   r12, 0x30(sp)  ; \
    lwz   r14, 0x34(sp)  ; \
    lwz   r15, 0x38(sp)  ; \
    lwz   r16, 0x3C(sp)  ; \
    lwz   r17, 0x40(sp)  ; \
    lwz   r18, 0x44(sp)  ; \
    lwz   r19, 0x48(sp)  ; \
    lwz   r20, 0x4C(sp)  ; \
    lwz   r21, 0x50(sp)  ; \
    lwz   r22, 0x54(sp)  ; \
    lwz   r23, 0x58(sp)  ; \
    lwz   r24, 0x5C(sp)  ; \
    lwz   r25, 0x60(sp)  ; \
    lwz   r26, 0x64(sp)  ; \
    lwz   r27, 0x68(sp)  ; \
    lwz   r28, 0x6C(sp)  ; \
    lwz   r29, 0x70(sp)  ; \
    lwz   r30, 0x74(sp)  ; \
    lwz   r31, 0x78(sp)  ; \
    lfd   f0,  0x80(sp)  ; \
    lfd   f1,  0x88(sp)  ; \
    lfd   f2,  0x90(sp)  ; \
    lfd   f3,  0x98(sp)  ; \
    lfd   f4,  0xA0(sp)  ; \
    lfd   f5,  0xA8(sp)  ; \
    lfd   f6,  0xB0(sp)  ; \
    lfd   f7,  0xB8(sp)  ; \
    lfd   f8,  0xC0(sp)  ; \
    lfd   f9,  0xC8(sp)  ; \
    lfd   f10, 0xD0(sp)  ; \
    lfd   f11, 0xD8(sp)  ; \
    lfd   f12, 0xE0(sp)  ; \
    lfd   f13, 0xE8(sp)  ; \
    lfd   f14, 0xF0(sp)  ; \
    lfd   f15, 0xF8(sp)  ; \
    lfd   f16, 0x100(sp) ; \
    lfd   f17, 0x108(sp) ; \
    lfd   f18, 0x110(sp) ; \
    lfd   f19, 0x118(sp) ; \
    lfd   f20, 0x120(sp) ; \
    lfd   f21, 0x128(sp) ; \
    lfd   f22, 0x130(sp) ; \
    lfd   f23, 0x138(sp) ; \
    lfd   f24, 0x140(sp) ; \
    lfd   f25, 0x148(sp) ; \
    lfd   f26, 0x150(sp) ; \
    lfd   f27, 0x158(sp) ; \
    lfd   f28, 0x160(sp) ; \
    lfd   f29, 0x168(sp) ; \
    lfd   f30, 0x170(sp) ; \
    lfd   f31, 0x178(sp) ; \
    addi  sp,  sp, 0x188


// Kamek *HELPFULLY* errors out ("Function contains a return partway through")
// if there's a blr within a kmBranchDefAsm(x, y) hook.
// So I substitute a useless instruction that codewarrior would never generate
// naturally for blr, and the build script later does find-replace on it in the
// output kamekfiles.
#define BACKDOOR_BLR or r31, r31, r31  // 0x7ffffb78


#endif  // ASM_MACROS_H

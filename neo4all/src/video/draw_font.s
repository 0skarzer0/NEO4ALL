! -----------------------------------------------------------
! draw_font function for Neo4all emulator by Chui
! Oscar Orallo Pelaez
! Date: 04/10/2005
!
! Parameters:
! unsigned short *br, unsigned short *paldata, unsigned *gfxdata
! r4 = br
! r5 = paldata
! r6 = gfxdata
! -----------------------------------------------------------
.text
.balign 32
.global _draw_font
_draw_font:
mov #8,r3         ! r3 = outer loop counter (y)
add #16,r4        ! getting up br to poke

mloop:
mov.l @r6+,r1     ! r1 = mydword
mov #-32,r2       ! r2 = shifting value
mov r1,r0

iloop:
add #4,r2
shld r2,r0
and #0x0F,r0      ! r0 = paldata index

! Poking to br...
shll r0           ! r0<<=1
mov.w @(r0,r5),r7 ! r7 = paldata[index]
tst r2,r2         ! testing shifting value
mov.w r7,@-r4

bf/s iloop
mov r1,r0

dt r3
bf/s mloop
add #32,r4        ! updating br

rts
nop

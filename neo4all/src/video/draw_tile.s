! -----------------------------------------------------------
! Draw tile function
! Oscar Orallo Pelaez
! Last modified: 04/06/2005
! Parameters:
! br     (word ptr)   r4
! paldata (word ptr)  r5
! gfxdata (dword ptr)  r6
! -----------------------------------------------------------
.text
.balign 32
.global _draw_tile
_draw_tile:
	
	mov #16,r1        ! r1 = loop counter
mloop:
	
	add #16,r4        ! r4 = br + displacement
	mov.l @r6+,r3     ! r3 = mydword
	mov #-28,r7       ! r7 = shifting value
	
.iloop:
	mov r3,r0
	shld r7,r0
	and #0x0F,r0
	shll r0
	mov.w @(r0,r5),r2
	add #4,r7
	mov r3,r0
	shld r7,r0
	and #0x0F,r0
	shll r0
	mov.w @(r0,r5),r0
	extu.w r2,r2
	shll16 r0
	or r2,r0
	
	
	cmp/pz r7
	mov.l r0,@r4
	add #4,r7
	bf/s .iloop
	add #4,r4

	mov.l @r6+,r3
	add #-32,r4
	mov #-28,r7       ! r7 = shifting value

.sloop:
	mov r3,r0
	shld r7,r0
	and #0x0F,r0
	shll r0
	mov.w @(r0,r5),r2
	add #4,r7
	mov r3,r0
	shld r7,r0
	and #0x0F,r0
	shll r0
	mov.w @(r0,r5),r0
	extu.w r2,r2
	shll16 r0
	or r2,r0

	cmp/pz r7
	mov.l r0,@r4
	add #4,r7
	bf/s .sloop
	add #4,r4
	
	dt r1
	bf/s mloop
	add #16,r4
	
	rts
	nop

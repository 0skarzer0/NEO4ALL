.text


! -----------------------------------------------------------
! Draw tile
! Parametros:
! br     (word ptr)   r4
! paldata (word ptr)  r5
! gfxdata (dword ptr)  r6
! -----------------------------------------------------------
.align 4
.globl _draw_font
_draw_font:
	mov #0x0f,r2       ! masking register (r2)


	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	mov.l @r6+,r0      ! gfxdata dword (r0)

	! saving gfxdata
	mov r0,r1

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0

	
	mov #-4,r7  ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0


	
	
	
	shlr8 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	
	
	shlr16 r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-24,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0

	
	and r2,r0

	
	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	add #2,r4

	
	mov r1,r0



	mov #-28,r7   ! adjusting shifting value (to 4)
	
	shld r7,r0

	
	and r2,r0

	
	shll r0
	mov.w @(r0,r5),r0
	mov.w r0,@r4

	


	! restoring regs and returning control
	rts
	nop

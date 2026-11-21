.text


! -----------------------------------------------------------
! Draw tile
! Parametros:
! br     (word ptr)   r4
! paldata (word ptr)  r5
! gfxdata (dword ptr)  r6
! -----------------------------------------------------------
.align 4
.globl _draw_tile
_draw_tile:
	mov.l r8,@-r15    ! saving r8

	mov #0x0f,r2       ! masking register (r2)


	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #18,r4

	mov.l @r6+,r0      ! first gfxdata dword (r0)
	mov #-28,r7        ! shifting value (r7)
	mov.l @r6+,r3      ! second gfxdata dword (r3)

	! saving gfxdata
	mov r0,r1
	mov r3,r8

	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-24,r7  ! adjusting shifting value (to 24)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3


	
	mov #-20,r7  ! adjusting shifting value (to 20)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr16 r0
	shlr16 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-12,r7  ! adjusting shifting value (to 12)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	
	shlr8 r0
	shlr8 r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	mov #-4,r7   ! adjusting shifting value (to 4)
	
	
	shld r7,r0
	shld r7,r3

	
	and r2,r0
	and r2,r3

	
	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	add #2,r4

	
	mov r1,r0
	mov r8,r3



	
	and r2,r0
	and r2,r3

	
	shll r0
	mov.w @(r0,r5),r0
	shll r3
	mov.w r0,@(16,r4)
	mov r3,r0
	mov.w @(r0,r5),r3
	mov.w r3,@r4

	


	! restoring reg and returning control
	rts
	mov.l @r15+,r8

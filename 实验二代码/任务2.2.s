.section .data
buf1: .byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 
buf2: .fill 10, 1, 0 
buf3: .fill 10, 1, 0
buf4: .fill 10, 1, 0

.section .text
.global  main
main:
	mov $0,%ecx
lopa:
	mov buf1(%ecx),%al
	mov %al,buf2(%ecx)
	inc %al
	mov %al,buf3(%ecx)
	add $3,%al
	mov %al,buf4(%ecx)
	inc %ecx
	cmp $9,%ecx
	jle lopa
	mov $1,%eax
	mov $0,%ebx
	int $0x80

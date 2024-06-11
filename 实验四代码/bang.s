mov 0x0c0d90db, %eax
mov $0x804c08c, %ecx
mov %eax, (%ecx)
push 0x080493f6

mov (0x250d3ee8),%eax
mov %eax,(0x804c218)
push 0x080493f6

ret


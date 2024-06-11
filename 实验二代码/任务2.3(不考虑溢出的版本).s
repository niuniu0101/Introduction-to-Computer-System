.section .data
sdmid: .fill 9,1,0
sda: .long 1
sdb: .long 2
sdc: .long 3
sf: .long 0

.section .text
.global _start
_start:
mov $sda,%esi  # 存储a的地址
call calculate

mov $1,%eax
mov $0,%ebx
int $0x80

# 子程序
.type  calculate @function
calculate:
pushl %ebx  
    pushl %ecx  
    pushl %edx  
    movl (%esi), %ebx        # ebx = a  
    movl 4(%esi), %ecx       # ecx = b  
    movl 8(%esi), %edx       # edx = c  
      
    # 计算f = (5a + b - c + 100) / 128  
    imull $5, %ebx          # ebx = 5a  
    addl %ecx, %ebx         # ebx = 5a + b  
    subl %edx, %ebx         # ebx = 5a + b - c  
    addl $100, %ebx         # ebx = 5a + b - c + 100  
    sarl $7, %ebx           # ebx = (5a + b - c + 100) / 128  
    movl %ebx, 12(%esi)     # 保存f到sf  
      
    # 根据f的值设置eax  
    cmpl $100, %ebx  
    jl less_than_100  
    je equal_to_100  
    jg greater_than_100  
  
less_than_100:  
    movl $-1, %eax  
    jmp end_calculate  
  
equal_to_100:  
    movl $0, %eax  
    jmp end_calculate  
  
greater_than_100:  
    movl $1, %eax  
  
end_calculate:  
    popl %edx  
    popl %ecx  
    popl %ebx  
    ret

.section .data
sdmid:  .ascii "000111", "\0\0\0"     # 每组数据的流水号（可以从1开始编号） 
sda:    .long  512   # 状态信息a
sdb:    .long  -1023    # 状态信息b
sdc:    .long  1265     # 状态信息c
sf:     .long  0        # 处理结果f

        .ascii  "000222","\0\0\0" 
        .long  256809      # 状态信息a
        .long  -1023    # 状态信息b
        .long  2780     # 状态信息c
        .long  0        # 处理结果f
        
       .ascii  "000333","\0\0\0" 
        .long  2513# 状态信息a
        .long  1265    # 状态信息b
        .long  1023     # 状态信息c
        .long  0        # 处理结果f
        
       .ascii  "000444","\0\0\0" 
        .long  512   # 状态信息a
        .long  -1023    # 状态信息b
        .long  1265     # 状态信息c
        .long  0        # 处理结果f
        .ascii "555555","\0\0\0"
        .long  2513
        .long  1265
        .long  1023
        .long  0
        .ascii "666666","\0\0\0"
        .long 256800
        .long -2000
        .long 1000
        .long 0
        num = 6
midf:  .fill  9, 1, 0 
          .long 0, 0, 0, 0
          .fill  9, 1, 0
          .long 0,0,0,0
       .fill 9, 1,0
       .long 0,0,0,0
highf: .fill  9, 1, 0 
          .long 0, 0, 0, 0
          .fill  9, 1, 0
          .long 0,0,0,0
          .fill 9,1,0
          .long 0,0,0,0
lowf:   .fill  9, 1, 0 
          .long 0, 0, 0, 0
          .fill  9, 1, 0
          .long 0,0,0,0
          .fill 9,1,0
          .long 0,0,0,0
       len=25

.section    .text
.global _start
_start:
    # 这里要写
    pushl   %ebp 
    movl    %esp,   %ebp

    # -4(读id) -16(midf p) -12(highf p) -8(lowf p)
    sub     $16,     %esp
    movl    $0,     -4(%ebp)
    movl    $lowf,  -8(%ebp)
    movl    $highf, -12(%ebp)
    movl    $midf,  -16(%ebp)
    mov     $sdmid, %ebx 
L1:
    lea     9(%ebx), %esi 
    call    calculate 

    push    $len
    cmp     $0,     %eax 
    jne     L2 
    pushl   -16(%ebp)
    addl    $len,   -16(%ebp)
    jmp     L4
L2:
    jg      L3 
    pushl   -8(%ebp)
    addl    $len,   -8(%ebp)
    jmp     L4 
L3:
    pushl   -12(%ebp)
    addl    $len,   -12(%ebp)
L4:
    mov     -4(%ebp), %ecx 
    push    %ebx
    call    copy_data
L5:
    add     $len,   %ebx 
    incl    -4(%ebp)
    cmpl    $num,   -4(%ebp)
    jl      L1

    movl    %ebp,   %esp 
    popl    %ebp

    mov     $1,     %eax
    mov     $0,     %ebx
    int     $0x80
    
.type  calculate @function
calculate:

    push %ebp
    mov %esp, %ebp
    pushl %ebx  
    pushl %ecx  
    pushl %edx
    pushl %edi 
    movl (%esi), %eax        # eax = a  
    movl 4(%esi), %ebx       # ebx = b  
    movl 8(%esi), %ecx       # ecx = c
    
    # 开始计算
    mov $5 , %edi
    imull %edi
    addl %ebx, %eax
    jno next1
    addl $1, %edx
next1:
    subl %ecx, %eax
    jno next2
    subl $1, %edx
next2:
    addl $100, %eax
    jno next3
    addl $1, %edx
next3:
    # 如果高位有数字，则直接大于100,不怕高位是符号1,因为全是1的话为-1
    sarl $7, %eax
    mov %eax, %ebx
    cmp $0, %edx
    jle next4
    mov $101, %ebx
    
next4: 
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
    popl %edi 
    popl %edx  
    popl %ecx
    popl %ebx  
    
    mov %ebp, %esp
    pop %ebp
    ret
    

.type copy_data @function
copy_data:  
    # 使用堆栈传递参数: 源地址, 目标地址, 字节长度  
    push %ebp
    mov %esp, %ebp
    
    push %ecx
    push %esi
    push %edi
    push %eax
    
    movl 16(%ebp), %ecx    # ecx = 字节长度  
    movl 12(%ebp), %esi    # esi = 源地址  
    movl 8(%ebp), %edi    # edi = 目标地址  
      
    # 拷贝数据，每次4字节，剩余1字节单独拷贝  
cpy_loop:  
    cmpl $4, %ecx  
    jl copy_last_byte  
    mov  (%edi), %eax
    mov  %eax, (%esi)                 # 拷贝4字节  
    subl $4, %ecx  
    addl $4, %esi  
    addl $4, %edi  
    jmp cpy_loop  
      
copy_last_byte:   
    je end_copy  
    movb (%esi), %al
    movb %al, (%edi)                   # 拷贝1字节  


end_copy:  
    # 恢复现场
    pop %eax
    pop %edi
    pop %esi
    pop %ecx
    
    mov %ebp, %esp
    pop %ebp
    ret

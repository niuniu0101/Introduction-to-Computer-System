.section .data
buf1: .byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 
buf2: .fill 10, 1, 0 
buf3: .fill 10, 1, 0
buf4: .fill 10, 1, 0

# 定义提示信息  
prompt: .asciz "Press any key to begin!\n"  
prompt_len = . - prompt  
  
# 定义一个字符缓冲区来读取用户按键  
input_char: .byte 1

.section .text
.global  main
main:
mov  $buf1, %esi
mov  $buf2, %edi 
mov  $buf3, %ebx 
mov  $buf4, %edx 
mov  $10, %ecx 

# 保存现场
push %rax
push %rbx
push %rcx
push %rdx

# 显示提示信息
mov $4, %eax           # 系统调用号 (sys_write)  
mov $1, %ebx           # 文件描述符 (stdout)  
mov $prompt, %ecx      # 消息的地址  
mov $prompt_len, %edx # 消息的长度  
int $0x80             # 调用内核  
  
# 等待用户按键  
mov $3, %eax          # 系统调用号 (sys_read)  
mov $0, %ebx          # 文件描述符 (stdin)  
mov $input_char, %ecx # 输入缓冲区地址  
mov $1, %edx          # 读取的字节数  
int $0x80             # 调用内核  

# 恢复现场
pop %rdx
pop %rcx
pop %rbx
pop %rax

lopa:  mov  (%esi), %al 
mov  %al, (%edi) 
inc  %al
mov  %al, (%ebx)
add  $3,  %al 
mov  %al, (%edx)
inc  %esi
inc  %edi
inc  %ebx
inc  %edx
dec  %ecx
jnz  lopa 
mov  $1, %eax
movl $0, %ebx
int  $0x80




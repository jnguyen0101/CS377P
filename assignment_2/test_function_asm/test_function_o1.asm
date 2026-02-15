testFunction:
        testl   %esi, %esi
        jle     .L4
        movq    %rdi, %rax
        leal    -1(%rsi), %edx
        leaq    4(%rdi,%rdx,4), %rcx
        movl    $0, %edx
.L3:
        addl    (%rax), %edx
        addq    $4, %rax
        cmpq    %rcx, %rax
        jne     .L3
.L1:
        movl    %edx, %eax
        ret
.L4:
        movl    $0, %edx
        jmp     .L1
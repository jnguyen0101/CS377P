testFunction:
        testl   %esi, %esi
        jle     .L7
        leal    -1(%rsi), %eax
        cmpl    $3, %eax
        jbe     .L8
        movl    %esi, %edx
        movq    %rdi, %rax
        pxor    %xmm0, %xmm0
        shrl    $2, %edx
        salq    $4, %rdx
        addq    %rdi, %rdx
.L4:
        movdqu  (%rax), %xmm2
        addq    $16, %rax
        paddd   %xmm2, %xmm0
        cmpq    %rdx, %rax
        jne     .L4
        movdqa  %xmm0, %xmm1
        movl    %esi, %edx
        psrldq  $8, %xmm1
        andl    $-4, %edx
        paddd   %xmm1, %xmm0
        movdqa  %xmm0, %xmm1
        psrldq  $4, %xmm1
        paddd   %xmm1, %xmm0
        movd    %xmm0, %eax
        testb   $3, %sil
        je      .L11
.L3:
        movslq  %edx, %rcx
        addl    (%rdi,%rcx,4), %eax
        leal    1(%rdx), %ecx
        cmpl    %ecx, %esi
        jle     .L1
        movslq  %ecx, %rcx
        addl    (%rdi,%rcx,4), %eax
        leal    2(%rdx), %ecx
        cmpl    %ecx, %esi
        jle     .L1
        movslq  %ecx, %rcx
        addl    $3, %edx
        addl    (%rdi,%rcx,4), %eax
        cmpl    %edx, %esi
        jle     .L1
        movslq  %edx, %rdx
        addl    (%rdi,%rdx,4), %eax
        ret
.L7:
        xorl    %eax, %eax
.L1:
        ret
.L11:
        ret
.L8:
        xorl    %edx, %edx
        xorl    %eax, %eax
        jmp     .L3
testFunction:
        testl   %esi, %esi
        jle     .L2
        movl    $0, %eax
.L3:
        addl    $1, %eax
        cmpl    %eax, %esi
        jne     .L3
.L2:
        movl    $0, %eax
        ret
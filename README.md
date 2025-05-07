# rainfall

Introductory binary exploitation project

In the folder for each level you will find:

* **flag** - password for next level

* **README.md** - how to find the password

## Security Configuration Summary

After ssh'ing to each level we get information regarding the set permissions and protections for each level
thees information's change based on the level so keep a close eye on them

Kernel Settings

    Stack Protector: Enabled

    Strict User Copy Checks: Disabled

    /dev/mem Access: Restricted

    /dev/kmem Access: Restricted

    grsecurity/PaX: Not present

    Kernel Heap Hardening: Disabled

    ASLR: Disabled (randomize_va_space = 0)

Binary Protections

    Protection         Status

    RELRO              No RELRO

    Stack Canary       Not Present

    NX                 Enabled

    PIE                Disabled

    RPATH/RUNPATH      Not Set


Exploit Implications

    No ASLR + No PIE → Predictable memory addresses.

    No Stack Canary → Easy stack overflows.

    NX Enabled → Requires ROP/ret2libc.

    No RELRO → GOT overwrite possible.

Suggested Attacks: [Ret2libc](https://en.wikipedia.org/wiki/Return-to-libc_attack), GOT overwrite, or simple buffer overflow.

## Resources
 - https://ctf101.org/

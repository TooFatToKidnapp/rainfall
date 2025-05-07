void o() __noreturn
{
    system("/bin/sh");
    _exit(1);
    /* no return */
}

void n() __noreturn
{
    char var_20c[0x208];
    fgets(&var_20c, 0x200, __bss_start);
    printf(&var_20c);
    exit(1);
    /* no return */
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    n();
    /* no return */
}

int32_t p(char* arg1)
{
    return printf(arg1);
}

uint32_t n()
{
    char buf[0x208];
    fgets(&buf, 0x200, __bss_start);
    p(&buf);
    uint32_t m_1 = m;

    if (m_1 != 0x1025544)
        return m_1;

    return system("/bin/cat /home/user/level5/.pass");
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return n();
}

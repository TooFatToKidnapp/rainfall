uint32_t v()
{
    char var_20c[0x208]; // 520
    fgets(&var_20c, 0x200, stdin);
    printf(&var_20c);
    uint32_t m_1 = m;

    if (m_1 != 0x40)
        return m_1;

    fwrite("Wait what?!\n", 1, 0xc, stdout);
    return system("/bin/sh");
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return v();
}

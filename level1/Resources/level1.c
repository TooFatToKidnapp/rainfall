int32_t run()
{
    fwrite("Good... Wait what?\n", 1, 0x13, stdout);
    return system("/bin/sh");
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    char buf[0x40]; // 0x40 = 64
    return gets(&buf);
}

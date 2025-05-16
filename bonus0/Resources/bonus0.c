char* p(char* arg1, char* arg2)
{
    puts(arg2);
    char buf[0x1008];
    read(0, &buf, 0x1000);
    *strchr(&buf, 0xa) = 0;
    return strncpy(arg1, &buf, 0x14);
}

char* pp(char* arg1)
{
    char var_34[0x14]; // \x90 * 20
    p(&var_34, " - ");
    char var_20[0x14]; // 'A' * 9 + <shell code adder> 
    p(&var_20, " - ");
    strcpy(arg1, &var_34);
    int32_t i = 0xffffffff;
    char* edi = arg1;

    while (i)
    {
        bool cond:0_1 = 0 != *edi;
        edi = &edi[1];
        i -= 1;

        if (!cond:0_1)
            break;
    }

    *(~i - 1 + arg1) = 0x20;
    return strcat(arg1, &var_20);
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    char str[0x2a];
    pp(&str);
    puts(&str);
    return 0;
}

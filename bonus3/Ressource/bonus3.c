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

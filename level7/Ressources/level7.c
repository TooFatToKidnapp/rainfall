int32_t m()
{
    return printf("%s - %d\n", &c, time(nullptr));
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    int32_t* eax = malloc(8);
    *eax = 1;
    eax[1] = malloc(8);
    int32_t* eax_4 = malloc(8);
    *eax_4 = 2;
    eax_4[1] = malloc(8);
    strcpy(eax[1], argv[1]);
    strcpy(eax_4[1], argv[2]);
    fgets(&c, 0x44, fopen("/home/user/level8/.pass", u"r…"));
    puts("~~");
    return 0;
}

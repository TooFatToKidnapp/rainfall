
int32_t greetuser()
{
    uint32_t language_1 = language;
    int32_t str;

    if (language_1 == 1)
    {
        __builtin_memcpy(&str,
            "\x48\x79\x76\xc3\xa4\xc3\xa4\x20\x70\xc3\xa4\x69\x76\xc3\xa4\xc3\xa4\x20", 0x12);
        char var_3a_1 = 0;
    }
    else if (language_1 == 2)
        __builtin_strcpy(&str, "Goedemiddag! ");
    else if (!language_1)
        __builtin_strncpy(&str, "Hello ", 7);

    void arg_4;
    strcat(&str, &arg_4);
    return puts(&str);
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    if (argc != 3)
        return 1;

    void s;
    __builtin_memset(&s, 0, 0x4c);
    size_t var_a8_1 = 0x28;
    char* var_ac = argv[1];
    void* name = &s;
    strncpy(name, var_ac, var_a8_1);
    size_t var_a8_2 = 0x20;
    var_ac = argv[2];
    void var_38;
    name = &var_38;
    strncpy(name, var_ac, var_a8_2);
    name = "LANG";
    char* name_1 = getenv(name);

    if (name_1)
    {
        size_t var_a8_3 = 2;
        var_ac = &data_804873d;
        name = name_1;

        if (memcmp(name, var_ac, var_a8_3))
        {
            size_t var_a8_4 = 2;
            var_ac = &data_8048740;
            name = name_1;

            if (!memcmp(name, var_ac, var_a8_4))
                language = 2;
        }
        else
            language = 1;
    }

    __builtin_memcpy(&name, &s, 0x4c);
    return greetuser();
}

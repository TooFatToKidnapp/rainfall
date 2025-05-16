int main(int argc, char **argv) {
    char buffer[40];
    int value;

    value = atoi(argv[1]);
    if (value > 9) {
        return 1;
    }
    memcpy(buffer, argv[2], value * 4);
    if (value == 0x574f4c46) {
        execl("/bin/sh", "sh", NULL);
    }
    return 0;
}


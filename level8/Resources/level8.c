char *auth = 0x0;
char *service = 0x0;

int main(void)
{
    char buffer [128];
    while (1) {
        printf("%p, %p \n",auth,service);
        if (!fgets(buffer,128,stdin)) {
            return (0);
        }
        if (strncmp(buffer, "auth ", 5) == 0) {
            auth = (char *)malloc(4);
            auth[0] = 0;
            if (strlen(buffer + 5) < 0x1f) {
                strcpy(auth,buffer + 5);
            }
        }
        if (strncmp(buffer, "reset", 5) == 0) {
            free(auth);
        }
        if (strncmp(buffer, "service", 6) == 0) {
            service = strdup(buffer + 8);
        }
        if (strncmp(buffer, "login", 5) == 0) {
            if (auth[32] != '\0') {
                system("/bin/sh");
            } else {
                fwrite("Password:\n",1,10,stdout);
            }
        }
    }
}

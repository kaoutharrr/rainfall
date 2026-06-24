int main(int argc, char **argv)
{
    int n;
    char buffer[0x40];
    int check_value;

    n = atoi(argv[1]);

    if (n > 9) {
        return 1;
    }
    memcpy(buffer, argv[2], n * 4);

    if (check_value == 0x574f4c46) {
        execl("/bin/sh", "/bin/sh", NULL);
    }
    return 0;
}
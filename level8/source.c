#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *auth = NULL;
char *service = NULL;

int main(void)
{
    char input_buffer[132];

    while (1)
    {
        printf("%p, %p \n", (void*)auth, (void*)service);

        if (fgets(input_buffer, 128, stdin) == NULL)
            return 0;

        if (strncmp(input_buffer, "auth ", 5) == 0)
        {
            auth = (char *)malloc(4);
            memset(auth, 0, 4);
            
            char *auth_argument = input_buffer + 5; 
            
            if (strlen(auth_argument) < 31)
                strcpy(auth, auth_argument);
        }

        if (strncmp(input_buffer, "reset", 5) == 0)
            free(auth);

        if (strncmp(input_buffer, "service", 7) == 0)
        {
            char *service_argument = input_buffer + 7; 
            service = strdup(service_argument);
        }

        if (strncmp(input_buffer, "login", 5) == 0)
        {
            if (*(int *)(auth + 32) == 0)
                printf("Password:\n");
            else
                system("/bin/sh");
        }
    }
}

int main(int argc, char **argv) {
    struct s {
        int a;
        void *b;
    };
    
    struct s *ptr1 = malloc(8);
    ptr1->a = 1;
    ptr1->b = malloc(8); 
    
    struct s *ptr2 = malloc(8);
    ptr2->a = 2;
    ptr2->b = malloc(8); 
    
    strcpy(ptr1->b, argv[1]); 
    strcpy(ptr2->b, argv[2]);
    
    FILE *f = fopen("/home/user/level8/.pass", "r");
    fgets(c, 0x44, f);
    
    puts("~~");
    
    return 0;
}

void m() {
    printf("%s - %d\n", c, time(0));
}


















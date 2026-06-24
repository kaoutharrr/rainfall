

void p(char *dest ,char *str) {
    int i ;
     char buffer[4096]; 
    puts(str);
    read(0, buffer, 4096);
    strchr(buffer, '\n');
    strncpy(buffer, dest, 20) //strcpy doesn't add a \0 (vulnerability)
}

void pp(char *dest) {
    char src[20];        
    char v3[20];         
    p(src, " - ");     
    p(v3, " - ");       
    strcpy(dest, src);  
    strcat(dest, v3);    
}
int main(){
    char *str = pp(1);
    puts(str);
    return(0);
}
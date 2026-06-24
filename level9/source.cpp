#include <iostream>
#include <cstring>

class N {
    public :
     N(int n);
    void  setAnnotation(char* );
     N& operator+(N & other);
     N& operator-(N &other);
     ~N();
    private:
    char annotation[100];
    int size; 


}

operator+(this) {

}
void N::setAnnotation(char* str) {
    size_t len = strlen(str)
    void *dest = (char*)this + 4; // 0x8(%ebp) first param of the class is *this 
    memcpy(dest, str, len); //here is the vulnerability (no size check)
}



int main (int argc, char **argv){
    if (argc <= 1)
        exit(1);
    N *n1 = new N(5);
    N *n2 = new N(6);
    n->setAnnotation(argv[1]); 
    (n2->operator+)(*n1);
}
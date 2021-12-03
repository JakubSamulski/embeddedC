#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct CString
{
    size_t size;
    char *chars;
} CString;

CString* cstring_new(const char* str)
{
    CString *string = malloc(sizeof (CString));
    string->size = strlen(str);
    string->chars = malloc(string->size);
    strncpy(string->chars, str, string->size);
    return string;
}

size_t getSize(CString* string){
    return string->size;
}
char getChar(CString* string,int index){
    return string->chars[index];
}

char* getString(CString* string){
    return string->chars;
}

bool modify(struct CString *string,int index,char newChar){
    if(index<string->size){
        string->chars[index] = newChar;
        return true;
    }
    return false;

}

int main() {

    CString *cString = cstring_new("Alaa");
    assert(getSize(cString)== 4);
    assert(strcmp(getString(cString),"Alaa")==0);
    assert(getChar(cString,1)=='l');

    modify(cString,1,'e');
    assert(getSize(cString)==4);
    assert(strcmp(getString(cString),"Alaa")!=0);
    assert(strcmp(getString(cString),"Aeaa")==0);
}

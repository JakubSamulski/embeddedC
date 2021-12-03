#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <stdio.h>
typedef struct CString
{
    size_t size;
    char *string;
} CString;

CString* cstring_new(const char* str)
{
    CString *string = malloc(sizeof (CString));
    string->string = str;
    string->size = strlen(str);
    return string;
}

size_t getSize(CString* string){
    return string->size;
}
char getChar(CString* string,int index){
    return string->string[index];
}

char* getString(CString* string){
    return string->string;
}

// wiem ,ze nie udalo mi sie uzyskac zlozonosci o(1)
void modify(CString *string,int index,char newChar){
    char temp[getSize(string)];
    memcpy(temp,string->string, getSize(string)+1 );
    temp[index] = newChar;
    string->string = &temp[0];
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

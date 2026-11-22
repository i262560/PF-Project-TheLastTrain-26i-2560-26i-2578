
#include "util.h"
#include <iostream>
using namespace std;

// Primitive helper utilities, clamping, and screen control
int clampInt(int val, int minVal, int maxVal) {
    if (val<minVal) {
        return minVal;
    }
    if (val>maxVal) {
        return maxVal;
    }
    return val;
}
void trimWhitespace(char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || str[len-1] == '\r' || str[len-1] == '\n')) {
        len--;
        str[len] = '\0';
    }

    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n') {
        start++;
    }

    if (start > 0) {
        int i = 0;
        while (str[start + i] != '\0') {
            str[i] = str[start + i];
            i++;
        }
        str[i] = '\0';
    }
}
bool stringEquals(const char* a, const char* b){
    int i=0;
    while(a[i]!='\0') {
        if (a[i]!=b[i]) {
            return false;
        }
        i++;
    }
        return b[i]=='\0';
    
}
void printHeader(const char* title){
 cout<<"========================================"<<endl;
   cout<<" "<<title<<endl;
    cout<<"========================================"<<endl;
}
void clearScreen(){
    for (int i=0;i<30;i++) {
     cout<<endl;
    }
    
}

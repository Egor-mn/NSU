#include <fstream>
#include <iostream>

class String {
    
    char *a;
    int length;
    
public:
    String();
    String(const char *str);
    String(const String& orig);
    ~String();
    
    String& operator= (const String &str);
    String operator+ (const String str);
    String& operator+= (const String &str);
    
    const char operator[] (const int index) const;
    operator const char*() const;
    
    String changeWord(String str2, String str3);
};

void func(int length, char*& a,const char* str) {
    a = new char[length + 1];
    
    a[length] = '\0';
    for (int i = 0; i < length; i++)
        a[i] = str[i];
}

String::String() {
    length = 0;
    a = new char[1];
    a[0] = '\0';
}

String::String(const char *str) {
    length = 0;
    while (str[length] != '\0') length++;
    
    func(length, a, str);
}

String::String(const String& str) {
    this->length = str.length;
    func(length, a, str);
}

String::~String() {
    delete[] a;
}

String& String::operator+= (const String &str) {
    *this = *this + str;
    return (*this);
}

String& String::operator= (const String& str) {
    length = str.length;
    
    delete[] a;
    
    func(length, a, str);
    
    return (*this);
}

String String::operator+ (const String str) {
    
    int i = 0;
    int newLength = length + str.length;
    
    char *c;
    c = new char[newLength + 1];
    
    for (i = 0; i < length; i++)
        c[i] = a[i];
    for (i = length; i < newLength; i++)
        c[i] = str.a[i - length];
    
    c[newLength] = '\0';
    
    String newString(c);
    delete[] c;
    return newString;
}

const char String::operator[] (const int index) const {
    if (index < 0 || index >= length) throw "Error";
    return a[index];
}

String::operator const char*() const {
    return this->a;
}

String String::changeWord(String str2, String str3) {
    bool correct = false;
    String newString;
    int firstSymbol, start = 0;
    
    for (int i = 0; i < this->length; i++) {
        
        if (this->a[i] == str2.a[0] && !correct) {
            firstSymbol = i;
            correct = true;
            
            for (int m = 0; m < str2.length && m < this->length - firstSymbol; m++) {
                if (this->a[firstSymbol + m] != str2.a[m]) {
                    correct = false;
                    break;
                }
            }
        }
        
        if (correct || this->length == i + 1) {
            if (correct || this->length != i + 1) this->a[i] = '\0';
            String add(&this->a[start]);
            start = i + str2.length;
            newString += add;
        }
        
        if (correct) {
            newString += str3;
            correct = false;
            i += str2.length - 1;
        }
    }
    
    return *this = newString;
}

int main() {
    char a[1000];
    
    std::ifstream in("input.txt");
    
    in.getline(a, 1000);
    String str1(a);
    in.getline(a, 1000);
    String str2(a);
    in.getline(a, 1000);
    String str3(a);
    
    str1.changeWord(str2, str3);
    
    std::ofstream out("output.txt");
    
    out << ((const char*)str1);
    
    in.close();
    out.close();
    
    return 0;
}

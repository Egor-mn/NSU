//
//  main.cpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>

#include "myHashTable.cpp"

template <typename K, typename T>
void doAll(std::ifstream& in, std::ofstream& out) {
    
    K key;
    T value;
    int  n;
    char c;
    HashMap<K, T> hash(10);
    
    in >> n;
    while (n--) {
        in >> c;
        
        switch (c) {
            case 'A':
                in >> key >> value;
                hash.insert(key, value);
                break;
                
            case 'R':
                in >> key;
                hash.erase(key);
                break;
                
            default:
                break;
        }
    }
    out << hash.count() << ' '
        << hash.uniqueValue();
}

int main() {
    
    std::ifstream in ("input.txt");
    std::ofstream out("output.txt");
    
    char a, b;
    
    in >> a >> b;
    
    if (a == 'I' && b == 'I') doAll<int, int>                 (in, out);
    if (a == 'I' && b == 'D') doAll<int, double>              (in, out);
    if (a == 'I' && b == 'S') doAll<int, std::string>         (in, out);
    if (a == 'D' && b == 'I') doAll<double, int>              (in, out);
    if (a == 'D' && b == 'D') doAll<double, double>           (in, out);
    if (a == 'D' && b == 'S') doAll<double, std::string>      (in, out);
    if (a == 'S' && b == 'I') doAll<std::string, int>         (in, out);
    if (a == 'S' && b == 'D') doAll<std::string, double>      (in, out);
    if (a == 'S' && b == 'S') doAll<std::string, std::string> (in, out);
    
    in.close();
    out.close();
    
    return 0;
}

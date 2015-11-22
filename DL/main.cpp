//
//  main.cpp
//  DL
//
//  Created by Egor Manylov on 22.11.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "DL.h"

int main() {
    /*DLexpression *ex = new DLlet("A", new DLval(20), new DLlet("B", new DLval(30), new DLif( new DLvar("A"), new DLadd( new DLvar("B"), new DLval(3)), new DLval(10), new DLadd( new DLvar("B"), new DLval(1)))));*/
    /*DLexpression *ex = new DLlet("F", new DLfunction("arg", new DLadd( new DLvar("arg"), new DLval(1))), new DLlet("V", new DLval(-1), new DLcall( new DLvar("F"), new DLvar("V"))));*/
    DLexpression *ex = new DLadd( new DLvar("A"), new DLvar("B"));
    
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    
    ex = scan(in);
    
    try {
        ex->eval()->print(out);
    } catch (const char* ex) {
        out << ex << std::endl;
    }
    return 0;
}

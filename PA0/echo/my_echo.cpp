/**********************************
 * Author: Richard Mwaba
 * Title: my_echo.cpp
 * Date: Tuesday, January 28, 2020
 * Description: print hello world
 * 
 **********************************/
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    for(int i = 1; i < argc;i++) {
        cout << argv[i] << " ";
    }
    cout << endl;
    return 0;
}
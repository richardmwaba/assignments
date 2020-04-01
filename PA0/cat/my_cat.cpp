/*********************************************************************************
 * Author: Richard Mwaba
 * Title: my_cat.cpp
 * Date: Tuesday, January 28, 2020
 * Description: concatenate files passed in the command line and print the output
 * 
 *********************************************************************************/
#include <iostream>
#include <stdio.h>
using namespace std;

int main (int argc, char* argv[]) {
    char c; 
    if (argc >= 2) {
        for(int i = 1; i < argc; i++) {
            FILE * openFile;
            openFile = fopen(argv[i], "r");

            if (openFile != NULL) {
                while ((c = fgetc(openFile)) != EOF) {
                    cout << c;
                }
                fclose (openFile);
            }else {
                cout << "Could not open file. Ensure file exists or is readable";
            }
        }
        cout << endl;
    }else{
        cout<<"usage: "<< argv[0] <<" <filename>" << endl;
    }

    return 0;
}
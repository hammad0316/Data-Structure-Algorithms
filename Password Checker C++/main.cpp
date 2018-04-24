//
//  main.cpp
//  AhmadH_Project6_CPP
//
//  Created by Hammad Ahmad on 4/22/18.
//  Copyright © 2018 Hammad Ahmad. All rights reserved.
//

#include <iostream>
using namespace std;

class passWordChecker{
    string password;
    string secondPassword;
    int charCount[5];
    int passwordLength;
    int index;
    
public:
    passWordChecker(){
        
        for(int x = 0; x<5; x++) charCount[x] = 0;
        
    beginning:
        displayRules();
        askUser4passwd();
        passwordLength = password.length();
        
        for(int i =0; i<passwordLength; i++){
            index = checkOneChar(password[i]);
            charCount[index]++;
        }
        
        if(!checkRules()) {
            displayFail();
            goto beginning;
        }
        
        cout << "Re-enter that password please: ";
        cin >> secondPassword;
        
        if(!matching()){
            displayMatchFail();
            goto beginning;
        }
        
        displaySuccess();
        for(int x = 0; x<5; x++) charCount[x] = 0;
        
    }
    
    void displayRules(){
        cout << "1) The password length: 8-32 characters" << endl;
        cout << "2) At least one numerical, i.e., 0, 1, 2,.." << endl;
        cout << "3) At least one upper case letter, i.e., A, B, C, ..." << endl;
        cout << "4) At least one upper case letter, i.e., a, b, c, ..." << endl;
        cout << "5) At least one of the special characters, but it must be within the set:{ # $ * ( ) % & ^ } a total of eight (8) special characters. No other special characters are allowed." << endl;
    }
    
    void askUser4passwd(){
        cout << "Please enter your password" <<endl;
        cin >> password;
        while(password.length() < 8 || password.length() > 32){
            cout << "Length invalid. Please re-enter one between 8 to 32 characters: ";
            cin >> password;
        }
    }
    
    void displaySuccess(){
        cout << "Your password will be updated in 2 minutes" <<endl;
    }
    
    void displayFail(){
        cout << "Your password failed one or more password rules" <<endl;
    }
    
    void displayMatchFail(){
        cout << "Match fail..." <<endl;
    }
    
    int checkOneChar(char a){
        if(int(a) == 35 || int(a) == 36 || int(a) == 42 || int(a) == 40 || int(a) == 41 || int(a) == 37 || int(a) == 38 || int(a) == 94) return 4;
        else if(int(a) > 47 && int(a) < 58) return 1;
        else if(int(a) > 96 && int(a) < 123) return 2;
        else if(int(a) > 64 && int(a) < 91) return 3;
        else
            return 0;
    }
    
    bool checkRules(){
        return charCount[0] == 0 && charCount[1] > 0 && charCount[2] > 0 && charCount[3] > 0 && charCount[4] > 0;
    }
    
    bool matching(){
        return password == secondPassword;
    }
    
};

int main(int argc, const char * argv[]) {
    passWordChecker checkPassword = *new passWordChecker();
    return 0;
}

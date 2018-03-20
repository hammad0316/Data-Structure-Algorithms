//
//  main.cpp
//  AhmadH_Project4_CPP
//
//  Created by Hammad Ahmad on 3/17/18.
//  Copyright © 2018 Hammad Ahmad. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

ifstream inFile;
ofstream outFile1;
ofstream outFile2;

class listNode{
    friend class linkedListStack;
    friend class linkedListQueue;
    friend class RadixSort;
    
    int data;
    listNode* next;
public:
    listNode(){
        this->data = NULL;
        this->next = NULL;
    }
    listNode(int data){
        this->data = data;
        this->next = NULL;
    }
};

class linkedListStack{
    friend class RadixSort;
    
    listNode* top;
public:
    linkedListStack(){
        this->top = NULL;
    }
    
    void push(listNode* newNode){
        if(isEmpty()){
            top = newNode;
            newNode->next = NULL;
            return;
        }
        newNode->next = top;
        this->top = newNode;
        newNode = NULL;
    }
    
    listNode* pop(){
        if (isEmpty()) {
            cout << "Is empty";
            return NULL;
        }
        listNode *temp = top;
        this->top = top->next;
        return temp;
    }
    
    bool isEmpty(){
        return top == NULL;
    }
    
    void printStack(ofstream& outFile){
        if(isEmpty()){
            cout << "It's empty";
            return;
        }
        listNode* temp;
        temp = top;
        outFile << "Top --> ";
        while(temp != NULL){
            if(temp->next == NULL){
                outFile << "(" << temp->data << ", NULL) --> NULL" << endl;
                return;
            }
            outFile << "(" << temp->data << ", " << temp->next->data << ") --> ";
            temp = temp->next;
        }
        outFile << endl;
        temp = NULL;
    }
};

class linkedListQueue{
    friend class RadixSort;
    
    listNode* head;
    listNode* tail;
    
public:
    linkedListQueue(){
        listNode* dummy = new listNode();
        head = dummy;
        tail = dummy;
    }
    
    void addTail(listNode* newNode){
        if(this->isEmpty()){
            head->next = newNode;
            tail = newNode;
            tail->next = NULL;
            newNode = NULL;
            return;
        }
        tail->next = newNode;
        tail = newNode;
        tail->next = NULL;
        newNode = NULL;
    }
    
    listNode* deleteFront(){
        if(this->isEmpty()){
            cout << "Queue is empty";
            return NULL;
        }
        listNode* temp = head->next;
        head->next = head->next->next;
        temp->next = NULL;
        return temp;
    }
    
    bool isEmpty(){
        return head->next == NULL;
    }
    
    void printQueue(ofstream& outFile){
        if(isEmpty()){
            cout << "It's empty";
            return;
        }
        listNode* temp = head->next;
        outFile << "Front (index) --> ";
        while(temp != NULL){
            if(temp->next == NULL){
                outFile << "(" << temp->data << ", NULL" << ") --> NULL" << endl;
                outFile << "              Tail (index) --> (" << temp->data << ", NULL) --> NULL" <<endl;
                return;
            }
            outFile << "(" << temp->data << ", " << temp->next->data << ") --> ";
            temp = temp->next;
        }
        outFile << endl;
        temp = NULL;
    }
};

class RadixSort{
public:
    int tableSize = 10;
    linkedListQueue** hashTable;
    int data;
    int currentTable;
    int previousTable;
    int maxDigits;
    int offSet;
    int currentDigit;
    linkedListStack* stack = new linkedListStack();
    linkedListQueue* queue = new linkedListQueue();
    void firstReading(){
        int negativeNum = 0, positiveNum = 0;
        int data;
        while(inFile >> data){
            if(data < negativeNum) negativeNum = data;
            if(data > positiveNum) positiveNum = data;
        }
        offSet = abs(negativeNum);
        positiveNum = positiveNum + offSet;
        maxDigits = getMaxDigits(positiveNum);
    }
    
    void loadStack(){
        int data;
        while(inFile >> data){
            data += offSet;
            listNode* newNode = new listNode(data);
            stack->push(newNode);
        }
        outFile2 << "*** Below is the constructed stack ***" << endl;
        stack->printStack(outFile2);
    }
    
    void dumpStack(){
        while(!stack->isEmpty()){
            listNode* node = stack->pop();
            int digit = getOneDigit(*node, currentDigit);
            int hashIndex = digit;
            hashTable[currentTable][hashIndex].addTail(node);
            node = NULL;
        }
        outFile2 << endl << "The queues created in the hash table aftering dumping the stack: " <<endl;
        printTable(hashTable[currentTable], outFile2);
    }
    
    int getMaxDigits(int number){
        string s = to_string(number);
        return s.length();
    }
    
    int getOneDigit(listNode node, int current){
        int temp = node.data;
        int num = 0;
        for(int i = 0; i<= current; i++){
            num = temp % 10;
            temp = temp /10;
        }
        return num;
    }
    
    int hashFunction(int number){
        return number % tableSize;
    }
    
    void Sort(){
        hashTable = new linkedListQueue*[2];
        for(int i = 0; i<2; i++){
            hashTable[i] = new linkedListQueue[tableSize];
        }
        for(int i = 0; i<2; i++){
            for(int j = 0; j< tableSize; j++){
                hashTable[i][j].head->next = NULL;
            }
        }
        currentDigit = 0;
        currentTable = 0;
        
        dumpStack();
        
        currentTable=1;
        previousTable = 0;
        int currentQueue = 0;
        int hashIndex;
        while (currentDigit <= maxDigits) {
            currentDigit++;
            while (currentQueue <= tableSize -1) {
                while (!hashTable[previousTable][currentQueue].isEmpty()) {
                    listNode* node = hashTable[previousTable][currentQueue].deleteFront();
                    int digit = getOneDigit(*node, currentDigit);
                    hashIndex = hashFunction(digit);
                    hashTable[currentTable][hashIndex].addTail(node);
                    node = NULL;
                }
                currentQueue++;
            }
            int temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            currentQueue = 0;
            temp = NULL;
        }
        outFile2 << endl << "hashTable[currentTable]: " <<endl;
        printTable(hashTable[currentTable], outFile2);
        printTable(hashTable[currentTable], outFile1);
    }
    
    void printTable(linkedListQueue* table, ofstream& outFile){
        for(int i =0; i< 2; i++){
            for(int j=0; j<tableSize; j++){
                if(!hashTable[i][j].isEmpty()){ //isn't going in this
                    outFile << "Table [" << i << "][" << j << "]: ";
                    hashTable[i][j].printQueue(outFile);
                }
            }
        }
    }
};

int main(int argc, const char * argv[]) {
    inFile.open("/Users/hammadahmad/Desktop/323 /AhmadH_Project4_CPP/inputFile");
    outFile1.open("/Users/hammadahmad/Desktop/323 /AhmadH_Project4_CPP/outFile1");
    outFile2.open("/Users/hammadahmad/Desktop/323 /AhmadH_Project4_CPP/outFile2");
    RadixSort sort;
    sort.firstReading();
    inFile.close();
    inFile.open("/Users/hammadahmad/Desktop/323 /AhmadH_Project4_CPP/inputFile");
    sort.loadStack();
    sort.Sort();
    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}

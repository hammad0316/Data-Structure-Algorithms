//
//  main.cpp
//  AhmadH_Project2_CPP
//
//  Created by Hammad Ahmad on 2/20/18.
//  Copyright © 2018 Hammad Ahmad. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

ifstream inFile;
ofstream outFile1;
ofstream outFile2;
ofstream outFile3;
ofstream outFile4;

class listBinTreeNode {
public:
    string chStr;
    int prob;
    string code;
    listBinTreeNode* next;
    listBinTreeNode* left;
    listBinTreeNode* right;
    
    listBinTreeNode(){
        this->chStr = "";
        this->prob = 0;
        this->code = "";
        this->next = NULL;
        this->left = NULL;
        this->right = NULL;
    }
    
    listBinTreeNode(string chStr, int prob){
        this->chStr = chStr;
        this->prob = prob;
        this->code = "";
        this->next = NULL;
        this->left = NULL;
        this->right = NULL;
    }
    
    void printNode(ofstream& outFile){
        outFile << (chStr + ", ");
    }
    
    bool isLeaf(){
        return this->left == NULL && this->right == NULL;
    }
};

class HuffmanLListTree {
private:
    listBinTreeNode* listHead;
    listBinTreeNode* Root;
public:
    void ConstructLinkedList(ofstream& outFile){
        listBinTreeNode* dummy;
        dummy = new listBinTreeNode("dummy", 0);
        listHead = dummy;
        listBinTreeNode* spot;
        listBinTreeNode* newNode;
        string x;
        int y;
        while(inFile >> x){
            inFile >> y;
            newNode = new listBinTreeNode(x, y);
            spot = findSpot(listHead,newNode->prob);
            insertNewNode(spot, newNode);
            printList();
        }
    }
    
    void ConstructBinTree(ofstream& outFile1, ofstream& outFile2){
        listBinTreeNode* newNode;
        listBinTreeNode* spot;
        outFile2 << "The new nodes created in the binary tree creation process: ";
        while(listHead->next->next != NULL){
            newNode = new listBinTreeNode((listHead->next->chStr + listHead->next->next->chStr), (listHead->next->prob + listHead->next->next->prob));
            newNode->left = listHead->next;
            newNode->right = listHead->next->next;
            newNode->printNode(outFile2);
            spot = findSpot(listHead, newNode->prob);
            insertNewNode(spot, newNode);
            listHead->next = listHead->next->next->next;
            printList();
        }
        Root = listHead->next;
    }
    
    void ConstructCode(listBinTreeNode* t, string f, ofstream& outFile3){
        string code = f;
        if(t == 0){
            cout << "this is an empty string";
            return;
        } else if(t->isLeaf()){
            t->code = code;
            outFile3 << t->chStr + "          " + t->code;
            outFile3 << endl;
        } else {
            ConstructCode(t->left, code + '0', outFile3);
            ConstructCode(t->right, code + '1', outFile3);
        }
    }
    
    void preOrder(listBinTreeNode* T, ofstream& outFile4){
        if(T == NULL) return;
        T->printNode(outFile4);
        preOrder(T->left, outFile4);
        preOrder(T->right, outFile4);
    }
    
    void inOrder(listBinTreeNode* T, ofstream& outFile4){
        if(T == NULL) return;
        inOrder(T->left, outFile4);
        T->printNode(outFile4);
        inOrder(T->right, outFile4);
    }
    
    void postOrder(listBinTreeNode* T, ofstream& outFile4){
        if(T == NULL) return;
        postOrder(T->left, outFile4);
        postOrder(T->right, outFile4);
        T->printNode(outFile4);
    }
    
    listBinTreeNode* findSpot(listBinTreeNode* node ,int prob){
        listBinTreeNode* spot = node;
        while(spot->next != NULL && spot->next->prob < prob)
            if(spot->next != NULL && spot->next->prob < prob) spot = spot->next;
        return spot;
    }
    
    void insertNewNode(listBinTreeNode* spot, listBinTreeNode* newNode){
        spot = findSpot(listHead, newNode->prob);
        if(spot != NULL){
            newNode->next = spot->next;
            spot->next = newNode;
        }
    }
    
    bool isEmpty(){
        return listHead->next->next == NULL;
    }
    
    void printList(){
        outFile1 << "listHead-->";
        listBinTreeNode* current;
        current = listHead;
        while(current->next != NULL){
            outFile1 << "(" << "\"" << current->chStr + "\"" << "," << current->prob << "," << "\"" + current->next->chStr << "\"" << ")" << "-->";
            current = current->next;
        }
        outFile1 << "(" << "\"" << current->chStr + "\"" << "," << current->prob << "," << "\"" << "NULL" << "\"" << ")" << "--> NULL";
        outFile1 << endl;
    }
    
    void HuffManCode(){
        outFile1 << "The creation of this linked list using insertion sort:" <<endl;
        ConstructLinkedList(outFile1);
        outFile1 << endl << endl <<"The creation of the binary tree root using the linked list: " <<endl;
        ConstructBinTree(outFile1, outFile2);
        outFile3<< "Symbol     Code" << endl;
        ConstructCode(Root, "", outFile3);
        outFile4 << "Pre Order: ";
        preOrder(Root, outFile4);
        outFile4<< endl << "In Order: ";
        inOrder(Root, outFile4);
        outFile4<< endl << "Post Order: ";
        postOrder(Root, outFile4);
        listHead = NULL;
        inFile.close();
        outFile1.close();
        outFile2.close();
        outFile3.close();
        outFile4.close();
    }
    
};

int main(int argc, const char * argv[]) {
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    outFile3.open(argv[4]);
    outFile4.open(argv[5]);
    HuffmanLListTree construct;
    construct.HuffManCode();
}


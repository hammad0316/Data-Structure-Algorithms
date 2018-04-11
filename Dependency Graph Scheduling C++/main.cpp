//
//  main.cpp
//  AhmadH_Project5_CPP
//
//  Created by Hammad Ahmad on 4/8/18.
//  Copyright © 2018 Hammad Ahmad. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

ifstream inFile1, inFile2;
ofstream outFile;
int numNodes, ProcGiven, totalJobTime;
int* jobMarked;
int* jobTimeAry;
int* processJob;
int* processTime;

class node{
public:
    int jobId;
    int jobTime;
    node* next;
    node(){
        this->jobId = 0;
        this->jobTime = 0;
        this->next = NULL;
    }
    node(int x, int y){
        this->jobId = x;
        this->jobTime = y;
        this->next = NULL;
    }
};

class Scheduling{
    int totalJobTimes;
    int** adjacencyMatrix;
    int** scheduleTable;
    int* parentCount;
    int* jobDone;
    int adjacencyRows;
public:
    void allocateArrays(){
        scheduleTable = new int*[numNodes+1];
        for(int i = 0; i<numNodes+1; i++){
            scheduleTable[i] = new int[totalJobTime+1];
        }
        for(int a = 0; a<numNodes+1; a++){
            for(int b=0; b<totalJobTime+1; b++){
                scheduleTable[a][b] = 0;
            }
        }
        processJob = new int[numNodes+1];
        processTime = new int[numNodes+1];
        parentCount = new int[numNodes+1];
        jobDone = new int[numNodes+1];
        jobMarked = new int[numNodes+1];
        for(int j = 1; j< numNodes+1; j++){
            processJob[j] = 0;
            processTime[j] = 0;
            jobDone[j] = 0;
            jobMarked[j] = 0;
        }
        int parentTotal = 0;
        int col = 1;
        for(int f = 1; f<numNodes+1; f++){
            for(int row = 0; row<adjacencyRows; row++){
                if(adjacencyMatrix[row][col] == f) parentTotal++;
            }
            parentCount[f] = parentTotal;
            parentTotal = 0;
        }
    }
    void countLines(ifstream& inFile){
        int x;
        int lines = 0;
        while(inFile>>x){
            inFile>>x;
            lines++;
        }
        adjacencyRows = lines;
    }
    
    void loadMatrix(ifstream& inFile){
        adjacencyMatrix = new int*[adjacencyRows];
        for(int i = 0; i<adjacencyRows; i++){
            adjacencyMatrix[i] = new int[2];
        }
        int x, row = 0, col = 0;
        inFile >> x;
        while(inFile>>x){
            adjacencyMatrix[row][col++] = x;
            inFile >> x;
            adjacencyMatrix[row++][col--] = x;
        }
    }
    int computeTotalJobTimes(ifstream& inFile){
        int x;
        jobTimeAry = new int[numNodes + 1];
        for(int i = 0; i<numNodes+1; i++){
            jobTimeAry[i] = 0;
        }
        int time = 0, count = 1;
        inFile >> x;
        while(inFile >> x){
            inFile >> x;
            jobTimeAry[count++] = x;
            time += x;
        }
        return time;
    }
    int getUnMarkOrphan(){
        for(int i = 1; i<numNodes+1; i++){
            if(jobMarked[i] == 0 && parentCount[i] == 0) return i;
        }
        return -1;
    }
    void insertOpen(node* OPEN, node* newNode){
        if(OPEN->next == NULL){
            OPEN->next = newNode;
            return;
        }
        node* walker = OPEN;
        while(walker->next != NULL && walker->next->jobTime < newNode->jobTime){
            walker = walker->next;
        }
        newNode->next = walker->next;
        walker->next = newNode;
    }
    node* deleteQ(node* OPEN){
        node* temp;
        temp = OPEN->next;
        if(OPEN->next->next == NULL){
            OPEN->next = NULL;
            temp->next = NULL;
            return temp;
        }
        OPEN->next = OPEN->next->next;
        temp->next = NULL;
        return temp;
    }
    void printList(node* OPEN){
        node* current;
        current = OPEN;
        while(current != NULL){
            cout << "(" << current->jobId << ", " << current->jobTime << ") -> ";
            current = current->next;
        }
        cout << "end" <<endl;
    }
    int findProcessor(){
        for(int i = 1; i< ProcGiven+1; i++) if(processTime[i] <= 0) return i;
        return -1;
    }
    void updateTable(int availProc, int currentTime, node* newJob){
        for(int i = currentTime; i< currentTime+newJob->jobTime; i++){
            scheduleTable[availProc][i] = newJob->jobId;
        }
    }
    bool checkCycle(node* OPEN){
        return OPEN->next==NULL && !checkGraph() && checkJobs();
    }
    int findDoneJob(int ProcGiven){
        int proc;
        if(processTime[ProcGiven] == 0){
            proc = processJob[ProcGiven];
            processJob[ProcGiven] = 0;
            return proc;
        }
        return -1;
    }
    void deleteNode(int x){
        jobDone[x] = 1;
    }
    void deleteEdge(int x){
        int kidIndex;
        for(int i = 0; i <adjacencyRows; i++){
            if(adjacencyMatrix[i][0]==x){
                kidIndex = adjacencyMatrix[i][1];
                parentCount[kidIndex]--;
            }
        }
    }
    bool checkGraph(){
        for(int i=1; i<numNodes+1; i++){
            if(jobDone[i] == 0) return false;
        }
        return true;
    }
    bool checkJobs(){
        for(int i=1; i<ProcGiven+1; i++){
            if(processJob[i] > 0) return false;
        }
        return true;
    }
    void printTable(int currentTime, int ProcGiven, ofstream& outFile){
        outFile << "      ";
        for(int i = 0; i<=currentTime; i++){
            if(i==10) outFile << "-"<< i << "-";
            else
                outFile << "--" << i << "--";
        }
        outFile << endl;
        
        for(int row = 1; row < ProcGiven+1; row++){
            if(row>9) outFile << "P(" << row << ")| ";
            else
                outFile << "P(" << row << ") | ";
            for(int col = 0; col <= currentTime; col++){
                if(scheduleTable[row][col]<10) outFile << " ";
                outFile << scheduleTable[row][col] << " | ";
            }
            outFile <<endl;
        }
    }
    void printTables(){
        cout << "Job Marked: " <<endl;
        for(int m = 0; m <numNodes+1; m++) cout << m << ": " << jobMarked[m] << endl;
        
        cout << "Process Job: " <<endl;
        for(int i = 0; i <numNodes+1; i++) cout << i << ": " << processJob[i] << endl;
        
        cout << "Process Time: " <<endl;
        for(int j = 0; j <numNodes+1; j++) cout << j << ": " << processTime[j] << endl;
        
        cout << "Parent Count: " <<endl;
        for(int k = 0; k <numNodes+1; k++) cout << k << ": " << parentCount[k] << endl;
        
        cout << "Job Done: " <<endl;
        for(int l = 0; l <numNodes+1; l++) cout << l << ": " << jobDone[l] << endl;
        
        cout << "Adjacency Table: " <<endl;
        for(int row =0; row < adjacencyRows; row++){
            for(int col = 0; col < 2; col++){
                cout << adjacencyMatrix[row][col] << " ";
            }
            cout << endl;
        }
        cout << "end of printTables"<<endl;
        
    }
    
};

int main(int argc, const char * argv[]) {
    node* OPEN;
    inFile1.open(argv[1]);
    inFile2.open(argv[2]);
    outFile.open(argv[3]);
    inFile1 >> numNodes;
    Scheduling scheduling;
    scheduling.countLines(inFile1);
    inFile1.close();
    inFile1.open(argv[1]);
    scheduling.loadMatrix(inFile1);
    inFile1.close();
    totalJobTime = scheduling.computeTotalJobTimes(inFile2);
    cout << "How many processors do you want? ";
    cin >> ProcGiven;
    if(ProcGiven <= 0){
        cout << "Can't have that number";
        return 0;
    }
    if(ProcGiven > numNodes) ProcGiven = numNodes;
    scheduling.allocateArrays();
    int ProcUsed = 0;
    int currentTime = 0;
    OPEN = new node();
    int orphanNode = 0;
    
    while(!scheduling.checkGraph()){
        
        orphanNode = 0;
        while(orphanNode!=-1){
            orphanNode = scheduling.getUnMarkOrphan();
            cout << orphanNode << " orphanNode" <<endl;
            if(orphanNode!= -1){
                jobMarked[orphanNode] = 1;
                node* newNode = new node(orphanNode, jobTimeAry[orphanNode]);
                scheduling.insertOpen(OPEN, newNode);
            }
        }
        
        scheduling.printList(OPEN);
        
        while(OPEN->next != NULL && ProcUsed < ProcGiven){
            int availProc = scheduling.findProcessor();
            cout << availProc << ": ";
            if(availProc > 0){
                ProcUsed++;
                node* newJob = scheduling.deleteQ(OPEN);
                cout << newJob->jobId << " " << newJob->jobTime << endl;
                processJob[availProc] = newJob->jobId;
                processTime[availProc] = newJob->jobTime;
                scheduling.updateTable(availProc, currentTime, newJob);
            } else break;
        }
        ProcUsed = 0;
        
        if(scheduling.checkCycle(OPEN)){
            cout<< "There is a cycle in this graph" <<endl;
            return 0;
        }
        
        scheduling.printTable(currentTime, ProcGiven, outFile);
        
        currentTime++;
        cout << "Current time: " << currentTime <<endl;
        for(int p = 1; p<=ProcGiven; p++) processTime[p]--;
        int job;
        
        for(int i=1; i<ProcGiven+1; i++){
            job = scheduling.findDoneJob(i);
            cout << "Done job: " << job << " " <<endl;
            if(job!=-1){
                scheduling.deleteEdge(job);
                scheduling.deleteNode(job);
            }
        }
        cout << "OPEN: " <<endl;
        
        scheduling.printList(OPEN);
        scheduling.printTables();
        
    }
    inFile2.close();
    outFile.close();
    return 0;
}

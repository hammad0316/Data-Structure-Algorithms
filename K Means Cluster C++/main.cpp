//
//  main.cpp
//  AhmadH_Project1_CPP
//
//  Created by Hammad Ahmad on 2/8/18.
//  Copyright © 2018 Hammad Ahmad. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

ifstream inFile;
ofstream outFile;

struct xyCoord{
    int xCoord;
    int yCoord;
    xyCoord()
    {
        xCoord = -1;
        yCoord = -1;
    }
    xyCoord(int x, int y)
    {
        xCoord = x;
        yCoord = y;
    }
};

class Point{
public:
    int xCoord;
    int yCoord;
    int Label;
    Point(){
        xCoord = -1;
        yCoord = -1;
        Label = -1;
    }
    Point(int x, int y){
        xCoord = x;
        yCoord = y;
    }
};

class Kmean{
    
private:
    int numRows;
    int numCols;
    int** imgAry;
    int kCluster;
    int numPts;
    Point *ptList;
    xyCoord* kCentroids;
    int* sumCnt;
    int* sumX;
    int* sumY;
    
public:
    int changeFlag = 1;
    Kmean(int a, int b, int c, int d){
        numRows = a;
        numCols = b;
        numPts = c;
        kCluster = d;
        ptList = new Point[numPts];
        imgAry = new int*[numRows];
        for(int i =0; i<numRows; i++){
            imgAry[i] = new int[numCols];
        }
        kCentroids = new xyCoord[kCluster +1];
        sumCnt = new int[kCluster+1];
        sumX = new int[kCluster+1];
        sumY = new int[kCluster+1];
        for(int i = 0; i<kCluster+1; i++){
            sumX[i] = 0;
            sumY[i] = 0;
            sumCnt[i] = 0;
        }
        loadPtList();
        assignLabel();
        printPtList();
        while(changeFlag >0){
            mapPoint2Image();
            PrettyPrintImgAry();
            changeFlag = 0;
            computeCentroid();
            clusterCheck();
        }
    }
    
    void loadPtList(){
        int x, y;
        int counter = 0;
        while(inFile>>x){
            ptList[counter].xCoord = x;
            inFile>>y;
            ptList[counter].yCoord = y;
            counter++;
        }
    }
    
    void assignLabel(){
        int label = 1;
        for(int i=0; i<numPts; i++){
            if(label > kCluster)
                label = 1;
            ptList[i].Label = label;
            label++;
        }
    }
    void computeCentroid(){
        int ptIndex = 0;
        int label;
        while(ptIndex < numPts){
            label = ptList[ptIndex].Label;
            sumX[label] += ptList[ptIndex].xCoord;
            sumY[label] += ptList[ptIndex].yCoord;
            sumCnt[label]++;
            ptIndex++;
        }
        
        int clusterIndex = 1;
        while(clusterIndex<=kCluster){
            kCentroids[clusterIndex].xCoord = sumX[clusterIndex]/sumCnt[clusterIndex];
            kCentroids[clusterIndex].yCoord = sumY[clusterIndex]/sumCnt[clusterIndex];
            clusterIndex++;
        }
    }
    
    void clusterCheck(){
        double* dist;
        dist = new double[kCluster + 1];
        dist[0] = 0.0;
        int ptIndex = 0;
        while(ptIndex<numPts){
            int minLabel = ptList[ptIndex].Label;
            double minDist = 99999.0;
            int clusterIndex = 1;
            while(clusterIndex <= kCluster){
                dist[clusterIndex] = computeDist(ptList[ptIndex], kCentroids[clusterIndex]);
                if(dist[clusterIndex] < minDist){
                    minDist = dist[clusterIndex];
                    minLabel = clusterIndex;
                }
                clusterIndex++;
            }
            if(ptList[ptIndex].Label != minLabel){
                ptList[ptIndex].Label=minLabel;
                changeFlag++;
            }
            ptIndex++;
        }
    }
    
    double computeDist(Point x, xyCoord y){
        return sqrt( ((x.xCoord - y.xCoord)*(x.xCoord - y.xCoord)) + ((x.yCoord - y.yCoord)*(x.yCoord - y.yCoord)) );
    }
    
    void printPtList(){
        for(int i=0; i<numPts; i++){
            cout << ptList[i].xCoord << " " << ptList[i].yCoord << " " << ptList[i].Label << endl;
        }
    }
    
    void mapPoint2Image(){
        int x, y;
        for(int i=0; i<numPts; i++){
            x = ptList[i].xCoord;
            y = ptList[i].yCoord;
            imgAry[x][y] = ptList[i].Label;
        }
    }
    
    void PrettyPrintImgAry(){
        for(int i = 0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j] >0) outFile << imgAry[i][j];
                else outFile << " ";
            }
            outFile << endl;
        }
        outFile << "---------------------" << endl;
    }
};

int main(int argc, const char * argv[]) {
    int numRows, numCols, numPts, kCluster;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    inFile >> numRows;
    inFile >> numCols;
    inFile >> numPts;
    cout << "Please enter a value for K: ";
    cin >> kCluster;
    Kmean Kmean(numRows, numCols, numPts, kCluster);
    inFile.close();
    outFile.close();
}



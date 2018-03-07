import java.io.*;
import java.util.*;

public class HeapSort {
	static String inFileName;

	static String outFile1Name;

	static String outFile2Name;
	
	int rootIndex=1;
	int fatherIndex;
	int leftKidIndex;
	int rightKidIndex;
	int minKidIndex;
	int numItems;
	int heapAry[];
	
	HeapSort(){
		rootIndex = -1;
		fatherIndex = -1;
		leftKidIndex = -1;
		rightKidIndex = -1;
		minKidIndex = -1;
		numItems = -1;
	}
	HeapSort(int numItems){
		this.numItems = numItems;
		heapAry = new int[this.numItems+1];
		heapAry[0] = 0;
	}
	
	int countData() throws FileNotFoundException{
		Scanner inFile = new Scanner(new FileReader(inFileName));
		int counter = 0;
		while(inFile.hasNext()){
			inFile.next();
			counter++;
		}
		inFile.close();
		return counter;
	}
	
	void buildHeap() throws IOException{
		Scanner inFile = new Scanner(new FileReader(inFileName));
		while(inFile.hasNext()){
			insertOneDataItem(inFile.nextInt());
			int kidIndex = heapAry[0];
			bubbleUp(kidIndex);
			printHeap(outFile1Name);
		}
		inFile.close();
	}
	
	void deleteHeap() throws IOException{
		BufferedWriter outFile2 = new BufferedWriter(new FileWriter(outFile2Name, true));
		while(!isHeapEmpty()){
			outFile2.write(getRoot() + ", ");
			replaceRoot();
			bubbleDown(rootIndex);
		}
		outFile2.newLine();
		outFile2.close();
	}
	
	void insertOneDataItem(int data){
		if(heapAry[0] == numItems){
			return;
		}
		heapAry[++heapAry[0]] = data;
	}
	
	int getRoot(){
		return heapAry[rootIndex];
	}
	
	void replaceRoot(){
		heapAry[rootIndex] = heapAry[heapAry[0]];
		heapAry[heapAry[0]--] = ' ';
	}
	
	void bubbleUp(int kidIndex){
		if(isRoot(kidIndex)) return;
		fatherIndex = kidIndex / 2;
		if(heapAry[kidIndex] >= heapAry[fatherIndex]) return;
		int temp = heapAry[fatherIndex];
		heapAry[fatherIndex] = heapAry[kidIndex];
		heapAry[kidIndex] = temp;
		bubbleUp(fatherIndex);
	}
	
	void bubbleDown(int fatherIndex){
		if(isLeaf(fatherIndex)) return;
		leftKidIndex = fatherIndex * 2;
		rightKidIndex = fatherIndex * 2 + 1;
		int minIndex;
		if(leftKidIndex > heapAry[0] && rightKidIndex > heapAry[0]) return;
		else if(leftKidIndex > heapAry[0]) minIndex = rightKidIndex;
		else if(rightKidIndex > heapAry[0]) minIndex = leftKidIndex;
		else {
			minIndex = findMinKidIndex(leftKidIndex, rightKidIndex);
		}
		if(heapAry[minIndex] >= heapAry[fatherIndex]) return;
		int temp = heapAry[fatherIndex];
		heapAry[fatherIndex] = heapAry[minIndex];
		heapAry[minIndex] = temp;
		leftKidIndex = 0;
		rightKidIndex = 0;
		bubbleDown(minIndex);
	}
	
	boolean isLeaf(int index){
		return index/2 > numItems && index/2+1 > numItems;
	}
	
	boolean isRoot(int index) {
		return index == 1;
	}
	
	int findMinKidIndex(int leftKidIndex, int rightKidIndex){
		if(heapAry[leftKidIndex] >= heapAry[rightKidIndex]) return rightKidIndex;
		return leftKidIndex;
	}
	
	boolean isHeapEmpty(){
		return heapAry[0] == 0;
	}

	boolean isHeapFull(){
		return heapAry[0] == numItems;
	}
	
	void printHeap(String outFileName) throws IOException{
		BufferedWriter outFile = new BufferedWriter(new FileWriter(outFileName, true));
		for(int i = 1; i <= numItems; i++){
			outFile.write(heapAry[i] + "|");
		}
		outFile.newLine();
		outFile.close();
	}
	
	public static void main(String[] args) throws IOException{
		inFileName = args[0];
		outFile1Name = args[1];
		outFile2Name = args[2];
		HeapSort HeapSort = new HeapSort();
		int numItems;
		numItems = HeapSort.countData();
		HeapSort = new HeapSort(numItems);
		HeapSort.buildHeap();
		HeapSort.deleteHeap();
	}
}

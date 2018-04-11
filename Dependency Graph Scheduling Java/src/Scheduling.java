import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Scheduling {
	static String inFile1, inFile2, outFile;
	int totalJobTimes;
	static int[][] adjacencyMatrix;
	static int[][] scheduleTable;
	static Node OPEN;
	static int[] parentCount;
	static int[] jobDone;
	static int adjacencyRows;
	static int numNodes;
	static int ProcGiven;
	static int totalJobTime;
	static int[] jobMarked;
	static int[] jobTimeAry;
	static int[] processJob;
	static int[] processTime;

	static void allocateArrays(){
		scheduleTable = new int[numNodes+1][totalJobTime+1];
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
	static void countLines(String fileName) throws FileNotFoundException{
		Scanner inFile = new Scanner(new FileReader(fileName));
		int x;
		int lines = 0;
		x = inFile.nextInt();
		while(inFile.hasNext()){
			x = inFile.nextInt();
			x = inFile.nextInt();
			lines++;
		}
		adjacencyRows = lines;
		inFile.close();
	}

	static void loadMatrix(String fileName) throws FileNotFoundException{
		Scanner inFile = new Scanner(new FileReader(fileName));
		adjacencyMatrix = new int[adjacencyRows][2];
		int x, row = 0, col = 0;
		x = inFile.nextInt();
		while(inFile.hasNext()){
			x = inFile.nextInt();
			adjacencyMatrix[row][col++] = x;
			x = inFile.nextInt();
			adjacencyMatrix[row++][col--] = x;
		}
		inFile.close();
	}
	static int computeTotalJobTimes(String fileName) throws FileNotFoundException{
		Scanner inFile = new Scanner(new FileReader(fileName));
		int x;
		jobTimeAry = new int[numNodes + 1];
		int time = 0, count = 1;
		x = inFile.nextInt();
		while(inFile.hasNextInt()){
			x = inFile.nextInt();
			x = inFile.nextInt();
			if(count > numNodes) break;
			jobTimeAry[count++] = x;
//			System.out.print(x + " ");
			time += x;
		}
		inFile.close();
		return time;
	}
	static int getUnMarkOrphan(){
		for(int i = 1; i<numNodes+1; i++){
			if(jobMarked[i] == 0 && parentCount[i] == 0) return i;
		}
		return -1;
	}
	static void insertOpen(Node newNode){
		if(OPEN.next == null){
			OPEN.next = newNode;
			return;
		}
		Node walker = OPEN;
		while(walker.next != null && walker.next.jobTime < newNode.jobTime){
			walker = walker.next;
		}
		newNode.next = walker.next;
		walker.next = newNode;
	}
	static Node deleteQ(){
		Node temp;
		temp = OPEN.next;
		if(OPEN.next.next == null){
			OPEN.next = null;
			return temp;
		}
		OPEN.next = OPEN.next.next;
		return temp;
	}
	static void printList(){
		Node current;
		current = OPEN;
		while(current != null){
			System.out.print("(" + current.jobId + ", " + current.jobTime + ") -> ");
			current = current.next;
		}
		System.out.println("end");
	}
	static int findProcessor(){
		for(int i = 1; i< ProcGiven+1; i++) {
			if(processTime[i] <= 0) return i;
		}
		return -1;
	}
	static void updateTable(int availProc, int currentTime, Node newJob){
		for(int i = currentTime; i< currentTime+newJob.jobTime; i++){
			scheduleTable[availProc][i] = newJob.jobId;
		}
	}
	static boolean checkCycle(){
		return OPEN.next==null && !checkGraph() && checkJobs();
	}
	static int findDoneJob(int ProcGiven){
		int proc;
		if(processTime[ProcGiven] == 0){
			proc = processJob[ProcGiven];
			processJob[ProcGiven] = 0;
			return proc;
		}
		return -1;
	}
	static void deleteNode(int x){
		jobDone[x] = 1;
	}
	static void deleteEdge(int x){
		int kidIndex;
		for(int i = 0; i <adjacencyRows; i++){
			if(adjacencyMatrix[i][0]==x){
				kidIndex = adjacencyMatrix[i][1];
				parentCount[kidIndex]--;
			}
		}
	}
	static boolean checkGraph(){
		for(int i=1; i<numNodes+1; i++){
			if(jobDone[i] == 0) return false;
		}
		return true;
	}
	static boolean checkJobs(){
		for(int i=1; i<ProcGiven+1; i++){
			if(processJob[i] > 0) return false;
		}
		return true;
	}
	static void printTable(int currentTime, int ProcGiven, String outFilename) throws IOException{
		BufferedWriter outFile = new BufferedWriter(new FileWriter(outFilename, true));
		outFile.append("      ");
		for(int i = 0; i<=currentTime; i++){
			if(i==10) outFile.append("-"+ i + "-");
			else
				outFile.append("--" + i + "--");
		}
		outFile.newLine();

		for(int row = 1; row < ProcGiven+1; row++){
			if(row>9) outFile.append("P(" + row + ")| ");
			else
				outFile.append("P(" + row + ") | ");
			for(int col = 0; col <= currentTime; col++){
				if(scheduleTable[row][col]<10) outFile.append(" ");
				outFile.append(scheduleTable[row][col] + " | ");
			}
			outFile.newLine();;
		}
		outFile.close();
	}
	static void printTables(){
		System.out.println("Job Marked: ");
		for(int m = 0; m <numNodes+1; m++) System.out.println(m + ": " + jobMarked[m]);

		System.out.println("Process Job: ");
		for(int i = 0; i <numNodes+1; i++) System.out.println(i + ": " + processJob[i]);

		System.out.println("Process Time: ");
		for(int j = 0; j <numNodes+1; j++) System.out.println(j + ": " + processTime[j]);

		System.out.println("Parent Count: ");
		for(int k = 0; k <numNodes+1; k++) System.out.println(k + ": " + parentCount[k]);

		System.out.println("Job Done: ");
		for(int l = 0; l <numNodes+1; l++) System.out.println(l + ": " + jobDone[l]);

		System.out.println("Adjacency Table: ");
		for(int row =0; row < adjacencyRows; row++){
			for(int col = 0; col < 2; col++){
				System.out.print(adjacencyMatrix[row][col] + " ");
			}
			System.out.println();
		}
		System.out.println("end of printTables");

	}

	public static void main(String[] args) throws IOException{
		inFile1 = args[0];
		inFile2 = args[1];
		outFile = args[2];
		Scanner inFileOne = new Scanner(new FileReader(inFile1));
		numNodes = inFileOne.nextInt();
		//System.out.print(numNodes);
		inFileOne.close();
		countLines(inFile1);
		loadMatrix(inFile1);
		totalJobTime = computeTotalJobTimes(inFile2);
		System.out.print("How many processors do you want? ");
		Scanner scanner = new Scanner(System. in); 
		ProcGiven = scanner. nextInt();
		if(ProcGiven <= 0){
			System.out.print("Can't have that number");
			return;
		}
		if(ProcGiven > numNodes) ProcGiven = numNodes;
		allocateArrays();
		int ProcUsed = 0;
		int currentTime = 0;
		OPEN = new Node();
		int orphanNode = 0;

		while(!checkGraph()){

			orphanNode = 0;
			while(orphanNode!=-1){
				orphanNode = getUnMarkOrphan();
				System.out.println(orphanNode + " orphanNode");
				if(orphanNode!= -1){
					jobMarked[orphanNode] = 1;
					Node newNode = new Node(orphanNode, jobTimeAry[orphanNode]);
					insertOpen(newNode);
				}
			}

			printList();

			while(OPEN.next != null && ProcUsed < ProcGiven){
				int availProc = findProcessor();
				System.out.println("availProc " + availProc);
				System.out.println(availProc + ": ");
				if(availProc > 0){
					ProcUsed++;
					Node newJob = deleteQ();
					System.out.println(newJob.jobId + " " + newJob.jobTime);
					processJob[availProc] = newJob.jobId;
					processTime[availProc] = newJob.jobTime;
					updateTable(availProc, currentTime, newJob);
				} else break;
			}
			ProcUsed = 0;

			if(checkCycle()){
				System.out.println("There is a cycle in this graph");
				return;
			}

			printTable(currentTime, ProcGiven, outFile);

			currentTime++;
			System.out.println("Current time: " + currentTime);
			for(int p = 1; p<=ProcGiven; p++) processTime[p]--;
			int job;

			for(int i=1; i<ProcGiven+1; i++){
				job = findDoneJob(i);
				System.out.println("Done job: " + job + " ");
				if(job!=-1){
					deleteEdge(job);
					deleteNode(job);
				}
			}
			System.out.println("OPEN: ");

			printList();
			printTables();

		}
	}
}

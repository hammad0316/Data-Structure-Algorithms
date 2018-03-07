import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class HuffmanLListTree {
	listBinTreeNode listHead;
	listBinTreeNode Root;
	//constructor

	void ConstructLinkedList(Scanner inFile, BufferedWriter outFile1) throws IOException{
		listBinTreeNode dummy = new listBinTreeNode("dummy", 0);
		listHead = dummy;
		listBinTreeNode spot;
		listBinTreeNode newNode;
		while(inFile.hasNext()){
			newNode = new listBinTreeNode(inFile.next(), inFile.nextInt());
			spot = findSpot(listHead, newNode.prob);
			insertNewNode(spot, newNode);
			printList(outFile1);
		}
		inFile.close();
	}
	
	public void ContructBinTree(BufferedWriter outFile1, BufferedWriter outFile2) throws IOException{
		listBinTreeNode newNode;
		listBinTreeNode spot;
		outFile2.append("The new nodes created in the binary tree creation process: ");
		while(listHead.next.next!= null){
			newNode = new listBinTreeNode((listHead.next.chStr + listHead.next.next.chStr),(listHead.next.prob + listHead.next.next.prob));
			newNode.left = listHead.next;
			newNode.right = listHead.next.next;
			newNode.printNode(outFile2);
			spot = findSpot(listHead, newNode.prob);
			insertNewNode(spot, newNode);
			listHead.next = listHead.next.next.next;
			printList(outFile1);
		}
		Root = listHead.next;
	}

	void ConstructCode(listBinTreeNode Root, String f, BufferedWriter outFile3) throws IOException{
		String code = f;
		if(Root == null){
			System.out.println("this is an empty string");
			return;
		}
		else if(isLeaf(Root)){
			Root.code = code;
			outFile3.append(Root.chStr + "          " + Root.code);
			outFile3.newLine();
		} else {
			ConstructCode(Root.left, code + '0', outFile3);
			ConstructCode(Root.right, code + '1', outFile3);
		}
		
	}

	private boolean isLeaf(listBinTreeNode t) {
		return t.left == null && t.right == null;
	}

	void preOrder(listBinTreeNode T, BufferedWriter outFile4) throws IOException{
		if(T == null) return;
		T.printNode(outFile4);
		preOrder(T.left, outFile4);
		preOrder(T.right, outFile4);
	}

	void inOrder(listBinTreeNode T, BufferedWriter outFile4) throws IOException{
		if(T == null) return;
		inOrder(T.left, outFile4);
		T.printNode(outFile4);
		inOrder(T.right, outFile4);
	}

	void postOrder(listBinTreeNode T, BufferedWriter outFile4) throws IOException{
		if(T == null) return;
		postOrder(T.left, outFile4);
		postOrder(T.right, outFile4);
		T.printNode(outFile4);
	}
	
	listBinTreeNode findSpot(listBinTreeNode listHead, int prob){
		listBinTreeNode spot = listHead;
		while(spot.next != null && spot.next.prob < prob)
			if(spot.next != null && spot.next.prob < prob) spot = spot.next;
		return spot;
	}
	
	void insertNewNode(listBinTreeNode spot, listBinTreeNode newNode){
		spot = findSpot(listHead, newNode.prob);
		if(spot != null){
			newNode.next = spot.next;
			spot.next = newNode;
		}
	}
	
	boolean isEmpty(){
		return listHead.next.next == null;
	}
	
	void printList(BufferedWriter outFile) throws IOException{
		outFile.append("listHead-->");
		listBinTreeNode current = listHead;
		while(current.next != null){
			outFile.append("(" + "\"" + current.chStr + "\"" +  "," + current.prob + "," + "\"" + current.next.chStr + "\"" + ")" + "-->");
			current = current.next;
		}
		outFile.append("(" + "\"" + current.chStr + "\"" +  "," + current.prob + "," + "\"" + "NULL" + "\"" + ")" + "--> NULL");
		outFile.newLine();
	}

}

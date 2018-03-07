import java.io.BufferedWriter;
import java.io.IOException;

public class listBinTreeNode {
	String chStr;
	int prob;
	String code;
	listBinTreeNode next;
	listBinTreeNode left;
	listBinTreeNode right;
	
	public listBinTreeNode(){
		chStr = "";
		prob = 0;
		code = "";
	} 
	
	public listBinTreeNode(String chStr, int prob){
		this.chStr = chStr;
		this.prob = prob;
	}
	
	public void printNode(BufferedWriter outFile) throws IOException{
		outFile.append(this.chStr + ", ");
	}
	
}

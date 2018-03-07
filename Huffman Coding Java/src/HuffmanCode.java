import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class HuffmanCode {
	public static void main(String[] args) throws IOException{
		try {
			File file = new File(args[0]);
			BufferedWriter outFile1 = new BufferedWriter(new FileWriter(args[1], true));
			BufferedWriter outFile2 = new BufferedWriter(new FileWriter(args[2], true));
			BufferedWriter outFile3 = new BufferedWriter(new FileWriter(args[3], true));
			BufferedWriter outFile4 = new BufferedWriter(new FileWriter(args[4], true));
			Scanner inFile = new Scanner(file);
			HuffmanLListTree construct = new HuffmanLListTree();
			outFile1.append("The creation of the linked list using the insertion sort: ");
			outFile1.newLine();
			construct.ConstructLinkedList(inFile, outFile1);
			outFile1.newLine();
			outFile1.newLine();
			outFile1.append("The creation of the binary tree root using the linked list: ");
			outFile1.newLine();
			construct.ContructBinTree(outFile1, outFile2);
            outFile3.append("Symbol     Code");
			outFile3.newLine();
			construct.ConstructCode(construct.Root, "", outFile3);
			outFile4.append("Pre Order: ");
			construct.preOrder(construct.Root, outFile4);
			outFile4.newLine();
			outFile4.append("In Order: ");
			construct.inOrder(construct.Root, outFile4);
			outFile4.newLine();
			outFile4.append("Post Order: ");
			construct.postOrder(construct.Root, outFile4);
			inFile.close();
			outFile1.close();
			outFile2.close();
			outFile3.close();
			outFile4.close();
		} catch (FileNotFoundException e){
			e.printStackTrace();
		}

	}
}

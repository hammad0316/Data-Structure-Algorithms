
public class Node {
	public int jobId;
	public int jobTime;
	Node next;
	Node(){
		this.jobId = 0;
		this.jobTime = 0;
		this.next = null;
	}
	Node(int x, int y){
		this.jobId = x;
		this.jobTime = y;
		this.next = null;
	}
}

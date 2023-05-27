#include "body.h"

//A Node can either be  a Body, or represent the space of the octants around it
class Node {
	public:
	Body value;
	Node* octant[8];
	Node* parent;
	
	double centreOfMass[3], totalMass;

	void print () {
		value.print();
	}

};

class System {
	Node head;

	/*
	  Insertion of a new Body into octant 0 to 7
		- If currentNode is an external Node (has not filled in any octants and thus is itself a body) we insert the newNode and the currentNode value into octant(s) recursively
		- If currentNode is an internal Node (has filled in any octants and thus is not itself a real body) we insert the new Node into one of the octants
		- We then update the mass of the currentNode 
	*/

	void insert(Node newNode,Node currentNode,int octant){

		//Checking if the currentNode is external or not 
		bool external = false;
		for(int i = 0; i<8; i++){ if(currentNode.octant[i]){external = true;}}
		
		if(external){
			//insert the body		 
			
			//We calculate the centre of mass using each nonempty octant of the node
			double totalMass=0;
			for(int i = 0; i<8;i++){
				
				if(currentNode.octant[i]){//if a Node exists on that index
					totalMass += (*currentNode.octant[i]).value.mass;
				}
			}
		}
	}
};

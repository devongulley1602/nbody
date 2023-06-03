#include "body.h"

class System {

	public:
		Body headNode;
		double theta;



	/*
	  Insertion of a new Body into octant 0 to 7
		- If the node does not contain a body, put one there
		- If node is an external (has not filled in any octants and thus is itself a physical body) we insert the node and the body into octant(s) recursively
		- If node is an internal (has filled in any octants and thus is not itself a center for the space) we insert the body into one of the octants
/Body
		- We then update the mass of the currentNode 
		
		This will create the 3d Barnes-Hutt Tree
	*/
		Body *addBarnesHutt(Body* body, Body* node = NULL){
				//We must be clever with the use of pointers, for standardisation, all bodies will be referred by address
				Body* newNode = NULL;

				//Insertion of the body into the newNode if the node doesn't exist
				if(node == NULL){
					newNode=body;
				}else{//the node exists

					//Determine if the node is internal or external, this is the only case in the program since instantiation of the body where this value should change
					for(int i = 0 ; i< 8 ; i++){
						if((*node).children[i] != NULL) {(*node).isExternal = false;}
					}

					
					if((*node).isExternal){//ie if it has no children  
					  //then there are two bodies conflicting for the same node, they must be added as children to the current 
						*newNode = Body((*node).startingMass,(*node).position,(*node).velocity) ;//copy of the original body node placed here but without its children
						int newNodeOctant = (*node).nextOctant();
						(*node).isExternal = false;//we only need set a node to internal if it has children
						(*node).children[newNodeOctant] = addBarnesHutt(newNode,(*node).children[newNodeOctant]);
						(*node).updateCentreOfMass();
					}else{
						newNode = node;
					}

					(*newNode).mass += (*body).mass;//the mass of the system below this node increases by the body being added
					int octant = (*body).nextOctant();//determine where the octant that the new body at this depth should be inserted
					(*newNode).children[octant] = addBarnesHutt(body,(*node).children[octant]); //adds the body to be a child of the node

				}	
				
				return newNode;
		};
	

		//recursively move down the tree and for each body, sum together its relative attractions to each neighbour (which may be internal or external)
		void calculateForces(Body body, Body node,double (&F)[3]){
			double F_ab[3] = {0,0,0};

			//Calculate the force exerted by the node and store the result by reference in F_ab so long as it's external 
			if(node.isExternal || (node.Domain()*node.Domain()/node.distanceSquaredTo(body)) < theta ){
				node.forceOn(body,F_ab);
			}else{



			}

			
			for (int i = 0 ; i < 3 ; i++){
				F[i] += F_ab[i];
			}
		}

		void Verlet(){

		}
	
	

};

/* system.h
 *
 * Author: Devon Gulley
 *
 * Completion Date: 05-06-2023
 *
 * To assemble the system of body.h::Body into a Barnes-Hutt implementation using the law of gravitation between point masses.
 * Clusters close masses as a body with its centre of mass visible to distant bodies to reduce computation pairs.
 *
 * Constructor System:
 * 	Creates an instance of a system 
 *
 * Public Member Functions:
 *
 * 	addBarnesHutt:
 * 		Builds the Barnes-Hutt tree one body at a time and clusters bodies based off of their properties in body.h
 * 		Accepts the body to insert, as well as along which node it begins the search for insertion into.
 * 		Returns the new node it has added to the sytsem
 * 		
 *	calculateForces:
 *		For one instant in time, calculates the net force acting on each body in the system
 *		Accepts the body in which it is calculating the net force on,and the top node to recurse down for
 *		Accepts and returns by reference the net force F on the body
 *
 */


#include "body.h"

class System {

	public:
		/* 
		 * The Barnes-Hutt theta criterion set by the system (typically 0.5) 
		 * The ratio of the domain of an internal node to the distance it is from a body	
		 */
		double theta;

		Body* head;
		
		System (double _theta=0.5){
			theta = _theta;
			head = new Body;
		};

		void add(Body &body){
			addBarnesHutt(&body,head);
		}
		/*
		 * addBarnesHutt:
		 *	Builds the Barnes-Hutt tree to represent point body or centre of mass clusters of point bodies
		 *
		 * @param body The body being inserted into the Barnes-Hutt tree.
		 * @param node The location in the tree where the body is intended to be inserted (which is itself of type Body as well.)
	   *
	   * Insertion of a new Body into octant 0 to 7
	   * - If the node does not contain a body, put one there
	   * - If node is an external (has not filled in any octants and thus is itself a physical body) we insert the node and the body into octant(s) recursively
	   * - If node is an internal (has filled in any octants and thus is not itself a center for the space) we insert the body into one of the octants/Body
	   * - We then update the mass of the currentNode 
	   *	
	   *	This will create the 3d Barnes-Hutt tree each calculation of the forces during an iteration of the Verlet algorithm.
	   */
		Body* addBarnesHutt(Body* body, Body* node){
			cout <<"\n\n---------------------------------------\nbody: "<<body << "\n" ;
			(*body).print() ;
			cout << "node: "<< node<< "\n";
 			//(*node).print();	
			Body *newNode = new Body;//the default return if the node is null


			if(node!=NULL) { //the node exists already
				newNode = node;
				//Determine if the node is internal or external, this is the only case in the program since instantiation, addition of a subnode, or a reset of the Barnes Hutt tree where this value should change
				(*node).isExternal = true;
				for(int i = 0 ; i< 8 ; i++){
					if((*node).subnode[i] != NULL) {(*node).isExternal = false;}
				}

				

					//If the node is external (has no subnode) then there are two bodies conflicting for the same node, they must be added as subnode to the current 
				if((*node).isExternal && node != head){

					Body superNode = *node;
					newNode = &superNode;
					int nextOctant = (*node).nextOctant();
					
					cout<<"\nnode is external and also a body, creating new node and adding that to octant: "<< nextOctant<< " as well\n";
					(*newNode).subnode[nextOctant] = addBarnesHutt(node,superNode.subnode[nextOctant]);
				}

				(*newNode).mass += (*body).mass;//the mass of the system below this node increases by the body being added
				int octant = (*body).nextOctant();//determine where the octant that the new body at this depth should be inserted

				cout <<"\nadding body "<<body<<" as subnode into octant: "<<octant<<" on node "<<  newNode << "\n";
				(*newNode).subnode[octant] = addBarnesHutt(body,(*newNode).subnode[octant]); //adds the body to be a subnode of the node
				(*newNode).updateCentreOfMass();
				//newNode = (*newNode).subnode[octant];

				
			}
			cout<<"\n==========RETURNS==========\n"<<newNode<<"\n==========================\n\n";
			return newNode;
		};
		

		/*
		 * Calculation of the forces of all subnodes starting at node onto the body
		 *
		 * @param body The body whose forces are being calculated for.
		 * @param node The node whose subnodes contribute to the forces acting on body.
		 * @param F    The reference value of the input force being changed through the forces algorithm.
		 *
		 * Recursively moves down the tree and sums together the force on a given body contributed by every relevant node in the system.
		 * - For nodes that are external, the force it exerts on the body will be summed and contributed
		 * - For nodes that are internal but satisfy the theta criterion and are sufficiently far from a given body, the same will occur
		 * - Nodes that are internal, but too close to the body in question, its subnodes will each recurse on this function to contribute to the force
		 */
		void calculateForces(Body body, Body node,double (&F)[3]){
			
			//Calculate the force exerted below the node and store the result by reference in F_ab 
			double F_ab[3] = {0,0,0};

			//The node's position can be used if it's external OR it's internal but sufficiently far from the body (the theta criterion)
			if(node.isExternal ||( (node.Domain()*node.Domain()< theta*node.distanceSquaredTo(body))) ){
				node.forceOn(body,F_ab);
			}else{//The default recursive body force pair computation
				for (int i = 0 ; i < 8 ; i++){
					if (node.subnode[i] != NULL){//if anything can be parallelised easily, it's this
						(*node.subnode[i]).forceOn(body,F_ab);//recursively determine the contribution of the force from each subnode onto the body
					}
				}
			}

			//To return the sum of the forces from each contribution
			for (int i = 0 ; i < 3 ; i++){
				F[i] += F_ab[i];
			}
		}
};

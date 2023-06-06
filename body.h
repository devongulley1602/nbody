/* body.h
 *
 * Author: Devon Gulley
 *
 * Completion Date: 05-06-2023
 *
 * A node data element to represent a physical point mass in a 3-d space
 * Can also be used to represent a centre of mass for its subnodes to be implemented in <system.h>::System
 *
 * Constructor Body:
 * 	Creates a point object based off of an initial mass, position, and velocity
 *
 *
 * Public Member Functions:
 * 
 * initialiseBody:
 * 	To be called by the constructor, or each time the Barnes-Hutt tree is rebuilt to unstack the body from the tree
 * 	Accepts the mass, new position, new velocity,unlinks any subnodes, and resets the domain of the Body
 *
 * kineticEnergy:
 * 	Returns the value of the kinetic energy based off of the current velocity of the body
 *
 * distanceSquaredTo:
 * 	Accepts another body as input
 * 	Returns the square of the Euclidean distance in 3d space between the two bodies
 *
 * Domain:
 * 	Returns the value of the current domain size which a body occupies
 * 
 * updateCentreOfMass:
 *  Changes the position of an internal node to match the centre of mass of all its subnodes
 *
 * forceOn:
 *  Calculates the force between a node and another body
 *  Accepts the body, and the force being modified by reference 
 *
 * nextOctant:
 * 	Returns the domain size of the octant that a given body is in
 *
 * resetTo0th:
 * 	Sets all values of a body to its initial condition as defined in initialiseBody
 *
 * print:
 * Sends key statistics to std::cout regarding properties of the body
 */


#include <iostream>
#include <cmath>
using namespace std;

class Body {

	protected:
		//The width of the octant in whcih a particular body occupies
		double domain;
	public:
		double startingMass, mass,position[3],velocity[3]; //Properties of the object
		Body *subnode[8];//subnodes on the Barnes-Hutt tree structure for each octant
		double cutoffDistance;//proximity in which the force between bodies is set 0 to avoid singularities
		bool isExternal; //true when the body is not a representation of other bodies in space


		/*
		 * Constructor for the Body class
		 *
		 * The initial traits of the body being initialised
		 *
		 * @params _mass
		 * @params _position[3]
		 * @params _velocity[3]
		 *
		 */
		Body (double _mass=0, double *_position=NULL,double *_velocity=NULL) {
			initialiseBody(_mass,_position,_velocity);		
		}


   	/*
		 * initialiseBody:
		 *
		 * @params _mass
		 * @params _position[3]
		 * @params _velocity[3]
     *
		 * Sets the initial values of the body while maintaining its current state in the sytem such
		 * Should be called each time the force is updated or when the system tree is rebuilt
		 */
		void initialiseBody (double _mass, double *_position,double *_velocity) {
			cutoffDistance = 0.001;
			domain = 1.000000 ;//the side length of the default octant (i.e. 1.0)
			startingMass = _mass;
			mass = _mass;
			
			isExternal = (_position && _velocity); //on initialisation internal nodes will not have an initial velocity or position

			if(isExternal){//internal nodes are fine to have garbage values for position and velocity
				for (int i = 0 ; i<3;i++){
					position[i] = _position[i];
					velocity[i] = _velocity[i];
				}
			}
			
			//On initialisation/reset of the system tree, all subnodes of a given body are unlinked in memory
			for(int i = 0 ; i < 8 ; i++){
				subnode[i] = NULL;
			}
		}


		/*
		 * kineticEnergy:
		 * 	Returns the non-relativistic kinetic energy of the body in non-scaled units
		 */
		double kineticEngergy () {
			double v_squared = 0;
			for (int i = 0; i<3;i++){
				v_squared += (velocity[i])*(velocity[i]);
			}
			return 0.5 * mass * v_squared;
		}

	
		/* 
		 * distanceSquaredTo:
		 * 	Returns the Euclidean distance to another body b
		 *
		 * @params b
		 */
		double distanceSquaredTo(Body b){
			double R_sq = 0;
			//Computing Pythagoras' theorem for each axis
			for (int i = 0;i<3;i++){
				R_sq += pow(position[i] - b.position[i],2);
			}
			return R_sq;
		}

		
	  // @property Domain 
		double Domain(){
			return domain;
		}

		
/*
 * updateCentreOfMass:
 * 
 * Returns the calculation for the centre of mass of all of a node's subnodes 
 * The centre of mass is calculated from: (Sum of each mass * each position vector)/ total mass
 * As soon as a body has subnodes, it is no longer a body, but a representation of the centre of mass of its subnode in space
 *
 */ 
		void updateCentreOfMass(){
			double com[3] = {position[0],position[1],position[2]};
			
			if(!isExternal){
				//the sum of each mass * each position vector
				for( int i = 0; i<8; i++){
					if(subnode[i] != NULL){
						for (int j = 0 ; j<3; j++){
							com[j] += ((*subnode[i]).mass)*(*subnode[i]).position[j];		
						}
					}
				}
			
				//divided by the overall mass
				for (int i = 0 ; i<3; i++){
					com[i] /= mass;
					position[i] = com[i];
				}
			}


		}


		/*
		 * forceOn:
		 * 	Calculates the force between this node and Body b based off of law of gravitation for point masses
		 * 	Stores the result of the force from this into the input parameter F[3] by reference
		 * 	The order of the subtraction of their positions designates the direction as pointing away from b
		 *
		 * @params b
		 * @params F[3]	
		 */ 
		void forceOn(Body b,double (&F)[3]){//a proportional force which the program driver can scale appropriately
			for (int i = 0; i<3; i++){
				//the force projection onto each axis
				F[i] =((position[i]-b.position[i])*mass*b.mass)/(distanceSquaredTo(b)*pow(distanceSquaredTo(b),0.5));
			}
		}
		

		/* 
		 * nextOctant:
		 * 	Determines the position of the node into the octant the next level down when subdividing along the system tree
		 * 	Divides the domain of the body in half to set it as a subnode of its current
		 * 	Returns the index of the appropriate octant as octantIndex
		 */
		double nextOctant () {
			//Divide the domain in half on each axis, positions outside this will be placed in the appropriate octant
			domain *= 0.5;

			int octantIndex = 0;

			//In each dimension i, we determine the octant in which the particle is in based off the size of the domain
			for (int i = 0; i<3; i++){
				double relativePosition = (position[i])/domain;
				octantIndex += (relativePosition>domain) *pow(2,i); //the octant from 0 to 7 corresponding to if the relativePosition is greater or less than 1 on half of the new domain  	
			}
			return octantIndex;
		}
		
		
		/*
		 * resetTo0th:
		 * 	Resets the body to its initial conditions with its updated position, and velocity by calling initialiseBody 
		 */
		void resetTo0th () {
			initialiseBody(startingMass,position,velocity);
		}


		/*
		 * print:
		 *  Sends to std::cout a printout of properties including mass, position[3] values, and velocity[3] values 
		 */
		void print() {
			std::cout << "Mass: "<< mass << "\nPosition: ("<<position[0]<<","<< position[1]<< "," << position[2] << ")\nVelocity: ("<<velocity[0]<<","<< velocity[1]<< "," << velocity[2] << ")\n\n";

		}
};


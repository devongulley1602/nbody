#include <iostream>
#include <cmath>
#include<array>
#include <bits/stdc++.h>
using namespace std;

class Body {

	protected:
		double main, domain;
	public:
		//Properties of the object
		double startingMass, mass,position[3],velocity[3];
		Body *children[8];
		static double cutoffDistance;
		bool isExternal;


		//Constructor
		Body (double _mass=0, double _position[3]={},double _velocity[3]={}) {
			initializeBody(_mass,_position,_velocity);		
		}
		
		void initializeBody (double _mass, double _position[3],double _velocity[3]) {
			cutoffDistance = 0.0001;
			domain = 1.000000 ;//the side length of the default quadrant (i.e. 1.0)
			startingMass = _mass;
			mass = _mass;
			for (int i = 0 ; i<3;i++){
				position[i] = _position[i];
				velocity[i] = _velocity[i];
			}
			
			//Until assigned, the children of a particular body mustn't exist in memory and thus the body is external on the Barnes-Hutt tree
			for(int i = 0 ; i < 8 ; i++){
				children[i] = NULL;
			}
			isExternal = true;
		}




		double kineticEngergy () {
			double v_squared = 0;
			for (int i = 0; i<3;i++){
				v_squared += (velocity[i])*(velocity[i]);
			}
			return 0.5 * mass * v_squared;
		}

		



		double distanceSquaredTo(Body b){
			double R_sq = 0;
			//Computing Pythagoras' theorem for each axis
			for (int i = 0;i<3;i++){
				R_sq += pow(2,position[i] - b.position[i]);
			}
			return R_sq;
		}

		double Domain(){
			return domain;
		}

		
		//The centre of mass is calculated from: (Sum of each mass * each position vector)/ total mass
		//As soon as a body has child nodes, it is no longer a body, but a representation of the centre of mass of its children in space, therefore we must
		void updateCentreOfMass(){
			double com[3] = {position[0],position[1],position[2]};
			
			if(!isExternal){
				//the sum of each mass * each position vector
				for( int i = 0; i<8; i++){
					if(children[i] != NULL){
						for (int j = 0 ; j<3; j++){
							com[j] += ((*children[i]).mass)*(*children[i]).position[j];		
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


		//Place an input force to be modified by reference directly (i.e usage in the system could look like myBody.forceOn(otherBody,theForceUsedForCalculationsBetweenThem) )
		void forceOn(Body b,double (&F)[3]){//a proportional force which the program driver can scale appropriately

			for (int i = 0; i<3; i++){
				F[i] =(b.position[i]-position[i])*mass*b.mass/(distanceSquaredTo(b)*pow(0.5,distanceSquaredTo(b)));//the force projection onto each axis
			}
		}
		

		//Places the node into the octant in the next level down
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
		

		void resetTo0th () {
			initializeBody(startingMass,position,velocity);
		}


		void print() {
			std::cout << "Mass: "<< mass << "\nPosition: ("<<position[0]<<","<< position[1]<< "," << position[2] << ")\nVelocity: ("<<velocity[0]<<","<< velocity[1]<< "," << velocity[2] << ")\n\n";

		}
};


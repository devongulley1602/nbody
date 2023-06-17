/* driver.cpp
 *
 * Author: Devon Gulley
 *
 * Completion Date: (ongoing)
 *
 * Creates an instance of a system of bodies and computes its time evolution using Verlet algorithm approximations based off of the simulation parameters
 * All position values should be contained within the space (0,0,0) to (1,1,1) at all times for the duration of the run
 */

#include "system.h"
#include <stdlib.h> 

double ranDouble(){
	return (double) rand()/RAND_MAX;
}

int main (){

	
	double pos[3] = {0.1,0.1,0.1};
	double vel[3] = {0.05,0.05,0.05};

	Body b = Body(1,pos,vel);
	Body b1 = Body(2,pos,vel);

	b.position[0] = 0.1;
	b.position[1] = 0.1;
	b.position[2] = 0.9;
	
	b1.position[0] = 0.9;
	b1.position[1] = 0.9;
	b1.position[2] = 0.9;

	
	System s = System();
	Body* h;
	
	h = s.head;
	cout<<"\nHead: "<< h <<"\n";		
	cout<<"\nADDING B\n";
	s.add(b);
	cout<<"\nADDING B1\n";
	s.add(b1);
	
	cout << "\nprinting subnodes of h\n";
	for(int i = 0 ; i<8; i++){
		if((*h).subnode[i]!= NULL){
			(*(*h).subnode[i]).print();	
		}
	}


/*	
	Body* internalNode = (*h).subnode[7];
	cout << "\nh_[7] ("<<internalNode <<") subnodes:\n";
	for(int i = 0; i<8; i++){
		if((*internalNode).subnode[i] != NULL){
			(*(*internalNode).subnode[i]).print();
		}
	}
*/


/*	s.addBarnesHutt(&b1);
	b.print();
	b1.print();
	double F_net[3] = {0,0,0};

	b.forceOn(b1,F_net); 
	cout << "d^2 " << b.distanceSquaredTo(b1) << "\n";
	cout<<"F " << F_net[0]<< " " << F_net[1] << " " << F_net[2];
*/	

/*	
	srand(4);//the random seed used for testing
	
	Body bodies[10];
	
	double initialRadius = 0.1;//the initial distribution of bodies in the system
	double initialMaxVelocity = 0.001;
	int numBodies = 10;
	
	for(int i = 0; i<333; i++){
		double pos[3] = {ranDouble()*initialRadius,ranDouble()*initialRadius,ranDouble()*initialRadius};
		double vel[3] = {ranDouble()*initialMaxVelocity,ranDouble()*initialMaxVelocity,ranDouble()*initialMaxVelocity};
		bodies[i] = Body(1.0,pos,vel);
		//bodies[i].print();
		//cout << &bodies[i] <<"\n";
		
		s.addBarnesHutt(&bodies[i],&s.head);
		cout << "_________________\nHEAD: " << &s.head << "\n_________________\n";
		
	}

*/
	
	cout<<"\nExiting\n";
	return 0;
};


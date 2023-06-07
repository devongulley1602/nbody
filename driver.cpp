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

	
	double pos[3] = {0.5,0.5,0.5};
	double vel[3] = {0.05,0.05,0.05};

	Body b = Body(1,pos,vel);
	Body b1 = Body(2,pos,vel);
	
	b1.position[1] = 0.4;
	b1.position[2] = 0.4;

	
	System s = System();
	Body* h = &s.head;
(*h).print();
	s.addBarnesHutt(&b,h);
	s.addBarnesHutt(&b1,h);
	
	
	cout << "\nprinting subnodes of h\n";
	for(int i = 0 ; i<8; i++){
		if((*h).subnode[i]!= NULL){
			(*s.head.subnode[i]).print();	
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


	cout <<"\n\n\nSanity check (head domain should  == 1) head domain: " << (*h).Domain();
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
	

	return 0;
};


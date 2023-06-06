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


int main (){
	double pos[3] = {0.5,0.5,0.5};
	double vel[3] = {0.05,0.05,0.05};

	Body b = Body(1,pos,vel);
	Body b1 = Body(2,pos,vel);
	
	b1.position[1] = 0.4;
	b1.position[2] = 0.4;

	System s = System();
	s.addBarnesHutt(&b);
	s.addBarnesHutt(&b1);
	b.print();
	b1.print();
	
	double F_net[3] = {0,0,0};

	b.forceOn(b1,F_net); 
	cout << "d^2 " << b.distanceSquaredTo(b1) << "\n";
	cout<<"F " << F_net[0]<< " " << F_net[1] << " " << F_net[2];
	//Body c = Body(1);
  //System s;
	//s.headNode = b;
	//s.addBarnesHutt(&c);
}


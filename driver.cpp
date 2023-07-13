/* driver.cpp
 *
 * Author: Devon Gulley
 *
 * Completion Date: (ongoing)
 *
 * Creates an instance of a system of bodies and computes its time evolution using Verlet algorithm approximations based off of the simulation parameters
 * All position values should be contained within the space (0,0,0) to (1,1,1) at all times for the duration of the run
 *
 */
#include <cmath>
#include "system.h"
#include <stdlib.h> 
#include <cstdio>

double ranDouble(){
	return (double) rand()/RAND_MAX;
}

double initialRadiusSample(){
	//B is the maximum cutoff of the distribution, r_i is the inner radius where 1/e particles would be outside of without cutoff
	//-ln|u*r_i|*r_i where random variable u is an element of (exp(-B/r_i)/r_i,1/r_i)	
	double B = 0.25;
	double r_i = 0.05;

	double u = exp(-1*B/r_i) + ranDouble()/r_i;
	return -1*log(u*r_i)*r_i;
}

int main (){
	//For back end logging of system properties
	freopen( "recentRun.log", "w", stdout );

	/*
	 * Create the system and populate it with the initial position and velocities of each body for a given random seed.
	 * The position will be determined by  Monte-Carlo sampling for the appropriate distribution chosen
	 * The initial velocity of a given body will begin as uniformly circular about the centre of mass of the system  
	 */ 
	

	//Seeding the random state of the system
	srand(0);
	
	//creating the initial state of the system
	int numBodies = 1000;

	//creating an array to keep track of the bodies in memory
	Body* bodies = new Body[numBodies];

	//uniform masssed particles
	double m = 1.0;
	
	//the total mass of the system
	double M = m*numBodies;

	double G = 6.6743*pow(10,-11);

	for (int i = 0; i< numBodies ; i++){
		
		//Creating the coordinates of the distribution (r,theta,phi) of a given body 
		double r = initialRadiusSample();
		double theta = M_PI*ranDouble();//azimuthal angle
		double phi = 2*M_PI*ranDouble();//polar angle
		
		//Transforming from spherical to Cartesian coordinates such that r=0 corresponds to (x,y,z)=(0.5,0.5,0.5)
		double x = r*sin(theta)*cos(phi) + 0.5;
		double y = r*sin(theta)*sin(phi) + 0.5;
		double z = r*cos(theta) + 0.5;

		//Creating the initial velocities of the distribution solely in the positive theta direction
		//the magnitude of the velocity points along theta and is proportional to the square root of the radius
		double v = sqrt(2*G*M/r);
		
		//distributing this for x and y velocities (0 initially in z direction for this system)
		double v_x = -v*cos(theta);
		double v_y = v*sin(theta);
		double v_z = 0;	

		//The parameters of the body have been set, now to apply them to the object and add them to the array
		double pos[3] = {x,y,z};
		double vel[3] = {v_x,v_y,v_z};
	
		Body b = Body(m,pos,vel);
		bodies[i] = b;	
	}


	//With each body and its properties created, they can now be added to the system	
	System s = System();

	for(int i = 0; i<numBodies;i++){
		s.add(bodies[i]);
	}
	
	
	//Change stdout back to terminal
	freopen ("/dev/tty", "a", stdout);
	
	/*
	 * The system is now created and populated with the bodies
	 * It can now be evolved in time with the Verlet alogorithm
	 *
	 */
	cout << "\nSolving...\n";
	
	double dt = 0.001; //the time step in which the system advances by
	int maxSteps = 1000;//the total number of steps that the driver will simulate for
	double progressBar = 0;
	
	for (int i = 0; i<maxSteps; i++){//the ordering of the bodies in the array may have an effect on the system evolution
		
		//Calulation of the force on each body j, then adjusting its velocity and position
		for (int j = 0; j<numBodies ; j++){
			//Calculate the force exerted on body[j]
			double F[3] = {0,0,0};
			s.calculateForces(bodies[j],*s.head,F);
			//The force F on body j has been solved for, now to calculate the motion 
			
			double m = bodies[j].mass;
			double a[3] = {F[0]/m,F[1]/m,F[2]/m};//the acceleration of the body
			

			//Discrete kinematic approximations
			//v+=a*dt
			//x+=v*dt
			for( int k = 0; k < 3; k++){
				bodies[j].velocity[k] += a[k]*dt;
				bodies[j].position[k] += bodies[j].velocity[k]*dt;
			}

		}
		
		//Keeping track of the progress of the program solving this 
		double progress = (((double)(i+1))/maxSteps)*100;
		if (progressBar < progress ){
			system("clear");
			progressBar += 4;
			double width=73;
			string progressString = " [";
			if(progressBar < 100){
				for (int p = 0; p< width; p++){
					if(p<(progress/100)*width){
						progressString +="#";
					}else{
						progressString += " ";
					}
				}
				progressString += "]";
				cout<<  progressBar<<" "<<progressString <<"%"<< "\n"; 
			}else{
				progressString = "[";	
				for (int p = 0; p < width - 1; p++){
					progressString += "#";	
				}
				progressString += "]";
				cout << "100% " << progressString <<"\n";
			}
		}
	}

	
	//Clean up and ending simulation
	printf("\nComplete\n");
	delete[] bodies;
	return 0;
};


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
#include <fstream>
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
	double r_i = 0.015;

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
	int numBodies =10;

	//creating an array to keep track of the bodies in memory
	Body* bodies = new Body[numBodies];

	//uniform masssed particles in this example
	double m = 1.0;
	
	//the total mass of the system in this example
	double M = m*numBodies;
	
	//arbitrarily chosen gravitational constant
	double G =6.6743*pow(10,-11);


	//Generating the traits of each body for the system
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
		double v_x = v*cos(theta);
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

	
	//To out.sys is to output the data to a readable source that can be graphed later
	std::ofstream out;
	out.open("out.sys");//delimeters ",;\n" for position, body, system state at a given time
	
	/* Output will be in the following form for two consecutive instants in time of 3 objects
	 * 
	 * out.sys:
	 * x1,y1,z1;x2,y2,z2;x3,y3,z3
	 * x1,y1,z1;x2,y2,z2;x3,y3,z3
	 *
	 */

	double dt = 0.001; //the time step in which the system advances by
	int maxSteps = 10000;//the total number of steps that the driver will simulate for
	int frames = 1000;
	int frameStep = maxSteps/frames;//every nth framestep the position values will be put in out.sys
	double progressBar = 0;
	
	for (int i = 0; i<maxSteps; i++){//the ordering of the bodies in the array may have an effect on the system evolution
		
		//Calulation of the force on each body j, then adjusting its velocity and position
		for (int j = 0; j<numBodies ; j++){
			//Calculate the force exerted on body[j]
			double F[3] = {0,0,0};
			s.calculateForces(bodies[j],*s.head,F);
			//The force F on body j has been solved for, now to calculate the motion 
				
			double m = bodies[j].mass;
			double a[3] = {G*F[0]/m,G*F[1]/m,G*F[2]/m};//the acceleration of the body


			if(isnan(a[0]) || isnan(a[1]) || isnan(a[2])){
				cerr << "\nForces to strong for double precision; consider a larger global cutoffDistance in body.h";
				return -1;
			}
			

			/* Discrete kinematic approximations
			 *
			 * v+=a*dt
			 * x+=v*dt
			 *
			 */
			for( int k = 0; k < 3; k++){
				//Calculating the position and velocity of body j along dimension k
				bodies[j].velocity[k] += a[k]*dt;
				bodies[j].position[k] += bodies[j].velocity[k]*dt;	
			}

				//Output to record the position of the body j
			if(i%frameStep==0){
						out << bodies[j].position[0] <<","<<bodies[j].position[1] << "," << bodies[j].position[2] << ";";
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
				cout<< progressString << progressBar <<"%\n"; 
			}else{
				progressString = "[";	
				for (int p = 0; p < width - 1; p++){
					progressString += "#";	
				}
				progressString += "]";
				cout << progressString <<"100%\n";
			}
		}
		if(i%frameStep==0){
			out << "\n";// record the state of the system for the next time step
		}
	}

	
	//Clean up and ending simulation
	out.close();
	printf("\nComplete\n");
	delete[] bodies;
	return 0;
};


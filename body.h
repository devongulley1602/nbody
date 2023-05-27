#include <iostream>

class Body {

	public:
		//Properties of the object
		double mass,charge,position[3],velocity[3];
		
		//Constructor
		Body (double _mass, double _position[3],double _velocity[3], double _charge = 0) {
			mass = _mass;
			charge = _charge;
				for (int i = 0 ; i<3;i++){
					position[i] = _position[i];
					velocity[i] = _velocity[i];
				}
		}

		double kineticEngergy () {
			double v_squared = 0;
			for (int i = 0; i<3;i++){
				v_squared += velocity[i]*velocity[i];
			}
			return 0.5 * mass * v_squared;
		}

		void print() {
			std::cout << "Mass: "<< mass << "\nPosition: ("<<position[0]<<","<< position[1]<< "," << position[2] << ")\nVelocity: ("<<velocity[0]<<","<< velocity[1]<< "," << velocity[2] << ")";

		}
};


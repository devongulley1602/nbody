#define DIMENSION 3
class Body {

	public:
		//Properties of the object
		double mass,charge,position[DIMENSION],velocity[DIMENSION];
		
		//Constructor
		Body (double _mass, double _position[DIMENSION],double _velocity[DIMENSION], double _charge = 0) {
			mass = _mass;
			charge = _charge;
				for (int i = 0 ; i<DIMENSION;i++){
					position[i] = _position[i];
					velocity[i] = _velocity[i];
				}
		}

		double kineticEngergy () {
			double v_squared = 0;
			for (int i = 0; i<DIMENSION;i++){
				v_squared += velocity[i]*velocity[i];
			}

			return 0.5 * mass * v_squared;
		}
};


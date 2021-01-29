#include <fstream>
#include "CCsv.h"

void main()
{
	std::string line;

	std::ifstream ifs( "PredAirCraft1_1162_0.csv" );
	CCsv csv(ifs);


	while(csv.getline(line) != 0){
		std::cout << "line = '" << line <<"'\n";

		for(int i = 0; i < csv.getnfield(); i ++){
			std::cout << "field[" << i << "] = '"
		     		  << csv.getfield(i) << "'\n";
		}
	}
}
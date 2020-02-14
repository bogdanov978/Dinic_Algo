# include <iostream>
# include <fstream>
# include "Dinic_graph.h"
int main()
{
	std::ifstream in_file;
	in_file.open("in.txt");
	Dinic_Graph problem(1000000, in_file);
	in_file.close();

	std::ofstream o_file;
	o_file.open("out.txt");
	std::cout << problem.Find_Max_Flow(o_file) << std::endl;
	o_file.close();

	system("pause");
	return 0;
}
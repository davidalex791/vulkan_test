#include "FirstApp.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <fstream>
#include <filesystem>
int main(int argc, char* argv[])
{
	try 
	{
		//std::filesystem::path cwd = std::filesystem::current_path() / "filename.txt";
		//std::ofstream file(cwd.string());
		//file.close();
		std::string cur_dir(argv[0]);
		cur_dir = cur_dir.substr(0, cur_dir.find_last_of("/\\")+1); //+1 to include the slash
		std::cout << "Current path: " << cur_dir << std::endl;

		lve::FirstApp app{cur_dir};

		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
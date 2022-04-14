#include <fstream>
#include <filesystem>
#include <string>
#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "mio/mio.hpp"

using namespace std;

int main(int argc, char** argv) {
	CLI::App app{ "App description" };

	std::string inPath;
	app.add_option("input", inPath, "Image file")
		->check(CLI::ExistingFile)
		->required();

	std::string outPath;
	app.add_option("outPath", outPath, "Drive to image")
		->check(CLI::ExistingPath)
		->required();

	CLI11_PARSE(app, argc, argv);


	// reading
	mio::mmap_source inFile(inPath);
	auto size = filesystem::file_size(inPath);

	// writing
	ofstream outStream(outPath);
	outStream.write(inFile.data(), size);
	return 0;
}

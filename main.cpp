#include <fstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "mio/mio.hpp"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
	// set up cli stuff
	CLI::App app{ "Fast, unsafe imager written in C++. What else could you want?" };

	string inPath;
	app.add_option("input", inPath, "Image file")
		->check(CLI::ExistingFile)
		->required();

	string outPath;
	app.add_option("output", outPath, "Drive to image")
		->check(CLI::ExistingPath)
		->required();

	CLI11_PARSE(app, argc, argv);


	// reading
	mio::mmap_source inFile(inPath);
	auto size = fs::file_size(inPath);

	// writing
	fs::perms filePerms = fs::status(outPath).permissions() & fs::perms::others_write;

	if (filePerms == fs::perms::none) {
		if (getuid()) {
			cout << "\x1B[93mhey if this seems like it was way too fast, try running it again with sudo\033[0m\t\t" << endl;
		}
	}

	ofstream outStream(outPath);
	outStream.write(inFile.data(), size);

	return 0;
}

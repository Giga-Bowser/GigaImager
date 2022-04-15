#include <fstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "mio/mio.hpp"
#include "xxhash/xxhash.h"

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

	bool verify = false;
	app.add_flag("-v,--verify", verify);

	CLI11_PARSE(app, argc, argv);


	// reading
	cout << "Reading file..." << "\r" << flush;
	mio::mmap_source inFile(inPath);
	auto size = fs::file_size(inPath);
	cout << "Finished reading!" << endl;

	// writing
	cout << "Writing file..." << "\r" << flush;
	fs::perms filePerms = fs::status(outPath).permissions() & fs::perms::others_write;

	if (filePerms == fs::perms::none) {
		if (getuid()) {
			cout << "\x1B[93mhey if this seems like it was way too fast, try running it again with sudo\033[0m\t\t" << endl;
		}
	}

	fstream outStream(outPath);
	outStream.write(inFile.data(), size);
	outStream.close();
	cout << "Finished writing!" << endl;


	if (verify) {
		cout << "Verifying..." << "\r" << flush;
		XXH64_hash_t fileHash = XXH64(inFile.data(), size, 0);
		inFile.unmap();

		ifstream verifyFile(outPath, ios::binary | ios::in);
		vector<char> verifyData(size);
		verifyFile.read(verifyData.data(), size);
		XXH64_hash_t driveHash = XXH64(verifyData.data(), size, 0);

		if (fileHash == driveHash) {
			cout << "Verified successfully!" << endl;
		} else {
			cout << "Image did not match :(" << endl;
		}
	} else {
		inFile.unmap();
	}


	return 0;
}

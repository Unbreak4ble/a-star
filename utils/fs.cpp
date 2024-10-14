#include "fs.hpp"
#include <string>
#include <vector>
#include <fstream>

namespace utils::fs {

    std::string readFile(std::string file){
	    std::string content;
	    std::string line;
    
	    std::ifstream hfile(file);
		if(hfile.fail()) return "";

	    std::ifstream shfile(file, std::ios::binary | std::ios::ate);
	    size_t size = shfile.tellg();
	    shfile.close();

	    std::vector<char> bytes;
	    bytes.resize(size);

	    hfile.read(bytes.data(), size);
    
	    hfile.close();

	    content.assign(bytes.data(), bytes.size());

        return content;
    }

}
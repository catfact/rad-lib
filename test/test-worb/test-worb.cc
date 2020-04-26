
#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"
#include "worb.h"

using nlohmann::json;

using std::string;
using std::stringstream;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, const char **argv){    
    if (argc < 2) {
	std::cerr << "missing required argument (configuration file)" << endl;
	exit(1);
    }

    string path(argv[1]);

    
    ifstream ifs(path);
    if (!ifs.is_open()) {
        cerr << "failed to open config.json file: " << path << endl;
        exit(1);
    }
    stringstream ss;
    ss << ifs.rdbuf();
    string str = ss.str();
    cout << str << endl;


    double period, gamma, base, noise, initial;
    unsigned int count;
    try {
        json jsn = json::parse(str);
	period = jsn["period"];
	gamma = jsn["gamma"];
	base = jsn["base"];
	noise = jsn["noise"];
	initial = jsn["initial"];
	count = static_cast<unsigned int>(jsn["count"]);
    } catch (json::exception &ex) {
        cerr << "failed to parse json from file: " << path << " (" << ex.what() << ")" << endl;
        exit(1);
    }

    rad::Worb worb;
    worb.init(initial, base, noise);

    worb.setPeriod(static_cast<unsigned int>(period));
    worb.setGamma(gamma);

    cout << "output = [ " << endl;
    for (int i=0; i<count; ++i) {
	std::cout << worb.next() << ", ";
    }
    cout << endl << "]" << endl;
    exit(0);    
}

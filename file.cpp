#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

int main() {
    std::ifstream configFile("cluster.conf");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open cluster.conf" << std::endl;
        return 1;
    }

    std::map<std::string, int> parameters;

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string paramName;
        char colon;
        int paramValue;

        if (iss >> paramName >> colon >> paramValue) {
            parameters[paramName] = paramValue;
        }
    }

    configFile.close();

    // Now you can access the parameters using the map
    int numberOfClusters = parameters["number_of_clusters"];
    int numberOfVectorHashTables = parameters["number_of_vector_hash_tables"];
    int numberOfVectorHashFunctions = parameters["number_of_vector_hash_functions"];
    int maxNumberMHypercube = parameters["max_number_M_hypercube"];
    int numberOfHypercubeDimensions = parameters["number_of_hypercube_dimensions"];
    int numberOfProbes = parameters["number_of_probes"];

    // Use the parameters in your algorithm
    std::cout << "Number of Clusters: " << numberOfClusters << std::endl;

    return 0;
}

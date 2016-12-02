#include <fstream>
#include <vector>
#include "rod.hpp"

#ifndef __READER__
#define __READER__


class Reader{
		char* buffer = new char[8];
		char* data = new char[8];
		std::ifstream* inputStream;
		bool closed;
		void chunk();
    std::vector<Rod*> rods;
    std::vector<Link*> links;
	public:
        double width;
		Reader(const std::string &filename);
		void load();
		int nextInt();
		long nextLong();
		double nextDouble();
		void close();
		glm::vec3* nextPoint();
    std::vector<Rod*> getRods();
    std::vector<Link*> getLinks();

};

#endif

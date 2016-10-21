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
		
	public:
		Reader(const std::string &filename);
		std::vector<Rod*> loadRods();
		int nextInt();
		long nextLong();
		double nextDouble();
		void close();
		glm::vec3* nextPoint();

};

#endif
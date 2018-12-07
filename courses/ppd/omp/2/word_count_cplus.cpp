#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <omp.h>

double tempo()
{
	struct timeval tv;
	gettimeofday(&tv,0);
	return tv.tv_sec + tv.tv_usec/1e6;
}

int main(int argc, char const *argv[])
{
	
	double endTime, startTime;

	std::vector <std::string> words;
	std::vector <std::string> listwords;
	std::vector <unsigned int> event;
	std::string vectorFile;

	//omp_set_nested(1);
	//void omp_set_num_threads(argv[2]);

	if (argc < 2) 
	{
		std::cerr << "Usage: ./word_count <file_for_count> <num_Threads>" ;
		return EXIT_FAILURE;
	}

	std::ifstream fileIn(argv[1]);

		
	//#pragma omp parallel shared(fileIn, vetorFile) private (i,words)
	//{
	while (fileIn >> vectorFile)
	{                
		if(vectorFile != ";" && vectorFile != ".")
		{
			words.push_back(vectorFile);
		}
		
	}
	fileIn.close();

	startTime = tempo();

	int sum=0;
	
	std::cout << "words size: " << listwords.size() << std::endl;
	#pragma omp parallel for reduction(+:sum)
	for (unsigned int j=0; j < words.size(); ++j)
	{	
			
		listwords.push_back(words.at(j));
		event.push_back(1);
		
		for(unsigned int i = 0; i < listwords.size()-1; ++i)
		{
			if (words.at(j) == listwords.at(i))
			{
				
				std::vector<unsigned int>::iterator it = event.begin()+i;
				event.insert(it,event.at(i)+1);
				listwords.pop_back();
				event.pop_back();
				
				//break;				
			}
		}
		std::cout << "Th: " << omp_get_thread_num() << std::endl;	 		
	 	sum++;
	}
	
	for (unsigned int i = 0; i < listwords.size(); ++i){
  		
  		std::cout << listwords.at(i) << " : " << event.at(i) << std::endl;
 	} 

	endTime = tempo();
		
	std::cout << "Total: "<< sum << std::endl;
	
 	std::cout << "Tempo Sequencial: " << endTime-startTime << std::endl;
	
	return 0;
}

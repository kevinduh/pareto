//////////////////////////////////////////////////////////////
// PARETO: computes the pareto frontier given a set of points
//////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Datapoint.h"
#include "ParetoAlgo.h"
#include "StablesortAlgo.h"
#include "BruteforceAlgo.h"
#include "NondominatedsortAlgo.h"

void usage(){
  std::cerr << "[usage] pareto (-a algorithmName) -l file1 -l file2 ... -l fileK \n \
  Computes pareto frontier given N K-dimensional datapoints. \n \
  There are K>1 files, each consisting of a column of N numbers. \n \
  The flag -l indicates larger number is better (maximize); \n \
  alternatively, the -s flag means smaller is better (minimize). \n \
  The output consists of N labels, where 1=pareto and 0=not. \n\n \
  Flag -a specifies the algorithm to use. \n \
  Supported algorithms: \n \
    -a bruteforce (default for K>2) \n \
    -a stablesort (default for K=2) \n \
    -a nondominatedsort (generates ranking, with higher number meaning higher level of pareto front)\n \
  " << std::endl;

  exit(1);
}

int main(int argc, char* argv[]){

  /////////////////////////////////
  // Command line parameters
  std::vector< std::pair<char*,int> > files; 
  std::string algoName; // name of specific paretro algorithm to use

  int c;
  while ((c=getopt(argc,argv,":l:s:a:")) !=-1) {
      switch (c){
      case 'l':
	files.push_back(std::make_pair(optarg,1));
	break;
      case 's':
	files.push_back(std::make_pair(optarg,-1));
	break;
      case 'a':
	algoName=std::string(optarg);
	break;
      case '?':
	usage();
	break;
      }
  }

  ////////////////////////////////////
  // Read data
  int K = files.size(); // # of files, i.e. K-dimensional problem
  if (K<2){
    std::cerr << "[error] Insufficient dimension (2 or more input files needed). " << std::endl;
    usage();
  }

  std::vector<Datapoint*> dataset; //dataset will contain N K-dim datapoints
  size_t idmax_file1; // for sanity check to ensure equal file lengths
  for (size_t f=0; f<files.size(); ++f){

    std::ifstream infile(files[f].first);
    if (!infile){
      std::cerr << "[error] Unable to open file: " << files[f].first << std::endl;
      exit(1);
    }

    size_t id=0;
    float num;
    float m=files[f].second;

    while (infile >> num){
      if (f==0){
	Datapoint *d = new Datapoint(id);
	dataset.push_back(d);
      }
      else {
	if (id >= idmax_file1){
	  std::cerr << "[error] unequal #datapoints in " << files[0].first << " vs " << files[f].first << std::endl;
	  exit(1);
	}
      }
      dataset[id]->addNumber(m*num);
      ++id;
    }

    infile.close();
    if (f == 0){
      idmax_file1 = id;
    }
    else {
      if (id != idmax_file1){
	std::cerr << "[error] unequal #datapoints in " << files[0].first << " vs " << files[f].first << std::endl;
	exit(1);
	  
      }
    }
  }


  //////////////////////////////////////
  // Choose the Pareto Algorithm to use
  if (algoName.empty()){
    if (K==2){
      algoName="stablesort";
    }
    else {
      algoName="bruteforce";
    }
  }

  ParetoAlgo* algo = NULL;
  if (algoName == "stablesort"){
    algo = new StablesortAlgo();
  }
  else if (algoName == "bruteforce") {
    algo = new BruteforceAlgo();
  }
  else if (algoName == "nondominatedsort"){
    algo = new NondominatedsortAlgo();
  }
  else {
    std::cout << "[error] Unknown AlgoName: " << algoName << std::endl;
  }


  ////////////////////////////////////
  // Compute the Pareto Frontier! 
  int numPareto = algo->computeFrontier(dataset);
  std::cerr << "#pareto: " << numPareto << " of " << dataset.size() << " total, by the " << algoName << " algorithm " << std::endl;
  for (size_t d=0; d<dataset.size(); ++d){
    //dataset[d]->print();
    std::cout << dataset[d]->getParetoStatus() << std::endl;
  }

  return 0;
}

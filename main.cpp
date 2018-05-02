#include "kMeans.hpp"
#include "DistanceKCliques.hpp"
#include "KernelClustering.hpp"
#include "StructureClustering.hpp"
using mType = long;

int main(int argc, char *argv[])
{
  /********** variable declarations *******/

  /********** file reading ****************/
  // argv[0] is the program name
  // argv[1] is the data file name
  if (argc < 2)
  {
    // if we don't have a file name, we want to quit now
    cout << "Program usage: " << argv[0] <<" <filename>" << endl;
    return 0;
  }

  ifstream file(argv[1]);
  if (!file.is_open())
  {
    // on open fail, quit program
    cout << "Could not open file" << endl;
    return 1;
  }

  // create the graph
  UndirectedGraph<mType> horse_graph(1);
  try
  {
    file >> horse_graph;
  }
  catch (MatrixErr& err)
  {
    cout << err.what() << endl;
    return 1;
  }
  file.close();

  const long cluster_count = 3;
  kMeans(horse_graph, cluster_count);
  DistanceKCliques(horse_graph);
  KernelClustering(horse_graph);
  StructureClustering(horse_graph);

  return 0;
}

#include "kMeans.hpp"
#include "DistanceKCliques.hpp"
#include "KernelClustering.hpp"
using mType = long;

int main(int argc, char *argv[])
{
  /********** variable declarations *******/
  srand(time(NULL));

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

  const long cluster_count = 8;
  auto kmeans_clusters = kMeans(horse_graph, cluster_count);
  auto kernel_clusters = KernelClustering(horse_graph, cluster_count);

  const long distance_k = 10;
  auto distK_clusters = DistanceKCliques(horse_graph, distance_k);

  std::cout << "Cluster Information for kMeans Algorithm:" << std::endl;
  OutputClusterInformation(horse_graph, kmeans_clusters);
  std::cout << "Cluster Information for Distance-k Algorithm:" << std::endl;
  OutputClusterInformation(horse_graph, distK_clusters);
  std::cout << "Cluster Information for Kernel Clustering Algorithm:" << std::endl;
  OutputClusterInformation(horse_graph, kernel_clusters);

  return 0;
}
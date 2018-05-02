#include "UndirectedGraph.h"
using mType = double;

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

  // the first number is the number of Vectors
  long num_rows;
  file >> num_rows;
  SymMatrix<mType> sym_matrix(num_rows);
  try
  {
    file >> sym_matrix;
  }
  catch (RangeErr<mType>& err)
  {
    cout << err.what() << endl;
    return 1;
  }

  file.close();

  std::cout << sym_matrix << std::endl;
  return 0;
}

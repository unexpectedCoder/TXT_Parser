#include <iostream>
#include <string>

#include "txt_parser.h"

using namespace std;

int main()
{
  try
  {
    double **matr = new double* [2];
    for (int i = 0; i < 2; i++)
    {
      matr[i] = new double [3];
      for (int j = 0; j < 3; j++)
        matr[i][j] = i * j + i * i + j - 1;
    }

    TXT_Parser par("1.txt", 'w');
    par.writeMatrix<double>(matr, 2, 3);
    par.close();

    par.open("1.txt", 'r');
    size_t size;
    double *arr = par.readArray(size);
    cout << "#1" << endl;
    for (size_t i = 0; i < size; i++)
      cout << arr[i] << '\t';
    cout << endl;

    arr = par.readArray(size);
    cout << "#2" << endl;
    for (size_t i = 0; i < size; i++)
      cout << arr[i] << '\t';
    cout << endl;
    par.close();
  }
  catch (const string &ex)
  {
    cout << ex << endl;
  }

  return 0;
}

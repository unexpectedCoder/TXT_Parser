#include <iostream>
#include <string>

#include "abstract_txt_parser.h"

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

    AbstractTXTParser::createFileTXT("1.txt");
    AbstractTXTParser par("1.txt", 'w');
    par.writeMatrix<double>(matr, 2, 3);
    par.close();

    par.open("1.txt", 'r');
    int r, c;
    double **m = par.readMatrix(r, c);
    cout << "rows = " << r << ", columns = " << c << endl;
    for (int i = 0; i < r; i++)
    {
      for (int j = 0; j < c; j++)
        cout << m[i][j] << '\t';
      cout << endl;
    }
    par.close();
  }
  catch (const string &ex)
  {
    cout << ex << endl;
  }

  cout << "Program successfully completed...\n";
  return 0;
}

#include <stdlib.h>
#include "abstract_txt_parser.h"

using namespace std;


void AbstractTXTParser::Error::formError(const string &err_txt)
{
  error = "ERROR: " + err_txt + "\n";
  errors.push(error);
}

const string& AbstractTXTParser::Error::sendMess(const string &err_txt)
{
  formError(err_txt);
  return error;
}

const stack<string>& AbstractTXTParser::Error::getErrors() const
{
  return errors;
}


AbstractTXTParser::AbstractTXTParser()
{
  curr_str = "...eof...\n";
  digit = {{'0', 0},
           {'1', 1},
           {'2', 2},
           {'3', 3},
           {'4', 4},
           {'5', 5},
           {'6', 6},
           {'7', 7},
           {'8', 8},
           {'9', 9}};
}

AbstractTXTParser::AbstractTXTParser(const string &path, char mode)
{
  curr_str = "...eof...\n";
  open(path, mode);
  digit = {{'0', 0},
           {'1', 1},
           {'2', 2},
           {'3', 3},
           {'4', 4},
           {'5', 5},
           {'6', 6},
           {'7', 7},
           {'8', 8},
           {'9', 9}};
}

AbstractTXTParser::~AbstractTXTParser()
{
  curr_str.clear();
  curr_str.~basic_string();
  delete [] arr;
  for (int i = 0; i < rows; i++)
    delete [] matr[i];
  delete [] matr;
}

void AbstractTXTParser::open(const string &path, char mode)
{
  if (!isExisting(path))
    throw err.sendMess("file is not found or existing!");
  if (!checkMode(mode))
    throw err.sendMess("unknown file opening mode!");

  if (mode == 'w')
  {
    file.open(path, ios_base::out);
    return;
  }
  if (mode == 'r')
  {
    file.open(path, ios_base::in);
    return;
  }
  if (mode == 'a')
  {
    file.open(path, ios_base::app);
    return;
  }
}

bool AbstractTXTParser::isExisting(const string &path) const
{
  fstream f(path.c_str());
  if (f.is_open())
    return true;
  return false;
}

void AbstractTXTParser::close()
{
  if (isOpen())
    file.close();
}

bool AbstractTXTParser::isOpen() const
{
  if (file.is_open())
    return true;
  return false;
}

bool AbstractTXTParser::isEnd() const
{
  if (file.eof())
    return true;
  return false;
}


void AbstractTXTParser::createFileTXT(const string &path)
{
  ofstream f{ path.c_str() };
}

void AbstractTXTParser::write(const string &text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  if (!text.empty())
    file << text.c_str();
}

void AbstractTXTParser::write(double v)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v;
}

void AbstractTXTParser::write(double v, char split)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v << split;
}

void AbstractTXTParser::write(double v, const string &text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v << text.c_str();
}

void AbstractTXTParser::write(const string &text, double v, char split)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << text.c_str() << v << split;
}

void AbstractTXTParser::write(const string &pre_text, double v, const string &post_text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << pre_text.c_str() << v << post_text.c_str();
}

const string& AbstractTXTParser::readNext()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");

  char *str;
  if (!file.eof())
    file >> str;
  curr_str = str;
  delete [] str;
  return curr_str;
}

double AbstractTXTParser::readDouble()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");

  double v;
  file >> v;
  return v;
}

double** AbstractTXTParser::readMatrix(int &r, int &c)
{
  vector<double*> buf;
  char line[MAX_CHAR_LINE];
  int c_buf = 0;
  r = 0; c = 0;

  while (true)
  {
    if (c != c_buf)
      throw err.sendMess("invalid matrix in file! Number of columns doesn't match in rows!");

    getLine(line, MAX_CHAR_LINE);
    if (isEnd() || strcmp(line, "") == 0) break;
    double *arr = readArray(line, c);
    c_buf = c;
    buf.push_back(arr);
  }

  r = buf.size();
  if (r < 1 || c < 1)
    throw err.sendMess("invalid matrix size! Number of rows or columns < 1!");
  matr = new double*[r];
  for (int i = 0; i < r; i++)
    matr[i] = buf[i];
  return matr;
}

double* AbstractTXTParser::readArray(const char *line, int &size)
{
  if (isEnd() || !isOpen())
    throw err.sendMess("end of file or file is not opened!");
  vector<double> buf;
  return strToArray(line, size);
}


void AbstractTXTParser::getLine(char *s, int size, char split)
{
  if (isEnd() || !isOpen())
    throw err.sendMess("end of file or file is not opened!");
  file.getline(s, size, split);
}

void AbstractTXTParser::newLine()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << '\n';
}


bool AbstractTXTParser::checkMode(char mode)
{
  if (mode == 'w' || mode == 'r' || mode == 'a')
    return true;
  return false;
}

double AbstractTXTParser::strToDouble(const string &str)
{
  const char* s = str.c_str();
  return strToDouble(s);
}

double AbstractTXTParser::strToDouble(const char *s)
{
  string val_str;
  string_num(s, val_str);

  double val = 0;
  int i;
  int pos_neg = 1;
  if (val_str[0] == '-')
    pos_neg = -1;

  bool is_integer = true;
  double ks = 1;
  int kb = 1;

  for ((val_str[0] == '-')? i = 1 : i = 0; i < val_str.size(); i++)
  {
    if (val_str[i] == '.')
    {
      is_integer = false;
      i++;
    }

    if (is_integer)
    {
      kb *= 10;
      integer(val, kb, digit[val_str[i]]);
    }
    else
    {
      ks *= 0.1;
      fraction(val, ks, digit[val_str[i]]);
    }
  }

  return pos_neg * val;
}

double* AbstractTXTParser::strToArray(const char *s, int &size)
{
  string val_str = "";
  vector<double> buf;
  while (true)
  {
    if (isalpha(*s) || isspace(*s) || !*s)
    {
      if (!val_str.empty())
      {
        buf.push_back(strToDouble(val_str));
        val_str.clear();
      }
      if (!*s)
        break;
      s++;
      continue;
    }
    val_str += *s;
    s++;
  }

  size = buf.size();
  arr = new double[size];
  for (int i = 0; i < size; i++)
    arr[i] = buf[i];
  return arr;
}

void AbstractTXTParser::string_num(const char *s, string &val_str)
{
  val_str = "";
  while (*s != '\0')
  {
    if (isalpha(*s) || isspace(*s)) break;
    if (!isdigit(*s) && *s != '-' && *s != '.')
    {
      s++;
      continue;
    }
    if ((val_str.empty() || val_str == "-") && *s == '.')
    {
      val_str = "0.";
      s++;
      continue;
    }

    val_str += *s;
    s++;
  }
}

void AbstractTXTParser::integer(double &v, int k, int a)
{
  v = k * v + a;
}

void AbstractTXTParser::fraction(double &v, double k, int a)
{
  v = v + k * a;
}

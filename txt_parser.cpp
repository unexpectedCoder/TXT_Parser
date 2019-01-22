#include <stdlib.h>
#include <iostream>
#include "txt_parser.h"
using namespace std;


void TXT_Parser::Error::formError(const string &err_txt)
{
    error = "ERROR: " + err_txt + "\n";
    errors.push(error);
}

const string& TXT_Parser::Error::sendMess(const string &err_txt)
{
  formError(err_txt);
    return error;
}

const stack<string>& TXT_Parser::Error::getErrors() const
{
    return errors;
}


TXT_Parser::TXT_Parser()
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

TXT_Parser::TXT_Parser(const string &path, char mode)
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

void TXT_Parser::open(const string &path, char mode)
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

bool TXT_Parser::isExisting(const string &path) const
{
    fstream f(path.c_str(), ios_base::in);
    if (f.is_open())
        return true;
    return false;
}

void TXT_Parser::close()
{
    if (isOpen())
        file.close();
}

bool TXT_Parser::isOpen() const
{
    if (file.is_open())
        return true;
    return false;
}

bool TXT_Parser::isEnd() const
{
  if (file.eof())
    return true;
  return false;
}


void TXT_Parser::createFileTXT(const string &path)
{
    ofstream f{ path.c_str() };
}

void TXT_Parser::write(const string &text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  if (!text.empty())
    file << text;
}

void TXT_Parser::write(double v)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v;
}

void TXT_Parser::write(double v, char split)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v << split;
}

void TXT_Parser::write(double v, const string &text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << v << text;
}

void TXT_Parser::write(const string &text, double v, char split)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << text << v << split;
}

void TXT_Parser::write(const string &pre_text, double v, const string &post_text)
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");
  file << pre_text << v << post_text;
}

const string& TXT_Parser::readNext()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");

  if (!file.eof())
    file >> curr_str;
  return curr_str;
}

double TXT_Parser::readDouble()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");

  double v;
  file >> v;
  return v;
}

double* TXT_Parser::readArray(size_t &size)
{
  std::vector<double> buf;
  char *s;
  file.getline(s, 100, '\n');       // TODO: parsing numbers from string
  cout << s << endl;
  buf.push_back(strToDouble(s));

  size = buf.size();
  double* arr = new double[size];
  for (size_t i = 0; i < size; i++)
    arr[i] = buf[i];

  return arr;
}


void TXT_Parser::newLine()
{
  if (!isOpen())
    throw err.sendMess("file is not opened!");

  file << '\n';
}


bool TXT_Parser::checkMode(char mode)
{
  if (mode == 'w' || mode == 'r' || mode == 'a')
    return true;
  return false;
}

double TXT_Parser::strToDouble(const string &str)
{
  const char* s = str.c_str();
  return strToDouble(s);
}

double TXT_Parser::strToDouble(const char *s)
{
  string val_str;
  string_num(s, val_str);

  double val = 0;
  size_t i;
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

void TXT_Parser::string_num(const char *s, string &val_str)
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

void TXT_Parser::integer(double &v, int k, int a)
{
  v = k * v + a;
}

void TXT_Parser::fraction(double &v, double k, int a)
{
  v = v + k * a;
}
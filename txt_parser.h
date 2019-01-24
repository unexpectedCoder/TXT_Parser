#ifndef TXT_PARSER
#define TXT_PARSER

#include <fstream>
#include <cstring>
#include <stack>
#include <map>
#include <vector>

#define MAX_CHAR_LINE 50

class TXT_Parser
{
private:
  class Error
  {
  private:
    std::string error;
    std::stack<std::string> errors;
  public:
    Error() : error("No errors.\n") {}
    ~Error() {
      error.clear();
    }
  const std::string& sendMess(const std::string &err_txt);
  const std::stack<std::string>& getErrors() const;

  private:
    void formError(const std::string &err_txt);
  };

public:
  TXT_Parser();
  TXT_Parser(const std::string &path, char mode);
  ~TXT_Parser();

  static void createFileTXT(const std::string &path);
  void open(const std::string &path, char mode);
  void close();

  bool isExisting(const std::string &path) const;
  bool isOpen() const;
  bool isEnd() const;

  void write(const std::string &text);
  void write(double v);
  void write(double v, char split);
  void write(double v, const std::string &text);
  void write(const std::string &text, double v, char split);
  void write(const std::string &pre_text, double v, const std::string &post_text);
  template<typename T>
  void writeArray(const T* arr, int size);
  template<typename T>
  void writeMatrix(T** matr, int rows, int columns);

  const std::string& readNext();
  double readDouble();
  double* readArray(const char *line, size_t &size);
  double** readMatrix(size_t &r, size_t &c);

  double strToDouble(const std::string &str);
  double strToDouble(const char *s);
  double* strToArray(const char *s, size_t &size);
  void getLine(char *s, int size = MAX_CHAR_LINE, char split = '\n');
  void newLine();

private:
  bool checkMode(char mode);
  void string_num(const char *s, std::string &str);
  void integer(double &v, int k, int a);
  void fraction(double &v, double k, int a);

private:
  std::fstream file;
  Error err;
  std::string curr_str;
  std::map<char, int> digit;
  double *arr, **matr;
  size_t rows;
};


template<typename T>
void TXT_Parser::writeArray(const T* arr, int n)
{
  if (n < 1)
    throw err.sendMess("invalid array size! Array size must be > 0!");
  for (int i = 0; i < n - 1; i++)
    file << arr[i] << '\t';
  file << arr[n - 1] << "\n\n";
}

template<typename T>
void TXT_Parser::writeMatrix(T** m, int r, int c)
{
  if (r < 1 || c < 1)
    throw err.sendMess("invalid matrix size! Number of rows and columns must be > 0!");

  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c - 1; j++)
      file << m[i][j] << '\t';
    file << m[i][c - 1] << '\n';
  }
  file << '\n';
}

#endif

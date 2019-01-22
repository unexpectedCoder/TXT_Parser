#ifndef TXT_PARSER
#define TXT_PARSER

#include <fstream>
#include <string>
#include <stack>
#include <map>
#include <vector>

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
  ~TXT_Parser() {
    curr_str.clear();
    curr_str.~basic_string();
  }

  void createFileTXT(const std::string &path);
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
  double* readArray(size_t &size);

  double strToDouble(const std::string &str);
  double strToDouble(const char *s);
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
};


template<typename T>
void TXT_Parser::writeArray(const T* arr, int size)
{
  if (size < 1)
    throw err.sendMess("invalid array size! Array size must be > 0!");

  for (int i = 0; i < size; i++)
    file << arr[i] << '\t';
  file << '\n';
}

template<typename T>
void TXT_Parser::writeMatrix(T** matr, int rows, int columns)
{
  if (rows < 1 || columns < 1)
    throw err.sendMess("invalid matrix size! Number of rows and columns must be > 0!");

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      file << matr[i][j] << '\t';
    file << '\n';
  }
}

#endif
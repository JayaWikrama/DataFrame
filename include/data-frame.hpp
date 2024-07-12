#ifndef __DATA_FRAME_HPP__
#define __DATA_FRAME_HPP__

#include <vector>
#include <string>

class DataFrame {
  private:
    unsigned char type;
    size_t sz;
    std::vector <unsigned char> data;
    const void *exeFunc;
    void *exeFuncParam;
    const void *postFunc;
    void *postFuncParam;
    DataFrame *next;
  public:
    typedef enum _FRAME_TYPE_t {
      FRAME_TYPE_START_BYTES = 0,
      FRAME_TYPE_CONTENT_LENGTH,
      FRAME_TYPE_COMMAND,
      FRAME_TYPE_DATA,
      FRAME_TYPE_STOP_BYTES,
      FRAME_TYPE_VALIDATOR
    } FRAME_TYPE_t;

    DataFrame();

    DataFrame(DataFrame::FRAME_TYPE_t type);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz,
              const unsigned char *data);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz,
              const unsigned char *data,
              const void *exeFunc,
              void *exeFuncParam);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz,
              const unsigned char *data,
              const void *exeFunc,
              void *exeFuncParam,
              const void *postFunc,
              void *postFuncParam
              );

    ~DataFrame();

    void setType(DataFrame::FRAME_TYPE_t type);

    void setSize(size_t sz);

    void setRefference(const unsigned char *reference, size_t sz);

    void setRefference(const std::vector<unsigned char> reference);

    void setData(const unsigned char *data, size_t sz);

    void setData(const std::vector<unsigned char> data);

    void setExecuteFunction(const void *_func, void *_param);

    void setPostExecuteFunction(const void *_func, void *_param);

    DataFrame::FRAME_TYPE_t getType();

    size_t getSize();

    size_t getReference(unsigned char *reference, size_t sizeOfReference);

    size_t getReference(std::vector<unsigned char> reference);

    size_t getData(unsigned char *data, size_t sizeOfData);

    size_t getData(std::vector<unsigned char> data);

    std::string getDataFrameFormat();

    DataFrame& operator+=(const DataFrame &obj);

    DataFrame& operator+(const DataFrame &obj);
};

#endif
#ifndef __DATA_FRAME_HPP__
#define __DATA_FRAME_HPP__

#include <vector>
#include <string>

class DataFrame {
  private:
    unsigned char type;
    bool isReference;
    size_t sz;
    std::vector <unsigned char> data;
#ifdef __USE_EXE_FUNC
    const void *exeFunc;
    void *exeFuncParam;
#endif
#ifdef __USE_POST_FUNC
    const void *postFunc;
    void *postFuncParam;
#endif
    DataFrame *next;
  public:
    typedef enum _FRAME_TYPE_t {
      FRAME_TYPE_START_BYTES = 0,
      FRAME_TYPE_CONTENT_LENGTH,
      FRAME_TYPE_COMMAND,
      FRAME_TYPE_DATA,
      FRAME_TYPE_DATA_1,
      FRAME_TYPE_DATA_2,
      FRAME_TYPE_DATA_3,
      FRAME_TYPE_DATA_4,
      FRAME_TYPE_DATA_5,
      FRAME_TYPE_DATA_6,
      FRAME_TYPE_DATA_7,
      FRAME_TYPE_DATA_8,
      FRAME_TYPE_DATA_9,
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
              const std::vector <unsigned char> data);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const char *data);

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const std::string data);

#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz,
              const unsigned char *data,
#ifdef __USE_EXE_FUNC
              const void *exeFunc,
              void *exeFuncParam);
#else
              const void *postFunc,
              void *postFuncParam);
#endif

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const std::vector <unsigned char> data,
#ifdef __USE_EXE_FUNC
              const void *exeFunc,
              void *exeFuncParam);
#else
              const void *postFunc,
              void *postFuncParam);
#endif

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const char *data,
#ifdef __USE_EXE_FUNC
              const void *exeFunc,
              void *exeFuncParam);
#else
              const void *postFunc,
              void *postFuncParam);
#endif

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const std::string data,
#ifdef __USE_EXE_FUNC
              const void *exeFunc,
              void *exeFuncParam);
#else
              const void *postFunc,
              void *postFuncParam);
#endif
#endif

#ifdef __USE_EXE_FUNC
#ifdef __USE_POST_FUNC
    DataFrame(DataFrame::FRAME_TYPE_t type,
              size_t sz,
              const unsigned char *data,
              const void *exeFunc,
              void *exeFuncParam,
              const void *postFunc,
              void *postFuncParam
              );

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const std::vector <unsigned char> data,
              const void *exeFunc,
              void *exeFuncParam,
              const void *postFunc,
              void *postFuncParam
              );

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const char *data,
              const void *exeFunc,
              void *exeFuncParam,
              const void *postFunc,
              void *postFuncParam
              );

    DataFrame(DataFrame::FRAME_TYPE_t type,
              const std::string data,
              const void *exeFunc,
              void *exeFuncParam,
              const void *postFunc,
              void *postFuncParam
              );
#endif
#endif

    ~DataFrame();

    void setType(DataFrame::FRAME_TYPE_t type);

    bool setSize(size_t sz);

    void setReference(const unsigned char *reference, size_t sz);

    void setReference(const std::vector<unsigned char> reference);

    void setReference(const char *reference);

    void setReference(const std::string reference);

    void setData(const unsigned char *data, size_t sz);

    void setData(const std::vector<unsigned char> data);

    void setData(const char *data);

    void setData(const std::string data);

#ifdef __USE_EXE_FUNC
    void setExecuteFunction(const void *_func, void *_param);
    void setExecuteFunction(DataFrame::FRAME_TYPE_t type, bool nullptrOnly, const void *_func, void *_param);
    const void *getExecuteFunction();
    void *getExecuteFunctionParam();
#endif

#ifdef __USE_POST_FUNC
    void setPostExecuteFunction(const void *_func, void *_param);
    void setPostExecuteFunction(DataFrame::FRAME_TYPE_t type, bool nullptrOnly, const void *_func, void *_param);
    const void *getPostExecuteFunction();
    void *getPostExecuteFunctionParam();
#endif

    DataFrame::FRAME_TYPE_t getType();

    size_t getSize();

    size_t getReference(unsigned char *reference, size_t sizeOfReference);

    size_t getReference(std::vector<unsigned char> &reference);

    size_t getData(unsigned char *data, size_t sizeOfData);

    size_t getData(std::vector<unsigned char> &data);

    std::vector <unsigned char> getDataAsVector();

    std::string getDataFrameFormat();

    DataFrame *getNext();

    size_t getAllData(unsigned char *data, size_t sizeOfData);

    size_t getAllData(std::vector<unsigned char> &data);

    std::vector <unsigned char> getAllDataAsVector();

    std::vector <unsigned char> getSpecificDataAsVector(const DataFrame *begin, const DataFrame *end);

#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    void execute();
#endif

    DataFrame& operator+=(const DataFrame &obj);

    DataFrame& operator+(const DataFrame &obj);

    DataFrame* operator[](int index);

};

#endif
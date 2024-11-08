#include <string>
#include <string.h>
#include "data-frame.hpp"

const std::string frameTypeStr[] = {
  "FRAME_TYPE_START_BYTES",
  "FRAME_TYPE_CONTENT_LENGTH",
  "FRAME_TYPE_COMMAND",
  "FRAME_TYPE_SN",
  "FRAME_TYPE_RFU",
  "FRAME_TYPE_BLOCK_NUMBER",
  "FRAME_TYPE_DATA",
  "FRAME_TYPE_DATA_1",
  "FRAME_TYPE_DATA_2",
  "FRAME_TYPE_DATA_3",
  "FRAME_TYPE_DATA_4",
  "FRAME_TYPE_DATA_5",
  "FRAME_TYPE_DATA_6",
  "FRAME_TYPE_DATA_7",
  "FRAME_TYPE_DATA_8",
  "FRAME_TYPE_DATA_9",
  "FRAME_TYPE_STOP_BYTES",
  "FRAME_TYPE_VALIDATOR"
};

DataFrame::DataFrame(){
  this->type = static_cast<unsigned char>(DataFrame::FRAME_TYPE_START_BYTES);
  this->isReference = false;
  this->sz = 0;
  this->data.clear();
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type){
  this->type = static_cast<unsigned char>(type);
  this->isReference = false;
  this->sz = 0;
  this->data.clear();
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     size_t sz){
  this->type = static_cast<unsigned char>(type);
  this->isReference = false;
  this->sz = sz;
  this->data.clear();
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     size_t sz,
                     const unsigned char *data){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = sz;
  if (data != nullptr) this->data.assign(data, data + sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type, const std::vector <unsigned char> data){
  this->type = static_cast<unsigned char>(type);
  if (data.size() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.size();
  if (data.size() > 0) this->data.assign(data.begin(), data.end());
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type, const char *data){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = strlen(data);
  if (data != nullptr) this->data.assign((const unsigned char *) data, (const unsigned char *) data + this->sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type, const std::string data){
  this->type = static_cast<unsigned char>(type);
  if (data.length() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.length();
  if (data.length() > 0) this->data.assign(data.c_str(), data.c_str() + this->sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = nullptr;
  this->exeFuncParam = nullptr;
#endif
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
  this->next = nullptr;
}

#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     size_t sz,
                     const unsigned char *data,
#ifdef __USE_EXE_FUNC
                     const void *exeFunc,
                     void *exeFuncParam
#else
                     const void *postFunc,
                     void *postFuncParam
#endif
){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = sz;
  if (data != nullptr) this->data.assign(data, data + sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
#else
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const std::vector <unsigned char> data,
#ifdef __USE_EXE_FUNC
                     const void *exeFunc,
                     void *exeFuncParam
#else
                     const void *postFunc,
                     void *postFuncParam
#endif
){
  this->type = static_cast<unsigned char>(type);
  if (data.size() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.size();
  if (data.size() > 0) this->data.assign(data.begin(), data.end());
#ifdef __USE_EXE_FUNC
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
#else
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const char *data,
#ifdef __USE_EXE_FUNC
                     const void *exeFunc,
                     void *exeFuncParam
#else
                     const void *postFunc,
                     void *postFuncParam
#endif
){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = strlen(data);
  if (data != nullptr) this->data.assign((const unsigned char *) data, (const unsigned char *) data + sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
#else
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
#endif
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const std::string data,
#ifdef __USE_EXE_FUNC
                     const void *exeFunc,
                     void *exeFuncParam
#else
                     const void *postFunc,
                     void *postFuncParam
#endif
){
  this->type = static_cast<unsigned char>(type);
  if (data.length() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.length();
  if (data.length() > 0) this->data.assign(data.c_str(), data.c_str() + this->sz);
#ifdef __USE_EXE_FUNC
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
#ifdef __USE_POST_FUNC
  this->postFunc = nullptr;
  this->postFuncParam = nullptr;
#endif
#else
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
#endif
  this->next = nullptr;
}
#endif

#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     size_t sz,
                     const unsigned char *data,
                     const void *exeFunc,
                     void *exeFuncParam,
                     const void *postFunc,
                     void *postFuncParam){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = sz;
  if (data != nullptr) this->data.assign(data, data + sz);
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const std::vector <unsigned char> data,
                     const void *exeFunc,
                     void *exeFuncParam,
                     const void *postFunc,
                     void *postFuncParam){
  this->type = static_cast<unsigned char>(type);
  if (data.size() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.size();
  if (data.size() > 0) this->data.assign(data.begin(), data.end());
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const char *data,
                     const void *exeFunc,
                     void *exeFuncParam,
                     const void *postFunc,
                     void *postFuncParam){
  this->type = static_cast<unsigned char>(type);
  if (data != nullptr) this->isReference = true;
  else this->isReference = false;
  this->sz = strlen(data);
  if (data != nullptr) this->data.assign((const unsigned char *) data, (const unsigned char *) data + this->sz);
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
  this->next = nullptr;
}

DataFrame::DataFrame(DataFrame::FRAME_TYPE_t type,
                     const std::string data,
                     const void *exeFunc,
                     void *exeFuncParam,
                     const void *postFunc,
                     void *postFuncParam){
  this->type = static_cast<unsigned char>(type);
  if (data.length() > 0) this->isReference = true;
  else this->isReference = false;
  this->sz = data.length();
  if (data.length() > 0) this->data.assign(data.c_str(), data.c_str() + this->sz);
  this->exeFunc = exeFunc;
  this->exeFuncParam = exeFuncParam;
  this->postFunc = postFunc;
  this->postFuncParam = postFuncParam;
  this->next = nullptr;
}
#endif

DataFrame::~DataFrame(){
  if (this->next != nullptr){
    delete this->next;
    this->next = nullptr;
  }
}

void DataFrame::setType(DataFrame::FRAME_TYPE_t type){
  this->type = static_cast<unsigned char>(type);
}

bool DataFrame::setSize(size_t sz){
  if (sz > 0x7FFFFFFF) return false;
  this->sz = sz;
  return true;
}

void DataFrame::setReference(const unsigned char *reference, size_t sz){
  this->isReference = true;
  this->data.assign(reference, reference + sz);
  this->sz = sz;
}

void DataFrame::setReference(const std::vector<unsigned char> reference){
  this->isReference = true;
  this->data.assign(reference.begin(), reference.end());
  this->sz = this->data.size();
}

void DataFrame::setReference(const char *reference){
  this->isReference = true;
  this->data.assign((const unsigned char *) reference, (const unsigned char *) reference + strlen(reference));
  this->sz = this->data.size();
}

void DataFrame::setReference(const std::string reference){
  this->isReference = true;
  this->data.assign(reference.c_str(), reference.c_str() + reference.length());
  this->sz = this->data.size();
}

void DataFrame::setData(const unsigned char *data, size_t sz){
  this->isReference = false;
  this->data.assign(data, data + sz);
  this->sz = sz;
}

void DataFrame::setData(const std::vector<unsigned char> data){
  this->isReference = false;
  this->data.assign(data.begin(), data.end());
  this->sz = this->data.size();
}

void DataFrame::setData(const char *data){
  this->isReference = false;
  this->data.assign((const unsigned char *) data, (const unsigned char *) data + strlen(data));
  this->sz = this->data.size();
}

void DataFrame::setData(const std::string data){
  this->isReference = false;
  this->data.assign((const unsigned char *) data.c_str(), (const unsigned char *) data.c_str() + data.length());
  this->sz = this->data.size();
}

#ifdef __USE_EXE_FUNC
void DataFrame::setExecuteFunction(const void *_func, void *_param){
  this->exeFunc = _func;
  this->exeFuncParam = _param;
}

void DataFrame::setExecuteFunction(DataFrame::FRAME_TYPE_t type, bool nullptrOnly, const void *_func, void *_param){
  DataFrame *tmp = this;
  while (tmp != nullptr){
    if (tmp->type == static_cast<unsigned char>(type)){
      if (nullptrOnly == true && tmp->exeFunc == nullptr){
        tmp->exeFunc = _func;
        tmp->exeFuncParam = _param;
      }
      else if (nullptrOnly == false){
        tmp->exeFunc = _func;
        tmp->exeFuncParam = _param;
      }
    }
    tmp = tmp->next;
  }
}

const void *DataFrame::getExecuteFunction(){
  return this->exeFunc;
}

void *DataFrame::getExecuteFunctionParam(){
  return this->exeFuncParam;
}
#endif

#ifdef __USE_POST_FUNC
void DataFrame::setPostExecuteFunction(const void *_func, void *_param){
  this->postFunc = _func;
  this->postFuncParam = _param;
}

void DataFrame::setPostExecuteFunction(DataFrame::FRAME_TYPE_t type, bool nullptrOnly, const void *_func, void *_param){
  DataFrame *tmp = this;
  while (tmp != nullptr){
    if (tmp->type == static_cast<unsigned char>(type)){
      if (nullptrOnly == true && tmp->postFunc == nullptr){
        tmp->postFunc = _func;
        tmp->postFuncParam = _param;
      }
      else if (nullptrOnly == false){
        tmp->postFunc = _func;
        tmp->postFuncParam = _param;
      }
    }
    tmp = tmp->next;
  }
}

const void *DataFrame::getPostExecuteFunction(){
  return this->postFunc;
}

void *DataFrame::getPostExecuteFunctionParam(){
  return this->postFuncParam;
}
#endif

DataFrame::FRAME_TYPE_t DataFrame::getType(){
  return static_cast<DataFrame::FRAME_TYPE_t>(this->type);
}

size_t DataFrame::getSize(){
  return this->sz;
}

size_t DataFrame::getReference(unsigned char *reference, size_t sizeOfReference){
  reference[0] = 0x00;
  if (this->isReference == false){
    return 0;
  }
  size_t size = (this->data.size() > sizeOfReference ? sizeOfReference : this->data.size());
  memcpy(reference, this->data.data(), size);
  return size;
}

size_t DataFrame::getReference(std::vector<unsigned char> &reference){
  reference.clear();
  if (this->isReference == false){
    return 0;
  }
  reference.assign(this->data.begin(), this->data.end());
  return this->data.size();
}

size_t DataFrame::getData(unsigned char *data, size_t sizeOfData){
  size_t size = (this->data.size() > sizeOfData ? sizeOfData : this->data.size());
  memcpy(data, this->data.data(), size);
  return size;
}

size_t DataFrame::getData(std::vector<unsigned char> &data){
  data.assign(this->data.begin(), this->data.end());
  return this->data.size();
}

std::vector <unsigned char> DataFrame::getDataAsVector(){
  std::vector <unsigned char> tmp;
  tmp.assign(this->data.begin(), this->data.end());
  return tmp;
}

std::string DataFrame::getDataFrameFormat(){
  std::string result = "";
  static char dthex[3];
  const DataFrame *tmp = this;
  dthex[2] = 0x00;
  while (tmp != nullptr){
    result += frameTypeStr[tmp->type] + "[size:" + std::to_string(tmp->sz) + "]:<<";
    for (auto i = tmp->data.begin(); i != tmp->data.end(); i++){
      sprintf(dthex, "%02X", static_cast<int>(*i));
      result += dthex;
    }
    result += ">>";
#ifdef __USE_EXE_FUNC
    result += "<<exeFunc:" + std::to_string((unsigned long long) tmp->exeFunc) + ">>";
#endif
#ifdef __USE_POST_FUNC
    result += "<<postFunc:" + std::to_string((unsigned long long) tmp->postFunc) + ">>";
#endif
    result += "\n";
    tmp = tmp->next;
  }
  return result;
}

DataFrame *DataFrame::getNext(){
  return this->next;
}

size_t DataFrame::getAllData(unsigned char *data, size_t sizeOfData){
  std::vector<unsigned char> dataTmp;
  size_t sz = this->getAllData(dataTmp);
  if (sizeOfData < sz) sz = sizeOfData;
  memcpy(data, dataTmp.data(), sz);
  return sz;
}

size_t DataFrame::getAllData(std::vector<unsigned char> &data){
  data.clear();
  const DataFrame *tmp = this;
  while (tmp != nullptr){
    data.insert(data.end(), tmp->data.begin(), tmp->data.end());
    tmp = tmp->next;
  }
  return data.size();
}

std::vector <unsigned char> DataFrame::getAllDataAsVector(){
  std::vector <unsigned char> tmp;
  this->getAllData(tmp);
  return tmp;
}

std::vector <unsigned char> DataFrame::getSpecificDataAsVector(const DataFrame *begin, const DataFrame *end){
  std::vector <unsigned char> data;
  const DataFrame *tmp = this;
  while(tmp != begin && tmp != nullptr){
    tmp = tmp->next;
  }
  while (tmp != nullptr){
    data.insert(data.end(), tmp->data.begin(), tmp->data.end());
    if (tmp == end) break;
    tmp = tmp->next;
  }
  return data;
}

#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
void DataFrame::execute(){
  DataFrame *tmp = this;
  while (tmp != nullptr){
#ifdef __USE_EXE_FUNC
    if (tmp->exeFunc != nullptr){
      void (*callback)(DataFrame &, void *) = (void (*)(DataFrame &, void *))tmp->exeFunc;
      callback(*tmp, tmp->exeFuncParam);
    }
#endif
#ifdef __USE_POST_FUNC
    if (tmp->postFunc != nullptr){
      void (*callback)(DataFrame &, void *) = (void (*)(DataFrame &, void *))tmp->postFunc;
      callback(*tmp, tmp->postFuncParam);
    }
#endif
    tmp = tmp->next;
  }
}
#endif

DataFrame& DataFrame::operator+=(const DataFrame &obj){
  DataFrame *tmp = this;
  while (tmp->next != nullptr){
    tmp = tmp->next;
  }
  const DataFrame *objTmp = &obj;
  while (objTmp != nullptr){
    tmp->next = new DataFrame(
      static_cast<DataFrame::FRAME_TYPE_t>(objTmp->type),
      objTmp->sz,
      objTmp->data.data()
#ifdef __USE_EXE_FUNC
      ,
      objTmp->exeFunc,
      objTmp->exeFuncParam
#endif
#ifdef __USE_POST_FUNC
      ,
      objTmp->postFunc,
      objTmp->postFuncParam
#endif
    );
    tmp = tmp->next;
    objTmp = objTmp->next;
  }
  return *this;
}

DataFrame& DataFrame::operator+(const DataFrame &obj){
  *this += obj;
  return *this;
}

DataFrame* DataFrame::operator[](int index){
  if (index < 0) return nullptr;
  if (index == 0) return this;
  DataFrame *tmp = this->next;
  int idx = 1;
  while (tmp != nullptr){
    if (idx == index) return tmp;
    tmp = tmp->next;
    idx++;
  }
  return nullptr;
}

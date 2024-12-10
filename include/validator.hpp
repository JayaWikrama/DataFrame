#ifndef __VALIDATOR_HPP__
#define __VALIDATOR_HPP__

#include <vector>
#include "data-frame.hpp"

class Validator {
  private:
    unsigned char type;
    std::vector <unsigned char> poly;
    std::vector <unsigned char> init;
    std::vector <unsigned char> xorOut;
    std::vector <unsigned char> lastChecksum;
    bool checksum_simple_additive_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
    bool checksum_xor_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
    bool checksum_fletcher_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
    bool crc8_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
    bool crc16_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
    bool crc32_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz);
  public:
    typedef enum _VALIDATOR_TYPE_t {
      VALIDATOR_TYPE_SIMPLE_ADDITIVE = 0,
      VALIDATOR_TYPE_XOR,
      VALIDATOR_TYPE_FLETCHER,
      VALIDATOR_TYPE_CRC8,
      VALIDATOR_TYPE_CRC16,
      VALIDATOR_TYPE_CRC32,
      VALIDATOR_TYPE_ADLER32
    } VALIDATOR_TYPE_t;

    Validator(VALIDATOR_TYPE_t type);

    bool setPoly(const unsigned char *poly, size_t sz);

    bool setInitialValue(const unsigned char *initialValue, size_t sz);

    bool setXorOut(const unsigned char *xorOut, size_t sz);

    std::vector <unsigned char> getChecksum(const unsigned char *data, size_t dataSz);

    std::vector <unsigned char> getChecksum(const std::vector <unsigned char> &data);

    std::vector <unsigned char> getChecksum(const DataFrame *begin, const DataFrame *end);

    std::vector <unsigned char> getChecksum(DataFrame &frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end);

    std::vector <unsigned char> getChecksum(DataFrame *frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end);

    bool validate(const unsigned char *ref, const unsigned char *data, size_t dataSz);

    bool validate(const unsigned char *ref, const std::vector <unsigned char> &data);

    bool validate(const unsigned char *ref, const DataFrame *begin, const DataFrame *end);

    bool validate(const unsigned char *ref, DataFrame &frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end);

    bool validate(const unsigned char *ref, DataFrame *frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end);

};

#endif
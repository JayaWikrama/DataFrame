#include <cstddef>
#include <cstring>
#include <iostream>
#include "validator.hpp"

bool Validator::checksum_simple_additive_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned int checksum = static_cast<int>(this->init[0]);
  for (size_t i = 0; i < dataSz; ++i) {
    checksum += data[i];
  }
  checksum = checksum % 256;
  return (static_cast<unsigned char>(checksum) == *checksum_ref);
}

bool Validator::checksum_xor_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned char checksum = this->init[0];
  for (size_t i = 0; i < dataSz; ++i) {
    checksum ^= data[i];
  }
  return (checksum == *checksum_ref);
}

bool Validator::checksum_fletcher_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned short sum1 = static_cast<unsigned short>(this->init[0]);
  unsigned short sum2 = static_cast<unsigned short>(this->init[1]);;
  for (size_t i = 0; i < dataSz; ++i) {
    sum1 = (sum1 + data[i]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }
  return (static_cast<unsigned char>(sum1) == checksum_ref[0]) && (static_cast<unsigned char>(sum2) == checksum_ref[1]);
}

bool Validator::crc8_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned char crc = this->init[0];
  unsigned char poly = this->poly[0];
  for (size_t i = 0; i < dataSz; ++i) {
    crc ^= data[i];
    for (unsigned char j = 0; j < 8; ++j) {
      crc = (crc & 0x80) ? ((crc << 1) ^ poly) : (crc << 1);
    }
  }
  crc ^= this->xorOut[0];
  return crc == *checksum_ref;
}

bool Validator::crc16_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned short crc = 0xFFFF;
  unsigned short poly = 0x8005;
  unsigned short xorOut = 0x0000;
  memcpy(&crc, this->init.data(), 2);
  memcpy(&poly, this->poly.data(), 2);
  memcpy(&xorOut, this->xorOut.data(), 2);
  for (size_t i = 0; i < dataSz; ++i) {
    crc ^= data[i] << 8;
    for (unsigned char j = 0; j < 8; ++j) {
      crc = (crc & 0x8000) ? (crc << 1) ^ poly : (crc << 1);
    }
  }
  crc ^= xorOut;
  unsigned short crc_ref = (checksum_ref[0] << 8) | checksum_ref[1];
  return crc == crc_ref;
}

bool Validator::crc32_check(const unsigned char *checksum_ref, const unsigned char *data, size_t dataSz){
  unsigned int crc = 0xFFFFFFFF;
  unsigned int poly = 0x04C11DB7;
  unsigned int xorOut = 0xFFFFFFFF;
  memcpy(&crc, this->init.data(), 4);
  memcpy(&poly, this->poly.data(), 4);
  memcpy(&xorOut, this->xorOut.data(), 4);
  for (size_t i = 0; i < dataSz; ++i) {
    crc ^= data[i] << 24;
    for (unsigned char j = 0; j < 8; ++j) {
      crc = (crc & 0x80000000) ? (crc << 1) ^ poly : (crc << 1);
    }
  }
  crc ^= xorOut;
  unsigned int crc_ref = (checksum_ref[0] << 24) | (checksum_ref[1] << 16) | (checksum_ref[2] << 8) | checksum_ref[3];
  return crc == crc_ref;
}

Validator::Validator(Validator::VALIDATOR_TYPE_t type){
  this->type = static_cast<unsigned int>(type);
  this->poly.clear();
  this->init.clear();
  this->xorOut.clear();
  switch (type){
    case Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE:
      this->init.push_back(0x00);
      break;
    case Validator::VALIDATOR_TYPE_XOR:
      this->init.push_back(0x00);
      break;
    case Validator::VALIDATOR_TYPE_FLETCHER:
      this->init.push_back(0x00);
      this->init.push_back(0x00);
      break;
    case Validator::VALIDATOR_TYPE_CRC8:
      this->init.push_back(0x00);
      this->poly.push_back(0x07);
      this->xorOut.push_back(0x00);
      break;
    case Validator::VALIDATOR_TYPE_CRC16:
      this->init.push_back(0xFF);
      this->init.push_back(0xFF);
      this->poly.push_back(0x05);
      this->poly.push_back(0x80);
      this->xorOut.push_back(0x00);
      this->xorOut.push_back(0x00);
      break;
    case Validator::VALIDATOR_TYPE_CRC32:
      this->init.push_back(0xFF);
      this->init.push_back(0xFF);
      this->init.push_back(0xFF);
      this->init.push_back(0xFF);
      this->poly.push_back(0xB7);
      this->poly.push_back(0x1D);
      this->poly.push_back(0xC1);
      this->poly.push_back(0x04);
      this->xorOut.push_back(0xFF);
      this->xorOut.push_back(0xFF);
      this->xorOut.push_back(0xFF);
      this->xorOut.push_back(0xFF);
      break;
    default:
      break;
  }
}

bool Validator::setPoly(const unsigned char *poly, size_t sz){
  bool isSzValid = false;
  switch (static_cast<Validator::VALIDATOR_TYPE_t>(type)){
    case Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_XOR:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_FLETCHER:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC8:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_CRC16:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC32:
      isSzValid = (sz == 4);
      break;
    default:
      break;
  }
  if (isSzValid == false){
    std::cout << __func__ << ": invalid size: " << std::to_string(sz) << std::endl;
    return false;
  }
  this->poly.assign(poly, poly + sz);
  return true;
}

bool Validator::setInitialValue(const unsigned char *initialValue, size_t sz){
  bool isSzValid = false;
  switch (static_cast<Validator::VALIDATOR_TYPE_t>(type)){
    case Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_XOR:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_FLETCHER:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC8:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_CRC16:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC32:
      isSzValid = (sz == 4);
      break;
    default:
      break;
  }
  if (isSzValid == false){
    std::cout << __func__ << ": invalid size: " << std::to_string(sz) << std::endl;
    return false;
  }
  this->init.assign(initialValue, initialValue + sz);
  return true;
}

bool Validator::setXorOut(const unsigned char *xorOut, size_t sz){
  bool isSzValid = false;
  switch (static_cast<Validator::VALIDATOR_TYPE_t>(this->type)){
    case Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_XOR:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_FLETCHER:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC8:
      isSzValid = (sz == 1);
      break;
    case Validator::VALIDATOR_TYPE_CRC16:
      isSzValid = (sz == 2);
      break;
    case Validator::VALIDATOR_TYPE_CRC32:
      isSzValid = (sz == 4);
      break;
    default:
      break;
  }
  if (isSzValid == false){
    std::cout << __func__ << ": invalid size: " << std::to_string(sz) << std::endl;
    return false;
  }
  this->xorOut.assign(xorOut, xorOut + sz);
  return true;
}

bool Validator::validate(const unsigned char *ref, const unsigned char *data, size_t dataSz){
  switch (static_cast<Validator::VALIDATOR_TYPE_t>(this->type)){
    case Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE:
      return checksum_simple_additive_check(ref, data, dataSz);
      break;
    case Validator::VALIDATOR_TYPE_XOR:
      return checksum_xor_check(ref, data, dataSz);
      break;
    case Validator::VALIDATOR_TYPE_FLETCHER:
      return checksum_fletcher_check(ref, data, dataSz);
      break;
    case Validator::VALIDATOR_TYPE_CRC8:
      return crc8_check(ref, data, dataSz);
      break;
    case Validator::VALIDATOR_TYPE_CRC16:
      return crc16_check(ref, data, dataSz);
      break;
    case Validator::VALIDATOR_TYPE_CRC32:
      return crc32_check(ref, data, dataSz);
      break;
    default:
      return false;
    break;
  }
  return false;
}

bool Validator::validate(const unsigned char *ref, const std::vector <unsigned char> &data){
  return this->validate(ref, data.data(), data.size());
}

bool Validator::validate(const unsigned char *ref, const DataFrame *begin, const DataFrame *end){
  DataFrame *frame = (DataFrame *) begin;
  std::vector <unsigned char> data = frame->getSpecificDataAsVector(begin, end);
  return this->validate(ref, data);
}

bool Validator::validate(const unsigned char *ref, DataFrame &frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end){
  
  DataFrame *fbegin = frame[begin];
  DataFrame *fend = frame[end];
  if (fbegin == nullptr || fend == nullptr){
    return false;
  }
  return this->validate(ref, fbegin, fend);
}

bool Validator::validate(const unsigned char *ref, DataFrame *frame, DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end){
  DataFrame *fbegin = (*frame)[begin];
  DataFrame *fend = (*frame)[end];
  if (fbegin == nullptr || fend == nullptr){
    return false;
  }
  return this->validate(ref, fbegin, fend);
}
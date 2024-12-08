#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>
#include <iostream>

#include "validator.hpp"

class ValidatorTest:public::testing::Test {
protected:
    DataFrame dataFrame;

    void SetUp() override {
        const unsigned char data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 
                                      0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
                                      0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 
                                      0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 
                                      0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 
                                      0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 
                                      0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 
                                      0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 
                                      0x7B, 0x7C
                                     };
        dataFrame.setData("\xAA\xBB\xCC\xDD");
        dataFrame += DataFrame(DataFrame::FRAME_TYPE_DATA, 124, data);
    }

    void TearDown() override {
        // do nothing
    }
};

/* Test Case for Simple Additive Validator */
TEST_F(ValidatorTest, SimpleAdditive_1) {
    Validator validator(Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x54", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, SimpleAdditive_2) {
    Validator validator(Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x54", data), true);
}

TEST_F(ValidatorTest, SimpleAdditive_3) {
    Validator validator(Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x54");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x54", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, SimpleAdditive_4) {
    Validator validator(Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x54");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x54", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, SimpleAdditive_5) {
    Validator validator(Validator::VALIDATOR_TYPE_SIMPLE_ADDITIVE);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x54");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x54", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

/* Test Case for XORing Validator */
TEST_F(ValidatorTest, XOR_1) {
    Validator validator(Validator::VALIDATOR_TYPE_XOR);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x7C", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, XOR_2) {
    Validator validator(Validator::VALIDATOR_TYPE_XOR);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x7C", data), true);
}

TEST_F(ValidatorTest, XOR_3) {
    Validator validator(Validator::VALIDATOR_TYPE_XOR);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x7C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x7C", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, XOR_4) {
    Validator validator(Validator::VALIDATOR_TYPE_XOR);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x7C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x7C", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, XOR_5) {
    Validator validator(Validator::VALIDATOR_TYPE_XOR);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x7C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x7C", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

/* Test Case for Fletcher Validator */
TEST_F(ValidatorTest, Fletcher_1) {
    Validator validator(Validator::VALIDATOR_TYPE_FLETCHER);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x75\x12", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, Fletcher_2) {
    Validator validator(Validator::VALIDATOR_TYPE_FLETCHER);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x75\x12", data), true);
}

TEST_F(ValidatorTest, Fletcher_3) {
    Validator validator(Validator::VALIDATOR_TYPE_FLETCHER);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x75\x12");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x75\x12", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, Fletcher_4) {
    Validator validator(Validator::VALIDATOR_TYPE_FLETCHER);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x75\x12");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x75\x12", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, Fletcher_5) {
    Validator validator(Validator::VALIDATOR_TYPE_FLETCHER);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x75\x12");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x75\x12", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

/* Test Case for CRC8 Validator */
TEST_F(ValidatorTest, CRC8_1) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC8);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x4C", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, CRC8_2) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC8);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x4C", data), true);
}

TEST_F(ValidatorTest, CRC8_3) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC8);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x4C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x4C", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, CRC8_4) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC8);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x4C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x4C", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC8_5) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC8);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x4C");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x4C", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

/* Test Case for CRC16 Validator */
TEST_F(ValidatorTest, CRC16_1) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\xAC\x86", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, CRC16_2) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\xAC\x86", data), true);
}

TEST_F(ValidatorTest, CRC16_3) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xAC\x86");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xAC\x86", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, CRC16_4) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xAC\x86");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xAC\x86", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC16_5) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xAC\x86");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xAC\x86", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC16_IBM_3740_1) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    unsigned short customPoly = 0x1021;
    validator.setPoly((const unsigned char *) &customPoly, 2);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x0A\x9E", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, CRC16_IBM_3740_2) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    unsigned short customPoly = 0x1021;
    validator.setPoly((const unsigned char *) &customPoly, 2);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x0A\x9E", data), true);
}

TEST_F(ValidatorTest, CRC16_IBM_3740_3) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    unsigned short customPoly = 0x1021;
    validator.setPoly((const unsigned char *) &customPoly, 2);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x0A\x9E");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x0A\x9E", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, CRC16_IBM_3740_4) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    unsigned short customPoly = 0x1021;
    validator.setPoly((const unsigned char *) &customPoly, 2);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x0A\x9E");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x0A\x9E", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC16_IBM_3740_5) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC16);
    unsigned short customPoly = 0x1021;
    validator.setPoly((const unsigned char *) &customPoly, 2);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x0A\x9E");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x0A\x9E", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

/* Test Case for CRC32 Validator */
TEST_F(ValidatorTest, CRC32_1) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\xC1\x6C\x6A\x05", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, CRC32_2) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\xC1\x6C\x6A\x05", data), true);
}

TEST_F(ValidatorTest, CRC32_3) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xC1\x6C\x6A\x05");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xC1\x6C\x6A\x05", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, CRC32_4) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xC1\x6C\x6A\x05");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xC1\x6C\x6A\x05", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC32_5) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\xC1\x6C\x6A\x05");
    ASSERT_EQ(validator.validate((const unsigned char *) "\xC1\x6C\x6A\x05", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC32_AIXM_1) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    unsigned int init = 0x00000000;
    unsigned int poly = 0x814141AB;
    unsigned int xorOut = 0x00000000;
    validator.setPoly((const unsigned char *) &poly, 4);
    validator.setInitialValue((const unsigned char *) &init, 4);
    validator.setXorOut((const unsigned char *) &xorOut, 4);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x3D\xAD\x90\xC1", data.data(), data.size()), true);
}

TEST_F(ValidatorTest, CRC32_AIXM_2) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    unsigned int init = 0x00000000;
    unsigned int poly = 0x814141AB;
    unsigned int xorOut = 0x00000000;
    validator.setPoly((const unsigned char *) &poly, 4);
    validator.setInitialValue((const unsigned char *) &init, 4);
    validator.setXorOut((const unsigned char *) &xorOut, 4);
    std::vector <unsigned char> data = dataFrame.getAllDataAsVector();
    ASSERT_EQ(data.size(), 128);
    ASSERT_EQ(validator.validate((const unsigned char *) "\x3D\xAD\x90\xC1", data), true);
}

TEST_F(ValidatorTest, CRC32_AIXM_3) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    unsigned int init = 0x00000000;
    unsigned int poly = 0x814141AB;
    unsigned int xorOut = 0x00000000;
    validator.setPoly((const unsigned char *) &poly, 4);
    validator.setInitialValue((const unsigned char *) &init, 4);
    validator.setXorOut((const unsigned char *) &xorOut, 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x3D\xAD\x90\xC1");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x3D\xAD\x90\xC1", dataFrame[DataFrame::FRAME_TYPE_START_BYTES], dataFrame[DataFrame::FRAME_TYPE_DATA]), true);
}

TEST_F(ValidatorTest, CRC32_AIXM_4) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    unsigned int init = 0x00000000;
    unsigned int poly = 0x814141AB;
    unsigned int xorOut = 0x00000000;
    validator.setPoly((const unsigned char *) &poly, 4);
    validator.setInitialValue((const unsigned char *) &init, 4);
    validator.setXorOut((const unsigned char *) &xorOut, 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x3D\xAD\x90\xC1");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x3D\xAD\x90\xC1", dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}

TEST_F(ValidatorTest, CRC32_AIXM_5) {
    Validator validator(Validator::VALIDATOR_TYPE_CRC32);
    unsigned int init = 0x00000000;
    unsigned int poly = 0x814141AB;
    unsigned int xorOut = 0x00000000;
    validator.setPoly((const unsigned char *) &poly, 4);
    validator.setInitialValue((const unsigned char *) &init, 4);
    validator.setXorOut((const unsigned char *) &xorOut, 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "\x3D\xAD\x90\xC1");
    ASSERT_EQ(validator.validate((const unsigned char *) "\x3D\xAD\x90\xC1", &dataFrame, DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA), true);
}
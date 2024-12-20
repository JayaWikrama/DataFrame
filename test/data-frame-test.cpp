#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>
#include <iostream>

#include "data-frame.hpp"

struct testPtr_t {
    unsigned char step;
    int type;
    std::string cst;
};

void exe_func(DataFrame &obj, void *ptr){
    if (ptr != nullptr){
        struct testPtr_t *sptr = (struct testPtr_t *) ptr;
        sptr->step = 1;
        sptr->type = static_cast<int>(obj.getType());
        sptr->cst = "exe";
    }
}

void post_exe_func(DataFrame &obj, void *ptr){
    if (ptr != nullptr){
        struct testPtr_t *sptr = (struct testPtr_t *) ptr;
        sptr->step = 2;
        sptr->type = static_cast<int>(obj.getType());
        sptr->cst += "post";
    }
}

void exe_func_custom(DataFrame &obj, void *ptr){
    if (ptr != nullptr){
        struct testPtr_t *sptr = (struct testPtr_t *) ptr;
        sptr->step = 1;
        sptr->type = static_cast<int>(obj.getType());
        sptr->cst += "exe->";
        sptr->cst += std::to_string(obj.getType());
    }
}

void post_exe_func_custom(DataFrame &obj, void *ptr){
    if (ptr != nullptr){
        struct testPtr_t *sptr = (struct testPtr_t *) ptr;
        sptr->step = 1;
        sptr->type = static_cast<int>(obj.getType());
        sptr->cst += "post->";
        sptr->cst += std::to_string(obj.getType());
    }
}

void crc16(DataFrame &frame, void *ptr) {
    /* Get object form function param */
    DataFrame *obj = (DataFrame *) ptr;
    /* Initialize crc16 param */
    unsigned short crc = 0x0000;
    unsigned short poly = 0x1021;
    /* Get data from Start Bytes until Data */
    std::vector <unsigned char> data = obj->getSpecificDataAsVector(obj, (*obj)[DataFrame::FRAME_TYPE_DATA]);
    /* Calculate crc16 */
    for (const auto &byte : data) {
        crc ^= (static_cast<unsigned short>(byte) << 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    /* Compare received CRC with calculated CRC */
    unsigned short rcvCRC = 0;
    frame.getData((unsigned char *) &rcvCRC, 2);
    if (rcvCRC != crc){
        /* invalid crc -> trigger stop flag to __readFramedData__ method */
        obj->trigInvDataIndicator();
    }
}

void setupLengthByCommand(DataFrame &frame, void *ptr){
    int data = 0;
    /* Get object form function param */
    DataFrame *obj = (DataFrame *) ptr;
    /* Get DataFrame target */
    DataFrame *target = (*obj)[DataFrame::FRAME_TYPE_DATA];
    if (target == nullptr) return;
    frame.getData((unsigned char *) &data, 1);
    if (data == 0x35){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(3);
    }
    else if (data == 0x36){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(2);
    }
    else {
        /* invalid value found -> trigger stop flag to __readFramedData__ method */
        obj->trigInvDataIndicator();
    }
}

class DataFrameTest:public::testing::Test {
protected:
    DataFrame dataFrame;
    struct testPtr_t testStruct;

    void SetUp() override {
        memset(&testStruct, 0x00, sizeof(testStruct));
    }

    void TearDown() override {
        // tidak ada kebutuhan post-set
    }
};

/* Test Case untuk Constructor */

TEST_F(DataFrameTest, DefaultConstructor1) {
    // Tes untuk default constructor
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dataFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 0);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR, 4);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_7) {
    std::vector <unsigned char> tmp(4);
    tmp[0] = static_cast<unsigned char>(0x0d);
    tmp[1] = static_cast<unsigned char>(0x0a);
    tmp[2] = static_cast<unsigned char>(0x0d);
    tmp[3] = static_cast<unsigned char>(0x0a);
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, tmp);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_8) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM2_9) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, std::string("\x0d\x0a\x0d\x0a"));
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>>\n"));
#endif
}

TEST_F(DataFrameTest, CustomConstructor_PM3_6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a");
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>>\n"));
#endif
}

#ifdef __USE_EXE_FUNC
TEST_F(DataFrameTest, CustomConstructor_PM4_1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_COMMAND));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_DATA));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_STOP_BYTES));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_VALIDATOR));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_7) {
    std::vector <unsigned char> tmp(4);
    tmp[0] = static_cast<unsigned char>(0x0d);
    tmp[1] = static_cast<unsigned char>(0x0a);
    tmp[2] = static_cast<unsigned char>(0x0d);
    tmp[3] = static_cast<unsigned char>(0x0a);
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, tmp, (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_8) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exe");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_9) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, std::string("\x0d\x0a\x0d\x0a"), (const void *) &exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_POST_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exe");
}
#elif defined(__USE_POST_FUNC)
TEST_F(DataFrameTest, CustomConstructor_PM4_1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_COMMAND));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_DATA));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_STOP_BYTES));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_VALIDATOR));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_7) {
    std::vector <unsigned char> tmp(4);
    tmp[0] = static_cast<unsigned char>(0x0d);
    tmp[1] = static_cast<unsigned char>(0x0a);
    tmp[2] = static_cast<unsigned char>(0x0d);
    tmp[3] = static_cast<unsigned char>(0x0a);
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, tmp, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_8) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, "\x0d\x0a\x0d\x0a", (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "post");
}

TEST_F(DataFrameTest, CustomConstructor_PM4_9) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, std::string("\x0d\x0a\x0d\x0a"), (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
#ifdef __USE_EXE_FUNC
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#else
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
#endif
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "post");
}
#endif

#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
TEST_F(DataFrameTest, CustomConstructor_PM5_1) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_2) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_3) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_COMMAND[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_COMMAND));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_4) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_DATA, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_DATA[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_DATA));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_5) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_STOP_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_STOP_BYTES));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_6) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_VALIDATOR, 4, (const unsigned char *) "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_VALIDATOR[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_VALIDATOR));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_7) {
    std::vector <unsigned char> tmp(4);
    tmp[0] = static_cast<unsigned char>(0x0d);
    tmp[1] = static_cast<unsigned char>(0x0a);
    tmp[2] = static_cast<unsigned char>(0x0d);
    tmp[3] = static_cast<unsigned char>(0x0a);
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, tmp, (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_8) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, "\x0d\x0a\x0d\x0a", (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exepost");
}

TEST_F(DataFrameTest, CustomConstructor_PM5_9) {
    DataFrame dFrame(DataFrame::FRAME_TYPE_START_BYTES, std::string("\x0d\x0a\x0d\x0a"), (const void *) &exe_func, (void *) &testStruct, (const void *) &post_exe_func, (void *) &testStruct);
    ASSERT_EQ(dFrame.getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dFrame.getSize(), 4);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dFrame.getReference(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getData(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, "\x0d\x0a\x0d\x0a\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dFrame.getDataFrameFormat(), std::string("FRAME_TYPE_START_BYTES[size:4]:<<0D0A0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    dFrame.execute();
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_START_BYTES));
    ASSERT_EQ(testStruct.cst, "exepost");
}
#endif

/* Test Case untuk Setter dan Getter */

TEST_F(DataFrameTest, SetterAndGetter_1) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setReference((const unsigned char *) "\x07\xFF", 2);
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x07);
    ASSERT_EQ(vec[1], 0xFF);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x07);
    ASSERT_EQ(vec[1], 0xFF);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x07\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x07\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_2) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setReference((const unsigned char *) "\x07\xFF\x0D\x0A", 4);
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 4);
    ASSERT_EQ(dataFrame.getReference(vec), 4);
    ASSERT_EQ(vec.size(), 4);
    ASSERT_EQ(vec[0], 0x07);
    ASSERT_EQ(vec[1], 0xFF);
    ASSERT_EQ(vec[2], 0x0D);
    ASSERT_EQ(vec[3], 0x0A);
    ASSERT_EQ(dataFrame.getData(vec), 4);
    ASSERT_EQ(vec.size(), 4);
    ASSERT_EQ(vec[0], 0x07);
    ASSERT_EQ(vec[1], 0xFF);
    ASSERT_EQ(vec[2], 0x0D);
    ASSERT_EQ(vec[3], 0x0A);
    unsigned char buffer[2];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x07\xFF", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x07\xFF", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<07FF0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<07FF0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<07FF0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:4]:<<07FF0D0A>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_3) {
    std::vector <unsigned char> vec;
    vec.push_back(0x7F);
    vec.push_back(0xFF);
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setReference(vec);
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
    vec.clear();
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x7F);
    ASSERT_EQ(vec[1], 0xFF);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x7F);
    ASSERT_EQ(vec[1], 0xFF);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x7F\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x7F\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_4) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setData((const unsigned char *) "\x07\xFF", 2);
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 0);
    ASSERT_EQ(vec.size(), 0);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x07);
    ASSERT_EQ(vec[1], 0xFF);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x07\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<07FF>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_5) {
    std::vector <unsigned char> vec;
    vec.push_back(0x7F);
    vec.push_back(0xFF);
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setData(vec);
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
    vec.clear();
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 0);
    ASSERT_EQ(vec.size(), 0);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x7F);
    ASSERT_EQ(vec[1], 0xFF);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x7F\xFF\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<7FFF>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_6) {
    dataFrame.setData((const unsigned char *) "1234", 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "5678") +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 5, (const unsigned char *) "QW\r\n\x00");
    std::vector <unsigned char> tmp;
    dataFrame.getAllData(tmp);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "12345678QW\r\n\x00", 9), 0);
}


TEST_F(DataFrameTest, SetterAndGetter_7) {
    dataFrame.setData((const unsigned char *) "1234", 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "5678") +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 5, (const unsigned char *) "QW\r\n\x00");
    unsigned char data[9];
    ASSERT_EQ(dataFrame.getAllData(data, sizeof(data)), 9);
    ASSERT_EQ(memcmp(data, (const unsigned char *) "12345678QW\r\n\x00", 9), 0);
}

TEST_F(DataFrameTest, SetterAndGetter_8) {
    dataFrame.setData((const unsigned char *) "1234", 4);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 4, (const unsigned char *) "5678") +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 5, (const unsigned char *) "QW\r\n\x00");
    unsigned char data[5];
    ASSERT_EQ(dataFrame.getAllData(data, sizeof(data)), 5);
    ASSERT_EQ(memcmp(data, (const unsigned char *) "12345", 5), 0);
}

TEST_F(DataFrameTest, SetterAndGetter_9) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setReference("\r\n");
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_10) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setSize(2);
    dataFrame.setReference(std::string("\r\n"));
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_11) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setData("\x0D\x0A");
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 0);
    ASSERT_EQ(vec.size(), 0);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_12) {
    std::vector <unsigned char> vec;
    dataFrame.setType(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame.setData(std::string("\x0D\x0A"));
#ifdef __USE_EXE_FUNC
    dataFrame.setExecuteFunction((const void *) &exe_func, (void *)&testStruct);
#endif
#ifdef __USE_POST_FUNC
    dataFrame.setPostExecuteFunction((const void *) &post_exe_func, (void *)&testStruct);
#endif
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(dataFrame.getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame.getSize(), 2);
    ASSERT_EQ(dataFrame.getReference(vec), 0);
    ASSERT_EQ(vec.size(), 0);
    ASSERT_EQ(dataFrame.getData(vec), 2);
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 0x0D);
    ASSERT_EQ(vec[1], 0x0A);
    unsigned char buffer[8];
    memset(buffer, 0x00, sizeof(buffer));
    ASSERT_EQ(dataFrame.getReference(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(memcmp(buffer, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
    ASSERT_EQ(dataFrame.getData(buffer, sizeof(buffer)), 2);
    ASSERT_EQ(memcmp(buffer, "\x0D\x0A\x00\x00\x00\x00\x00\x00", sizeof(buffer)), 0);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    ASSERT_EQ(testStruct.type, static_cast<int>(DataFrame::FRAME_TYPE_CONTENT_LENGTH));
#endif
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 2);
    ASSERT_EQ(testStruct.cst, "exepost");
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<exeFunc:" + std::to_string((unsigned long long) (const void *) &exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.step, 1);
    ASSERT_EQ(testStruct.cst, "exe");
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>><<postFunc:" + std::to_string((unsigned long long) (const void *) &post_exe_func) + ">>\n"));
    ASSERT_EQ(testStruct.cst, "post");
    ASSERT_EQ(testStruct.step, 2);
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(), std::string("FRAME_TYPE_CONTENT_LENGTH[size:2]:<<0D0A>>\n"));
    ASSERT_EQ(testStruct.cst, "");
    ASSERT_EQ(testStruct.step, 0);
#endif
}

TEST_F(DataFrameTest, SetterAndGetter_13) {
    std::vector <unsigned char> vec;
    ASSERT_EQ(dataFrame.setSize(0x7FFFFFFF), true);
    ASSERT_EQ(dataFrame.setSize(-1), false);
}

TEST_F(DataFrameTest, SetterAndGetter_14) {
    std::vector <unsigned char> vec;
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, "123") +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, "456") +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, "789") +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, "0-=") +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, "qwerty");
    vec = dataFrame[1]->getDataAsVector();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "123", 3), 0);
    vec = dataFrame[2]->getDataAsVector();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "456", 3), 0);
    vec = dataFrame[3]->getDataAsVector();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "789", 3), 0);
    vec = dataFrame[4]->getDataAsVector();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "0-=", 3), 0);
    vec = dataFrame[5]->getDataAsVector();
    ASSERT_EQ(vec.size(), 6);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "qwerty", 6), 0);
    DataFrame *begin = dataFrame[2];
    DataFrame *end = dataFrame[4];
    vec = dataFrame.getSpecificDataAsVector(begin, end);
    ASSERT_EQ(vec.size(), 9);
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "4567890-=", 9), 0);
}

/* Parse Test */

TEST_F(DataFrameTest, ParseTest_1) {
    std::vector <unsigned char> vec;
    dataFrame.setReference("1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcValidatorBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, (void *) &dataFrame);
    crcValidatorBytes.setPostExecuteFunction((const void *) &crc16, (void *) &dataFrame);
    dataFrame += cmdBytes + dataBytes + crcValidatorBytes + stopBytes;
    ASSERT_EQ(dataFrame.parse("\x31\x32\x33\x34\x35\x36\x37\x38\x15\x90\x39\x30\x2D\x3D"), true);
    vec = dataFrame.getAllDataAsVector();
    ASSERT_EQ(memcmp(vec.data(), (const unsigned char *) "\x31\x32\x33\x34\x35\x36\x37\x38\x15\x90\x39\x30\x2D\x3D", vec.size()), 0);
}

TEST_F(DataFrameTest, ParseTest_2) {
    std::vector <unsigned char> vec;
    dataFrame.setReference("1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcValidatorBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, (void *) &dataFrame);
    crcValidatorBytes.setPostExecuteFunction((const void *) &crc16, (void *) &dataFrame);
    dataFrame += cmdBytes + dataBytes + crcValidatorBytes + stopBytes;
    ASSERT_EQ(dataFrame.parse("\x31\x32\x33\x34\x37\x36\x37\x38\x15\x90\x39\x30\x2D\x3D"), false);
}

TEST_F(DataFrameTest, ParseTest_3) {
    std::vector <unsigned char> vec;
    dataFrame.setReference("1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcValidatorBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, (void *) &dataFrame);
    crcValidatorBytes.setPostExecuteFunction((const void *) &crc16, (void *) &dataFrame);
    dataFrame += cmdBytes + dataBytes + crcValidatorBytes + stopBytes;
    ASSERT_EQ(dataFrame.parse("\x31\x32\x33\x34\x35\x36\x37\x38\x15\x91\x39\x30\x2D\x3D"), false);
}

/* Test Operator Overloading */

TEST_F(DataFrameTest, OperatorOverloading_1) {
    dataFrame = DataFrame(DataFrame::FRAME_TYPE_COMMAND);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, OperatorOverloading_2) {
    dataFrame = DataFrame(DataFrame::FRAME_TYPE_COMMAND);
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_COMMAND[size:0]:<<>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>>\n"));
#endif
}

TEST_F(DataFrameTest, OperatorOverloading_3) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"));
#elif defined(__USE_EXE_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<exeFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<exeFunc:0>>\n"));
#elif defined(__USE_POST_FUNC)
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_START_BYTES[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_COMMAND[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>><<postFunc:0>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>><<postFunc:0>>\n"));
#else
    ASSERT_EQ(dataFrame.getDataFrameFormat(),
              std::string("FRAME_TYPE_START_BYTES[size:0]:<<>>\n"
                          "FRAME_TYPE_COMMAND[size:0]:<<>>\n"
                          "FRAME_TYPE_CONTENT_LENGTH[size:0]:<<>>\n"
                          "FRAME_TYPE_VALIDATOR[size:0]:<<>>\n"
                          "FRAME_TYPE_STOP_BYTES[size:0]:<<>>\n"));
#endif
}

#if defined(__USE_EXE_FUNC) && defined(__USE_POST_FUNC)
TEST_F(DataFrameTest, OperatorOverloading_4) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, (const void *) &exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr, (const void *) &post_exe_func, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, "exepostpostpost");
}
#elif defined(__USE_EXE_FUNC)
TEST_F(DataFrameTest, OperatorOverloading_4) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, (const void *) &exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, "exe");
}
#elif defined(__USE_POST_FUNC)
TEST_F(DataFrameTest, OperatorOverloading_4) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, (const void *) &post_exe_func, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, "postpostpostpost");
}
#else
TEST_F(DataFrameTest, OperatorOverloading_4) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr);
#if defined(__USE_EXE_FUNC) || defined(__USE_POST_FUNC)
    dataFrame.execute();
#endif
    ASSERT_EQ(testStruct.cst, "");
}
#endif

#ifdef __USE_EXE_FUNC
TEST_F(DataFrameTest, OperatorOverloading_5) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr);
    dataFrame.setExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, false, (const void *) &exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("exe->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND) + 
                              std::string("exe->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}

TEST_F(DataFrameTest, OperatorOverloading_6) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr);
    dataFrame.setExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, true, (const void *) &exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("exe->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND) + 
                              std::string("exe->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}

TEST_F(DataFrameTest, OperatorOverloading_7) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, (const void *) &exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr);
    dataFrame.setExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, true, (const void *) &exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("exe") +
                              std::string("exe->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}

#ifdef __USE_EXE_FUNC
TEST_F(DataFrameTest, OperatorOverloading_8) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    dataFrame.setPostExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, false, (const void *) &post_exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("post->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND) + 
                              std::string("post->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}

TEST_F(DataFrameTest, OperatorOverloading_9) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    dataFrame.setPostExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, true, (const void *) &post_exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("post->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND) + 
                              std::string("post->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}

TEST_F(DataFrameTest, OperatorOverloading_10) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, (const void *) &post_exe_func, &testStruct) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR, 0, nullptr, nullptr, nullptr, nullptr, nullptr) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES, 0, nullptr, nullptr, nullptr, nullptr, nullptr);
    dataFrame.setPostExecuteFunction(DataFrame::FRAME_TYPE_COMMAND, true, (const void *) &post_exe_func_custom, &testStruct);
    dataFrame.execute();
    ASSERT_EQ(testStruct.cst, std::string("post") +
                              std::string("post->") +
                              std::to_string(DataFrame::FRAME_TYPE_COMMAND));
}
#endif
#endif

TEST_F(DataFrameTest, OperatorOverloading_11) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dataFrame[0]->getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dataFrame[1]->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dataFrame[2]->getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame[3]->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dataFrame[4]->getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dataFrame[5]->getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dataFrame[-1], nullptr);
    ASSERT_EQ(dataFrame[6], nullptr);
}

TEST_F(DataFrameTest, OperatorOverloading_12) {
    dataFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND) +
                 DataFrame(DataFrame::FRAME_TYPE_CONTENT_LENGTH) +
                 DataFrame(DataFrame::FRAME_TYPE_COMMAND) +
                 DataFrame(DataFrame::FRAME_TYPE_VALIDATOR) +
                 DataFrame(DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_START_BYTES]->getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_COMMAND]->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_CONTENT_LENGTH]->getType(), DataFrame::FRAME_TYPE_CONTENT_LENGTH);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_VALIDATOR]->getType(), DataFrame::FRAME_TYPE_VALIDATOR);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_STOP_BYTES]->getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_SN], nullptr);
    ASSERT_EQ(dataFrame[DataFrame::FRAME_TYPE_RFU], nullptr);
}

TEST_F(DataFrameTest, OperatorOverloading_13) {
    DataFrame cmdBytes0(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame dataBytes0(DataFrame::FRAME_TYPE_DATA, 1);
    DataFrame cmdBytes1(DataFrame::FRAME_TYPE_COMMAND, 2);
    DataFrame dataBytes1(DataFrame::FRAME_TYPE_DATA, 2);
    DataFrame cmdBytes2(DataFrame::FRAME_TYPE_COMMAND, 3);
    DataFrame dataBytes2(DataFrame::FRAME_TYPE_DATA, 3);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    dataFrame += cmdBytes0 + dataBytes0 + cmdBytes1 + dataBytes1 + cmdBytes2 + dataBytes2 + stopBytes;
    DataFrame *frame = nullptr;
    frame = dataFrame[{DataFrame::FRAME_TYPE_COMMAND, 0}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 1);
    frame = dataFrame[{DataFrame::FRAME_TYPE_DATA, 0}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 1);
    frame = dataFrame[{DataFrame::FRAME_TYPE_COMMAND, 1}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 2);
    frame = dataFrame[{DataFrame::FRAME_TYPE_DATA, 1}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 2);
    frame = dataFrame[{DataFrame::FRAME_TYPE_COMMAND, 2}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 3);
    frame = dataFrame[{DataFrame::FRAME_TYPE_DATA, 2}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 3);
    frame = dataFrame[{DataFrame::FRAME_TYPE_STOP_BYTES}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(frame->getSize(), 4);
    frame = dataFrame[{DataFrame::FRAME_TYPE_VALIDATOR}];
    ASSERT_EQ(frame, nullptr);
}
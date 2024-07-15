#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>

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

class DataFrameTest:public::testing::Test {
protected:
    DataFrame dataFrame;
    struct testPtr_t testStruct;

    void SetUp() override {
        // tidak ada kebutuhan pre-set
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
#endif


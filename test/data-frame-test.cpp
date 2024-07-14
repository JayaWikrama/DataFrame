#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>

#include "data-frame.hpp"

class DataFrameTest:public::testing::Test {
protected:
    DataFrame dataFrame;

    void SetUp() override {
        // tidak ada kebutuhan pre-set
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



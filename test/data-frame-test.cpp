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

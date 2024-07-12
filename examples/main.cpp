#include <iostream>
#include <string.h>

#include "data-frame.hpp"

int main(int argc, char **argv){
    DataFrame myFrame(DataFrame::FRAME_TYPE_START_BYTES);
    myFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND);
    myFrame += DataFrame(DataFrame::FRAME_TYPE_DATA);
    myFrame += DataFrame(DataFrame::FRAME_TYPE_VALIDATOR);
    std::cout << myFrame.getDataFrameFormat() << std::endl;
    return 0;
}

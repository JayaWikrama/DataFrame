#include <iostream>
#include <string.h>

#include "data-frame.hpp"

void testExe(DataFrame &obj, void *param){
    std::cout << "[Exe] Type: " << obj.getType() << std::endl;
}

void testPost(DataFrame &obj, void *param){
    std::cout << "[Post] Type: " << obj.getType() << std::endl;
}

int main(int argc, char **argv){
    DataFrame myFrame(DataFrame::FRAME_TYPE_START_BYTES);
    myFrame += DataFrame(DataFrame::FRAME_TYPE_COMMAND) + DataFrame(DataFrame::FRAME_TYPE_DATA) + DataFrame(DataFrame::FRAME_TYPE_VALIDATOR);
    myFrame.setExecuteFunction((const void *) &testExe, nullptr);
    myFrame.setPostExecuteFunction((const void *) &testPost, nullptr);
    std::cout << myFrame.getDataFrameFormat() << std::endl;
    myFrame.execute();
    return 0;
}

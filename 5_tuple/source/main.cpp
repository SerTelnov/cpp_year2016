//
// Created by Sergey on 22.11.2017.
//

#ifdef _GTEST
#include <gtest/gtest.h>
#endif

int main(int argc, char *argv[])  {
#ifdef _GTEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

    return 0;
}
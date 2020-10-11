#include "config.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(configloaderTest, defConstructor)
{
    cfg::Config conf;
    ASSERT_EQ(conf.getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::WIDTH), 123);
}
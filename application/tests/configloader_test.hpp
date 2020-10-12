#include "config.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ConfigLoaderTest : public ::testing::Test
{
  protected:
    ConfigLoaderTest() {}
    virtual ~ConfigLoaderTest() {}
    void SetUp() { tc = new cfg::Config(); }
    void SetUp(const std::string &filename) { tc = new cfg::Config(filename); }
    virtual void TearDown() { delete tc; }
    cfg::Config *tc;
};

TEST_F(ConfigLoaderTest, defConstructorTest)
{
    std::cout << std::endl;
    EXPECT_NO_THROW(SetUp());
}

TEST_F(ConfigLoaderTest, properDemeanorConstructorTest)
{
    std::cout << std::endl;
    EXPECT_NO_THROW(SetUp((fs::current_path() /= "config/config.yaml").string()));
}

TEST_F(ConfigLoaderTest, exception1ConstructorTest)
{
    std::cout << std::endl;
    try
    {
        SetUp((fs::current_path() /= "badconfig/bconfig1.yaml").string());
    }
    catch (std::invalid_argument const &err)
    {
        EXPECT_EQ(err.what(), std::string("Missing 'application' parameter in 'appconfig.yaml'."));
    }
    catch (...)
    {
        FAIL() << "Expectedstd::invalid_argument";
    }
}

TEST_F(ConfigLoaderTest, exception2ConstructorTest)
{
    std::cout << std::endl;
    try
    {
        SetUp((fs::current_path() /= "badconfig/bconfig2.yaml").string());
    }
    catch (std::invalid_argument const &err)
    {
        EXPECT_EQ(err.what(), std::string("Missing 'window.height' parameter in 'appconfig.yaml'."));
    }
    catch (...)
    {
        FAIL() << "Expected std::invalid_argument";
    }
}

TEST_F(ConfigLoaderTest, exception3ConstructorTest)
{
    std::cout << std::endl;
    try
    {
        SetUp((fs::current_path() /= "badconfig/bconfig3.yaml").string());
    }
    catch (std::invalid_argument const &err)
    {
        EXPECT_EQ(err.what(), std::string("Missing 'window' parameter in 'appconfig.yaml'."));
    }
    catch (...)
    {
        FAIL() << "Expected std::invalid_argument";
    }
}

TEST_F(ConfigLoaderTest, getPathTest)
{
    std::cout << std::endl;
    SetUp();
    ASSERT_EQ(tc->getPath(), std::string((fs::current_path() /= "config").string()));
}

TEST_F(ConfigLoaderTest, getPropTest)
{
    std::cout << std::endl;
    SetUp();
    ASSERT_EQ(tc->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::HEIGHT), 321);
    ASSERT_EQ(tc->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::WIDTH), 123);
}

TEST_F(ConfigLoaderTest, setDefaultCfgTest)
{
    std::cout << std::endl;
    SetUp();
    tc->uploadDefaultCfg();
    ASSERT_EQ(tc->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::HEIGHT), 480);
    ASSERT_EQ(tc->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::WIDTH), 640);
    ASSERT_EQ(tc->getPath(), std::string(""));
    // Techniclally the config_file is NULL, however I can't find related assert.
}

TEST_F(ConfigLoaderTest, setIsCfgDefault)
{
    std::cout << std::endl;
    SetUp();
    ASSERT_FALSE(tc->isConfigDefault());
    tc->uploadDefaultCfg();
    ASSERT_TRUE(tc->isConfigDefault());
}

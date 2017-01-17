#include "gtest/gtest.h"
#include "config_parser.h"

// Nginx String Config Fixture based on lecture example 
class NginxStringConfigTest : public ::testing::Test {
protected:
  bool ParseString(const std::string config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }

  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST(NginxConfigParserTest, SimpleConfigExistingFile) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool is_file_parsed = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(is_file_parsed);
}

TEST(NginxConfigParserTest, SimpleConfigNonExistingFile) {
  NginxConfigParser parser;
  NginxConfig out_config;
  
  bool is_file_parsed = parser.Parse("non_existent_config", &out_config);
  
  EXPECT_FALSE(is_file_parsed);
}


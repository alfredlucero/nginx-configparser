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

  EXPECT_TRUE(is_file_parsed) << "Example config file can be parsed";
}

TEST(NginxConfigParserTest, SimpleConfigNonExistingFile) {
  NginxConfigParser parser;
  NginxConfig out_config;
  
  bool is_file_parsed = parser.Parse("non_existent_config", &out_config);
  
  EXPECT_FALSE(is_file_parsed) << "Config file is nonexistent and cannot be parsed.";
}

TEST(NginxConfigStatementTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  
  EXPECT_EQ(statement.ToString(0), "foo bar;\n") << "ToString function should convert the tokens to the statement: foo bar;\n";

}

TEST_F(NginxStringConfigTest, SimpleStatementConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement";
  EXPECT_EQ("bar", out_config_.statements_[0]->tokens_[1]);
}

TEST_F(NginxStringConfigTest, SimpleInvalidStatementConfig) {
  EXPECT_FALSE(ParseString("foo bar")) << "Config missing semicolon";
  EXPECT_FALSE(ParseString("foo 'bar;")) << "Config missing closed single quote";
  EXPECT_FALSE(ParseString("foo 'bar\";")) << "Config with improper closed quote";
}

TEST_F(NginxStringConfigTest, NestedStatementConfig) {
  EXPECT_TRUE(ParseString("simple { nested statement; }"));
  EXPECT_EQ(1, out_config_.statements_.size()) << "Config has one statement";
  EXPECT_EQ("simple", out_config_.statements_[0]->tokens_[0]); 
}

TEST_F(NginxStringConfigTest, InvalidNestedStatementConfig) {
  EXPECT_FALSE(ParseString("missing { semicolon }")) << "Nested config statement missing semicolon";
  EXPECT_FALSE(ParseString("missing { closing curly braces; ")) << "Nested config missing matching curly braces"; // Bug found here with unmatched curly braces TODO: fix in config_parser.cc
  EXPECT_FALSE(ParseString("improper } arrangement of curlies; {")) << "Nested config with curly braces in wrong closed/open order";
}

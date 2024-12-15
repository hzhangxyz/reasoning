#include "boost/locale/encoding_utf.hpp"
import std;
import reasoning.lisp_grammar;

struct lisp_grammar : reasoning::lisp_grammar<lisp_grammar, std::string>
{
  static auto u32_to_name(const auto& string)
  {
    return boost::locale::conv::utf_to_utf<char, char32_t>(string);
  }
  static auto name_to_u32(const auto& string)
  {
    return boost::locale::conv::utf_to_utf<char32_t, char>(string);
  }
};

auto
main() -> int
{
  auto file = std::ifstream("../database/命题逻辑.lisp_grammar");
  auto input = boost::locale::conv::utf_to_utf<char32_t, char>(std::string(
    std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
  auto it = input.begin();
  auto eof = input.end();
  while (it != eof) {
    std::shared_ptr<const lisp_grammar::Term> result;
    it = lisp_grammar::read_from(it, eof, result);
    std::u32string string;
    lisp_grammar::write_to(std::back_inserter(string), result);
    std::println("{}", boost::locale::conv::utf_to_utf<char, char32_t>(string));
  }
  return 0;
}

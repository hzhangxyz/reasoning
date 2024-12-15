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
  std::u32string input = U"(hello world (real world) (加 i2 (乘 i2 i5))) (a b)";
  std::shared_ptr<const lisp_grammar::Term> result;
  auto iter = input.begin();
  auto end = input.end();
  while (iter != end) {
    iter = lisp_grammar::read_from(iter, end, result);
    std::u32string output;
    lisp_grammar::write_to(std::back_inserter(output), result);
    std::println("{}", boost::locale::conv::utf_to_utf<char, char32_t>(output));
  }
  return 0;
}

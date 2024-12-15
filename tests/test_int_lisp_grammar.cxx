#include "boost/locale/encoding_utf.hpp"
import std;
import reasoning.lisp_grammar;

struct lisp_grammar : reasoning::lisp_grammar<lisp_grammar, int>
{
  static auto u32_to_name(const auto& string)
  {
    auto s = boost::locale::conv::utf_to_utf<char, char32_t>(string);
    auto in = std::istringstream(s);
    int result;
    in >> result;
    return result;
  }
  static auto name_to_u32(const auto& value)
  {
    auto output = std::ostringstream();
    output << value;
    return boost::locale::conv::utf_to_utf<char32_t, char>(output.str());
  }
};

auto
main() -> int
{
  std::u32string input = U"(1 2 (3 4) (5 6 (7 8 9))) (10 11)";
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

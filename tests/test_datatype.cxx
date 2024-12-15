import std;
import reasoning.datatype;

auto
main() -> int
{
  using datatype = reasoning::datatype<std::string>;
  auto 类型 = datatype::make_atom("类型");
  auto 自然数 = datatype::make_atom("自然数");
  auto 自然数是一类型 = datatype::make_list(类型, 自然数);
  auto 零 = datatype::make_atom("零");
  auto 零是一自然数 = datatype::make_list(自然数, 零);
  auto 后继 = datatype::make_atom("后继");
  auto 变量 = datatype::make_atom("变量");
  auto 变量是一自然数 = datatype::make_list(自然数, 变量);
  auto 变量后继 = datatype::make_list(后继, 变量);
  auto 变量后继是一自然数 = datatype::make_list(自然数, 变量后继);
  std::println("{}",
               std::same_as<std::shared_ptr<const datatype::Term>,
                            decltype(变量后继是一自然数)>);
  return 0;
}

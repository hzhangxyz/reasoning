export module reasoning.simple_engine;

import std;
import reasoning.datatype;

export namespace reasoning {
template<typename Name>
struct simple_reasoning
{
private:
  using Term = typename datatype<Name>::Term;
  using Atom = typename datatype<Name>::Atom;
  using List = typename datatype<Name>::List;

public:
  static auto 演绎(std::shared_ptr<const List> 新变量,
                   std::map<Name, std::shared_ptr<const Term>> 替换表,
                   std::shared_ptr<const Term> 事实)
    -> std::shared_ptr<const Term>
  {
  }
};
}

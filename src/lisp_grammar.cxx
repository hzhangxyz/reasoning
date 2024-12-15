module;
#define BOOST_SPIRIT_UNICODE
#include <boost/spirit/include/qi.hpp>

export module reasoning.lisp_grammar;

import std;
import reasoning.datatype;

export namespace reasoning {

template<typename Derived, typename Name>
struct lisp_grammar
{
private:
  using Skipper = boost::spirit::unicode::space_type;

public:
  using Term = typename datatype<Name>::Term;
  using Atom = typename datatype<Name>::Atom;
  using List = typename datatype<Name>::List;

private:
  template<typename Iterator>
  struct Parser
    : boost::spirit::qi::grammar<
        Iterator,
        std::shared_ptr<const typename datatype<Name>::Term>(),
        Skipper>
  {
    Parser()
      : Parser::base_type(term)
    {
      name = boost::spirit::qi::lexeme[+boost::spirit::unicode::alnum];
      group = +term;
      term = list | atom;
      atom = name[([](auto& attrib, auto& context) {
        boost::fusion::at_c<0>(context.attributes) =
          std::static_pointer_cast<const Term>(
            std::make_shared<const Atom>(Derived::u32_to_name(attrib)));
      })];
      list = (("(" >> group >> ")") |
              ("[" >> group >> "]"))[([](auto& attrib, auto& context) {
        boost::fusion::at_c<0>(context.attributes) =
          std::static_pointer_cast<const Term>(
            std::make_shared<const List>(attrib));
      })];
    }

    boost::spirit::qi::rule<Iterator, std::shared_ptr<const Term>(), Skipper>
      term;
    boost::spirit::qi::rule<Iterator, std::shared_ptr<const Term>(), Skipper>
      atom;
    boost::spirit::qi::rule<Iterator, std::shared_ptr<const Term>(), Skipper>
      list;
    boost::spirit::qi::rule<Iterator, std::u32string(), Skipper> name;
    boost::spirit::qi::
      rule<Iterator, std::list<std::shared_ptr<const Term>>(), Skipper>
        group;
  };

public:
  static auto write_to(std::output_iterator<char32_t> auto out,
                       const std::shared_ptr<const Term>& term) -> decltype(out)
  {
    if (term->is_atom())
      return write_to(out, std::dynamic_pointer_cast<const Atom>(term));
    else if (term->is_list())
      return write_to(
        out, std::dynamic_pointer_cast<const List>(term), /*first=*/true);
    else
      throw std::runtime_error("Invalid term");
  }

  static auto read_from(auto in,
                        auto eof,
                        std::shared_ptr<const Term>& term) -> decltype(in)
  {
    if (!boost::spirit::qi::phrase_parse(
          in, eof, Parser<decltype(in)>(), Skipper(), term))
      throw std::runtime_error("Unknown format");
    return in;
  }

private:
  static auto write_to(std::output_iterator<char32_t> auto out,
                       const std::shared_ptr<const Atom>& atom) -> decltype(out)
  {
    auto string = Derived::name_to_u32(atom->name);
    return std::copy(string.begin(), string.end(), out);
  }
  static auto write_to(std::output_iterator<char32_t> auto out,
                       const std::shared_ptr<const List>& list,
                       bool first) -> decltype(out)
  {
    if (first)
      out = std::format_to(out, "(");
    out = write_to(out, list->head);
    if (list->tail) {
      out = std::format_to(out, " ");
      out = write_to(out, list->tail, /*first=*/false);
    } else
      out = std::format_to(out, ")");
    return out;
  }
};

}

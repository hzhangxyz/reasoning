export module reasoning.datatype;

import std;

export namespace reasoning {
template<typename Name>
struct datatype
{
  struct Term : public std::enable_shared_from_this<Term>
  {
    virtual ~Term() {}
    virtual bool is_atom() const = 0;
    virtual bool is_list() const = 0;
  };
  struct Atom : public Term
  {
    Atom(Name&& n)
      : name(std::move(n))
    {
    }
    ~Atom() override {}
    bool is_atom() const override { return true; }
    bool is_list() const override { return false; }

    Name name;
  };
  struct List : public Term
  {
    List(std::ranges::range auto l)
      requires std::same_as<std::ranges::range_value_t<decltype(l)>,
                            std::shared_ptr<const Term>>
      : List(l.begin(), l.end())
    {
    }
    List(std::incrementable auto begin, std::incrementable auto end)
      requires std::same_as<decltype(begin), decltype(end)> &&
               std::same_as<std::iter_value_t<decltype(begin)>,
                            std::shared_ptr<const Term>>
    {
      head = *begin;
      ++begin;
      if (begin == end) {
        tail = nullptr;
      } else {
        tail = std::make_shared<const List>(begin, end);
      }
    }
    ~List() override {}
    bool is_atom() const override { return false; }
    bool is_list() const override { return true; }

    std::shared_ptr<const Term> head;
    std::shared_ptr<const List> tail;
  };

  static auto make_atom(Name&& name) -> std::shared_ptr<const Term>
  {
    return std::static_pointer_cast<const Term>(
      std::make_shared<const Atom>(std::move(name)));
  }
  static auto make_list(std::same_as<std::shared_ptr<const Term>> auto... list)
    -> std::shared_ptr<const Term>
  {
    return std::static_pointer_cast<const Term>(std::make_shared<const List>(
      std::initializer_list<std::shared_ptr<const Term>>{ list... }));
  }
};
}

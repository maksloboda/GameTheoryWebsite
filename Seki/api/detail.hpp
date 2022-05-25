#ifndef DETAIL_H
#define DETAIL_H
namespace api::detail {
template<typename T, typename U, size_t N>
std::optional<U> match(std::array<T, N> from, std::array<U, N> to, T what) {
  auto it = std::find(from.begin(), from.end(), what);
  if (it == from.end()) {
    return std::nullopt;
  }
  size_t idx = it - from.begin();
  return to[idx];
}

template<typename T, size_t idx, size_t total>
std::tuple<> convert_all_json(T &tuple, nlohmann::json &src_array) {
  static_assert(idx == total);
  return std::make_tuple();
}

template<typename T, size_t idx, size_t total, typename Arg, typename... Args>
std::tuple<Arg, Args...> convert_all_json(T &tuple, nlohmann::json &src_array) {

  Arg target = src_array[idx].get<Arg>();

  return std::tuple_cat(std::make_tuple(std::move(target)),
    convert_all_json<T, idx + 1, total, Args...>(tuple, src_array));

}

template<typename R, typename... Args>
std::string call_with_json(R (*f)(Args...), nlohmann::json &from) {
  constexpr size_t arg_count = sizeof...(Args);
  if (!from.is_array()) {
    throw std::runtime_error("Malformed call");
  }
  if (from.size() != arg_count) {
    std::stringstream ss;
    ss << "Missing arguments, expected " << arg_count << " got " << from.size();
    throw std::runtime_error(ss.str());
  }

  std::tuple<Args...> decoded_args = convert_all_json
    <decltype(decoded_args), 0, arg_count, Args...>(decoded_args, from);

  R result = std::apply(f, decoded_args);

  return nlohmann::json(result).dump();
}
}
#endif // DETAIL_H
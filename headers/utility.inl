
template <typename T>
std::string to_string(const T& value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}

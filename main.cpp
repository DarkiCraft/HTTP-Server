// testing main.cpp
// this will be replaced with the actual code later

// expected output:
// Found: Bob
// 42 is the Answer

#include <iostream>
#include <optional>
#include <vector>
#include <string>

std::optional<std::string> find_name(const std::vector<std::string>& names, const std::string& search) {
  for (const auto& name : names) {
    if (name == search) return name;
  }
  return std::nullopt;
}

int main() {
  std::vector<std::string> names = { "Alice", "Bob", "Charlie" };
  std::string query = "Bob";

  if (auto result = find_name(names, query); result) {
    std::cout << "Found: " << *result << '\n';
  } else {
    std::cout << "Not found\n";
  }

  std::pair<int, std::string> item = {42, "Answer"};
  auto [number, text] = item;
  std::cout << number << " is the " << text << '\n';
}

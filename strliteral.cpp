#include <array>
#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <locale>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>

static_assert(sizeof(char) == sizeof(std::uint8_t), "char not one wide");

struct char_map {
  std::uint8_t c2 : 4;
  std::uint8_t c1 : 4;
};

char map_hex(char num) {
  switch (num) {
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  case 3:
    return '3';
  case 4:
    return '4';
  case 5:
    return '5';
  case 6:
    return '6';
  case 7:
    return '7';
  case 8:
    return '8';
  case 9:
    return '9';
  case 10:
    return 'a';
  case 11:
    return 'b';
  case 12:
    return 'c';
  case 13:
    return 'd';
  case 14:
    return 'e';
  case 15:
    return 'f';
  default:
    std::cerr << "char bigger 15 passed to map_hex" << std::endl;
    std::abort();
  }
}

void failure() {
  std::cerr << "strliteral [-i infile] [-o outfile] variable_name" << std::endl;
  std::exit(1);
}

constexpr std::size_t to_output_size(std::size_t input_size) {
  std::size_t literal_size = 4 * input_size;
  std::size_t lines = input_size / 16;
  return literal_size + 3 * lines;
}

template <std::size_t input_buffersize = 4096>
void process_file(std::istream &in, std::ostream &out, std::string_view name) {
  static_assert(input_buffersize % 16 == 0,
                "input_buffersize should be a multitude of 16");
  std::array<char, input_buffersize> in_buffer;
  std::array<char, to_output_size(input_buffersize)> out_buffer;
  in.imbue(std::locale::classic());
  out.imbue(std::locale::classic());
  out << "#if defined _WIN32 || defined __CYGWIN__\n"
         "#define _STRLITERAL_EXPORT_SYMBOL __declspec(dllexport)\n"
         "#elif defined __GNUC__\n"
         "#define _STRLITERAL_EXPORT_SYMBOL [[gnu::visibility(\"default\")]]\n"
         "#else\n"
         "#define _STRLITERAL_EXPORT_SYMBOL\n"
         "#endif\n"
         "#include <cstdint>\n"
         "#include <cstdlib>\n"
         "_STRLITERAL_EXPORT_SYMBOL extern const constexpr std::uint8_t "
      << name << "[]=\n";
  std::size_t length = 0;
  while (in.good() && out.good()) {
    in.read(in_buffer.data(), input_buffersize);
    std::size_t in_max = in.gcount();
    std::size_t out_used = 0;
    if (in_max == 0) {
      out_buffer[out_used++] = '"';
      out_buffer[out_used++] = '"';
      out_buffer[out_used++] = '\n';
      length += in_max;
      out.write(out_buffer.data(), out_used);
      continue;
    }
    std::size_t in_used = 0;
    while (true) {
      out_buffer[out_used++] = '"';
      for (int x = 0; x < 16; x++) {
        out_buffer[out_used++] = '\\';
        out_buffer[out_used++] = 'x';
        char tmp_char = (in_buffer[in_used++]);
        char_map curr;
        std::memcpy(&curr, &tmp_char, sizeof(char));
        out_buffer[out_used++] = map_hex(curr.c1);
        out_buffer[out_used++] = map_hex(curr.c2);
        if (in_used >= in_max)
          goto loop_end;
      }
      out_buffer[out_used++] = '"';
      out_buffer[out_used++] = '\n';
    }
  loop_end:
    out_buffer[out_used++] = '"';
    out_buffer[out_used++] = '\n';
    length += in_max;
    out.write(out_buffer.data(), out_used);
  }
  if (in.eof() && out.good()) {
    out << ";extern const constexpr std::size_t " << name
        << "_size = " << length + 1
        << ";\n"
           "#undef _STRLITERAL_EXPORT_SYMBOL"
        << std::endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    failure();
  std::unique_ptr<std::ifstream> inf = nullptr;
  std::unique_ptr<std::ofstream> of = nullptr;
  std::reference_wrapper<std::istream> i(std::cin);
  std::reference_wrapper<std::ostream> o(std::cout);
  std::string name;
  for (int x = 1; x < argc; x++) {
    std::string str(argv[x]);
    if (str == "-o") {
      if (x + 2 < argc) {
        x++;
        { std::ofstream output(argv[x]); }
        of = std::make_unique<std::ofstream>(
            argv[x], std::ios_base::in | std::ios_base::binary);
        o = *of;
      } else
        failure();
    } else if (str == "-i") {
      if (x + 2 < argc) {
        x++;
        inf = std::make_unique<std::ifstream>(
            argv[x], std::ios_base::out | std::ios_base::trunc);
        i = *inf;
      } else
        failure();
    } else if ((x + 1) == argc) {
      name = std::move(str);
    } else
      failure();
  }
  process_file(i.get(), o.get(), name);
  return 0;
}

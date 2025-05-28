#ifndef IGOR_MDSPAN_TO_NPY_HPP_
#define IGOR_MDSPAN_TO_NPY_HPP_

#include <fstream>
#include <mdspan>

#include <Igor/Logging.hpp>

namespace Igor {

namespace detail {

// -------------------------------------------------------------------------------------------------
// NOTE: Use const references for compatibility with Igor::MdArray
template <std::floating_point Float, typename Extents, typename LayoutPolicy>
[[nodiscard]] auto write_npy_header(std::ostream& out,
                                    const std::mdspan<Float, Extents, LayoutPolicy>& data,
                                    const std::string& filename) noexcept -> bool {
  using namespace std::string_literals;
  static_assert(std::is_same_v<LayoutPolicy, std::layout_right> ||
                    std::is_same_v<LayoutPolicy, std::layout_left>,
                "LayoutPolicy must be layout_left or layout_right");

  // Write magic string
  constexpr std::streamsize magic_string_len = 6;
  if (!out.write("\x93NUMPY", magic_string_len)) {
    Igor::Warn("Could not write magic string to  {}: {}", filename, std::strerror(errno));
    return false;
  }

  // Write format version, use 1.0
  constexpr std::streamsize version_len = 2;
  if (!out.write("\x01\x00", version_len)) {
    Igor::Warn("Could not write version number to  {}: {}", filename, std::strerror(errno));
    return false;
  }

  // Length of length entry
  constexpr std::streamsize header_len_len = 2;

  // Create header
  std::string header = "{"s;

  // Data type
  header += "'descr': '<f"s + std::to_string(sizeof(Float)) + "', "s;
  // Data order, Fortran order (column major) or C order (row major)
  header += "'fortran_order': "s +
            (std::is_same_v<LayoutPolicy, std::layout_right> ? "False"s : "True"s) + ", "s;
  // Data shape
  IGOR_ASSERT(data.rank() >= 1UZ, "Rank must be at least one entry but is {}", data.rank());
  header += "'shape': ("s;
  for (size_t i = 0; i < data.rank(); ++i) {
    header += std::to_string(data.extent(i)) + ", "s;
  }
  header += "), "s;

  header += "}"s;

  // Pad header with spaces s.t. magic string, version, header length and header together are
  // divisible by 64
  for (auto preamble_len = magic_string_len + version_len + header_len_len + header.size() + 1;
       preamble_len % 64 != 0;
       preamble_len = magic_string_len + version_len + header_len_len + header.size() + 1) {
    header.push_back('\x20');
  }
  header.push_back('\n');

  // Write header length
  IGOR_ASSERT(
      header.size() <= std::numeric_limits<uint16_t>::max(),
      "Size cannot be larger than the max for an unsigned 16-bit integer as it is stored in one.");
  const auto header_len = static_cast<uint16_t>(header.size());
  if (!out.write(reinterpret_cast<const char*>(&header_len), header_len_len)) {  // NOLINT
    Igor::Warn("Could not write header length to  {}: {}", filename, std::strerror(errno));
    return false;
  }

  // Write header
  if (!out.write(header.data(), static_cast<std::streamsize>(header.size()))) {
    Igor::Warn("Could not write header to  {}: {}", filename, std::strerror(errno));
    return false;
  }

  return true;
}

}  // namespace detail

// -------------------------------------------------------------------------------------------------
// NOTE: Use const references for compatibility with Igor::MdArray
template <std::floating_point Float, typename Extents, typename LayoutPolicy>
[[nodiscard]] auto mdspan_to_npy(const std::mdspan<Float, Extents, LayoutPolicy>& data,
                                 const std::string& filename) -> bool {
  std::ofstream out(filename, std::ios::binary | std::ios::out);
  if (!out) {
    Igor::Warn("Could not open file `{}`: {}", filename, std::strerror(errno));
    return false;
  }

  if (!detail::write_npy_header(out, data, filename)) { return false; }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (!out.write(reinterpret_cast<const char*>(data.data_handle()),
                 static_cast<std::streamsize>(data.size() * sizeof(Float)))) {
    Igor::Warn("Could not write data to `{}`: {}", filename, std::strerror(errno));
    return false;
  }

  return true;
}

}  // namespace Igor

#endif  // IGOR_MDSPAN_TO_NPY_HPP_

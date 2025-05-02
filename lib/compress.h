#ifndef COMPRESS_H
#define COMPRESS_H

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace compress {

  class Compressor {
  public:
    template<typename T>
    Compressor(T impl) : pimpl_(std::make_unique<Model<T>>(std::move(impl))) {}

    std::vector<uint8_t> compress(const std::vector<uint8_t> &data) { return pimpl_->compress(data); }

    std::vector<uint8_t> decompress(const std::vector<uint8_t> &data) { return pimpl_->decompress(data); }

    Compressor(Compressor &&) noexcept = default;
    Compressor &operator=(Compressor &&) noexcept = default;

    Compressor(const Compressor &) = delete;
    Compressor &operator=(const Compressor &) = delete;

  private:
    struct Concept {
      virtual ~Concept() = default;
      virtual std::vector<uint8_t> compress(const std::vector<uint8_t> &) = 0;
      virtual std::vector<uint8_t> decompress(const std::vector<uint8_t> &) = 0;
    };

    template<typename T>
    struct Model : Concept {
      Model(T impl) : impl_(std::move(impl)) {}
      std::vector<uint8_t> compress(const std::vector<uint8_t> &data) override { return impl_.compress(data); }
      std::vector<uint8_t> decompress(const std::vector<uint8_t> &data) override { return impl_.decompress(data); }
      T impl_;
    };

    std::unique_ptr<Concept> pimpl_;
  };

  Compressor createRLECompressor();
  Compressor createLZWCompressor();
  Compressor createHuffmanCompressor();

} // namespace compress
#endif // COMPRESS_H

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "../Export/TLSHA256.h"

namespace tl {

    const std::vector<uint32_t> CTLSha256::initial_message_digest_{
        0x6a09e667, 0xbb67ae85, 0x3c6ef372,
        0xa54ff53a, 0x510e527f, 0x9b05688c,
        0x1f83d9ab, 0x5be0cd19
    };

    const std::vector<uint32_t> CTLSha256::add_constant_{
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    std::vector<uint8_t> CTLSha256::encrypt(std::vector<uint8_t> input_message) const
    {
        //! 文本预处理
        preprocessing(input_message);

        //! 将文本分解成连续的64Byte大小的数据块
        auto chunks = breakTextInto64ByteChunks(input_message);

        //! 由64Byte大小的数据块，构造出64个4Byte大小的字。然后进行循环迭代。
        std::vector<uint32_t> message_digest(initial_message_digest_); // 初始化信息摘要
        for (const auto& chunk : chunks)
        {
            transform(structureWords(chunk), message_digest);
        }

        //! 获取最终结果
        return produceFinalHashValue(message_digest);
    }

    std::string CTLSha256::getHexMessageDigest(const std::string& message) const
    {
        auto digest = encrypt(std::vector<uint8_t>(message.begin(), message.end()));

        std::ostringstream o_s;
        o_s << std::hex << std::setiosflags(std::ios::uppercase);
        for (auto c : digest) {
            o_s << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(c);
        }

        return o_s.str();
    }

    void CTLSha256::preprocessing(std::vector<uint8_t>& message) const
    {
        const auto original_bit_size = message.size() * 8;

        //! 附加填充比特
        auto remainder = message.size() % 64;
        auto origialSize = message.size();
        if (remainder < 56)
        {
            message.resize(message.size() + 56 - remainder, 0x00);
            message[origialSize] = 0x80; // ox80即10000000
        }
        else if (remainder == 56)
        {
            message.resize(message.size() + 64, 0x00);
            message[origialSize] = 0x80;
        }
        else
        {
            message.resize(message.size() + 120 - remainder, 0x00);
            message[origialSize] = 0x80;
        }

        //! 附加原始文本的长度值
        for (int i = 1; i <= 8; ++i)
        {
            message.emplace_back(static_cast<uint8_t>(original_bit_size >> (64 - 8 * i)));
        }
    }

    std::vector<std::vector<uint8_t>> CTLSha256::breakTextInto64ByteChunks(const std::vector<uint8_t>& message) const
    {
        if (0 != message.size() % 64)
        {
            std::ostringstream oss;
            oss << "invalid message size: " << message.size();
            throw std::invalid_argument(oss.str());
        }

        std::vector<std::vector<uint8_t>> chunks;
        auto quotient = message.size() / 64;
        for (size_t i = 0; i < quotient; ++i)
        {
            chunks.emplace_back(message.begin() + i * 64, message.begin() + (i + 1) * 64);
        }
        return chunks;
    }

    std::vector<uint32_t> CTLSha256::structureWords(const std::vector<uint8_t>& chunk) const
    {
        if (64 != chunk.size())
        {
            std::ostringstream oss;
            oss << "invalid chunk size: " << chunk.size();
            throw std::invalid_argument(oss.str());
        }

        std::vector<uint32_t> words(64);
        for (size_t i = 0; i < 16; ++i)
        {
            words[i] = (static_cast<uint32_t>(chunk[i * 4]) << 24) | (static_cast<uint32_t>(chunk[i * 4 + 1]) << 16) |
                (static_cast<uint32_t>(chunk[i * 4 + 2]) << 8) | static_cast<uint32_t>(chunk[i * 4 + 3]);
        }
        for (size_t i = 16; i < 64; ++i)
        {
            words[i] = smallSigma1(words[i - 2]) + words[i - 7] + smallSigma0(words[i - 15]) + words[i - 16];
        }
        return words;
    }

    void CTLSha256::transform(const std::vector<uint32_t>& words, std::vector<uint32_t>& message_digest) const
    {
        if (8 != message_digest.size() || 64 != words.size())
        {
            std::ostringstream oss;
            oss << "invalid message_digest size: " << message_digest.size() <<
                "Or invalid words size: " << words.size();
            throw std::invalid_argument(oss.str());
        }

        auto d = message_digest;
        for (int i = 0; i < 64; ++i)
        {
            uint32_t temp1 = d[7] + bigSigma1(d[4]) + ch(d[4], d[5], d[6]) + add_constant_[i] + words[i];
            uint32_t temp2 = bigSigma0(d[0]) + maj(d[0], d[1], d[2]);

            d[7] = d[6];
            d[6] = d[5];
            d[5] = d[4];
            d[4] = d[3] + temp1;
            d[3] = d[2];
            d[2] = d[1];
            d[1] = d[0];
            d[0] = temp1 + temp2;
        }

        for (int i = 0; i < 8; ++i)
        {
            message_digest[i] += d[i];
        }
    }

    std::vector<uint8_t> CTLSha256::produceFinalHashValue(const std::vector<uint32_t>& input) const
    {
        std::vector<uint8_t> output;
        for (auto it = input.begin(); it != input.end(); ++it)
        {
            for (int i = 0; i < 4; i++)
            {
                output.emplace_back(static_cast<uint8_t>((*it) >> (24 - 8 * i)));
            }
        }

        return output;
    }

}
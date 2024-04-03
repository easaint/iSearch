/*
 * Copyright (c)
 * Filename:    sha256.h
 * Brief:       SHA256算法实现
 * Depend:      C++11
 *
 * Note:        1、代码优化：使用单例模式；
 *              2、debug：支持加密空字符串；
 *              3、性能优化。
 *
 * Attention:   输入信息中有中文时，得到的数字指纹与使用其他工具得到数字指纹可能不相同。原因是不同平台中文的编码方式不同。
 * 使用方式：
 *    std::string message("abcxxx");
 *    std::string result = tl::CTLSha256::getInstance().getHexMessageDigest(message);
 */
#ifndef _TL_SHA_256_H
#define _TL_SHA_256_H

#include <cstdint>
#include <string>
#include <vector>

#include "TLCommonBase.h"

namespace tl {

    /**
 * @brief SHA256加密类
*/
    class TL_COMMON_API_ CTLSha256 {
    public:
        //! 获取单例
        inline static CTLSha256& getInstance()
        {
            static CTLSha256 instance;
            return instance;
        }

        /**
         * @brief: 使用SHA256算法，加密输入信息（获取数字指纹）
         * @param[in] message: 输入信息
         * @return: 摘要（数字指纹）
        */
        std::vector<uint8_t> encrypt(std::vector<uint8_t> message) const;

        /**
         * @brief: 获取十六进制表示的信息摘要（数字指纹）
         * @param[in] message: 输入信息
         * @return: 十六进制表示的信息摘要（数字指纹），一共64位
        */
        std::string getHexMessageDigest(const std::string& message) const;

    protected:
        /// SHA256算法中定义的6种逻辑运算 ///

        inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const
        {
            return (x & y) ^ ((~x) & z);
        }

        inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const
        {
            return (x & y) ^ (x & z) ^ (y & z);
        }

        inline uint32_t bigSigma0(uint32_t x) const
        {
            return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
        }

        inline uint32_t bigSigma1(uint32_t x) const
        {
            return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
        }

        inline uint32_t smallSigma0(uint32_t x) const
        {
            return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
        }

        inline uint32_t smallSigma1(uint32_t x) const
        {
            return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
        }

        /**
         * @brief: SHA256算法对输入信息的预处理，包括“附加填充比特”和“附加长度值”
                附加填充比特: 在报文末尾进行填充，先补第一个比特为1，然后都补0，直到长度满足对512取模后余数是448。需要注意的是，信息必须进行填充。
                附加长度值: 用一个64位的数据来表示原始消息（填充前的消息）的长度，并将其补到已经进行了填充操作的消息后面。
         * @param[in][out] message: 待处理的信息
        */
        void preprocessing(std::vector<uint8_t>& message) const;

        /**
         * @brief: 将信息分解成连续的64Byte大小的数据块
         * @param[in] message: 输入信息，长度为64Byte的倍数
         * @return: 输出数据块
        */
        std::vector<std::vector<uint8_t>> breakTextInto64ByteChunks(const std::vector<uint8_t>& message) const;

        /**
         * @brief: 由64Byte大小的数据块，构造出64个4Byte大小的字。
                构造算法：前16个字直接由数据块分解得到，其余的字由如下迭代公式得到：
                W[t] = smallSigma1(W[t-2]) + W[t-7] + smallSigma0(W[t-15]) + W[t-16]
         * @param[in] chunk: 输入数据块，大小为64Byte
         * @return: 输出字
        */
        std::vector<uint32_t> structureWords(const std::vector<uint8_t>& chunk) const;

        /**
         * @breif: 基于64个4Byte大小的字，进行64次循环加密
         * @param[in] words: 64个4Byte大小的字
         * @param[in][out] message_digest: 信息摘要
        */
        void transform(const std::vector<uint32_t>& words, std::vector<uint32_t>& message_digest) const;

        /**
         * @brief: 输出最终的哈希值（数字指纹）
         * @param[in] input: 步长为32bit的哈希值
         * @return: 步长为8bit的哈希值
        */
        std::vector<uint8_t> produceFinalHashValue(const std::vector<uint32_t>& input) const;

    private:
        /* 单例模式 */
        CTLSha256() = default;

        CTLSha256(const CTLSha256&) = delete;
        CTLSha256& operator=(const CTLSha256&) = delete;

        CTLSha256(CTLSha256&&) = delete;
        CTLSha256& operator=(CTLSha256&&) = delete;

        ~CTLSha256() = default;


        // 在SHA256算法中的初始信息摘要，这些常量是对自然数中前8个质数的平方根的小数部分取前32bit而来
        static const std::vector<uint32_t> initial_message_digest_;

        // 在SHA256算法中，用到64个常量，这些常量是对自然数中前64个质数的立方根的小数部分取前32bit而来
        static const std::vector<uint32_t> add_constant_;
    };
}

#endif
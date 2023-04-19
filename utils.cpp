/**
 * @file utils.cpp
 *
 * @author Adam King
 * @date 2023-04-19
 */

#include "utils.h"

#include <cstddef>

namespace utils {

    error::enmErrorType
        openFile(
            std::string const strFileName,
            std::ios_base::openmode enmMode,
            std::ifstream& objIfsOut
        ) {
        if (strFileName.empty()) {
            return error::enmErrorInvalidArg;
        }

        objIfsOut.open(
            strFileName,
            enmMode
        );
        if (!objIfsOut.is_open()) {
            return error::enmErrorFileDne;
        }

        return error::enmErrorNone;
    }

    error::enmErrorType
        trimWhitespace(
            std::string const strIn,
            std::string& strOut
        ) {
        std::size_t sizBegin = 0;
        std::size_t sizEnd = strIn.size();

        // Remove leading whitespace
        while ((sizBegin < sizEnd) && std::isspace(strIn[sizBegin])) {
            ++sizBegin;
        }

        // Remove trailing whitespace
        while ((sizBegin < sizEnd) && std::isspace(strIn[sizEnd - 1])) {
            --sizEnd;
        }

        // Copy the non-whitespace characters into the result string
        strOut = "";
        for (std::size_t sizIdx = sizBegin; sizEnd > sizIdx; ++sizIdx) {
            strOut += strIn[sizIdx];
        }

        return error::enmErrorNone;
    }

    error::enmErrorType
        tokenizeString(
            std::string const strIn,
            char const chrDelim,
            std::vector<std::string>& vctStrOut
        ) {
        if (strIn.empty()) {
            return error::enmErrorInvalidArg;
        }

        std::string strToken;
        size_t sizStart = 0;
        size_t sizEnd = 0;
        vctStrOut.clear();

        while (std::string::npos != (sizEnd = strIn.find(chrDelim, sizStart))) {
            strToken = strIn.substr(sizStart, sizEnd - sizStart);
            sizStart = sizEnd + 1;
            trimWhitespace(
                strToken,
                strToken
            );
            vctStrOut.push_back(strToken);
        }

        strToken = strIn.substr(sizStart);
        if (!strToken.empty()) {
            trimWhitespace(
                strToken,
                strToken
            );
            vctStrOut.push_back(strToken);
        }

        return error::enmErrorNone;
    }

    uint8_t
        popU8Le(
            char const*& ptrChrBufIn
        ) {
        uint8_t const u8l = static_cast<uint8_t>(ptrChrBufIn[0]);
        uint8_t const u8Out = static_cast<uint8_t>(
            (u8l << 0)
            );
        ptrChrBufIn += sizeof(uint8_t);
        return u8Out;
    }

    uint16_t
        popU16Le(
            char const*& ptrChrBufIn
        ) {
        uint8_t const u8h = static_cast<uint8_t>(ptrChrBufIn[1]);
        uint8_t const u8l = static_cast<uint8_t>(ptrChrBufIn[0]);
        uint16_t const u16Out = static_cast<uint16_t>(
            (u8h << 8) |
            (u8l << 0)
            );
        ptrChrBufIn += sizeof(uint16_t);
        return u16Out;
    }

    uint32_t
        popU32Le(
            char const*& ptrChrBufIn
        ) {
        uint8_t const u8hh = static_cast<uint8_t>(ptrChrBufIn[3]);
        uint8_t const u8hl = static_cast<uint8_t>(ptrChrBufIn[2]);
        uint8_t const u8lh = static_cast<uint8_t>(ptrChrBufIn[1]);
        uint8_t const u8ll = static_cast<uint8_t>(ptrChrBufIn[0]);
        uint32_t const u32Out = static_cast<uint32_t>(
            (u8hh << 24) |
            (u8hl << 16) |
            (u8lh << 8) |
            (u8ll << 0)
            );
        ptrChrBufIn += sizeof(uint32_t);
        return u32Out;
    }

    uint64_t
        popU64Le(
            char const*& ptrChrBufIn
        ) {
        uint8_t const u8hhh = static_cast<uint8_t>(ptrChrBufIn[7]);
        uint8_t const u8hhl = static_cast<uint8_t>(ptrChrBufIn[6]);
        uint8_t const u8hlh = static_cast<uint8_t>(ptrChrBufIn[5]);
        uint8_t const u8hll = static_cast<uint8_t>(ptrChrBufIn[4]);
        uint8_t const u8lhh = static_cast<uint8_t>(ptrChrBufIn[3]);
        uint8_t const u8lhl = static_cast<uint8_t>(ptrChrBufIn[2]);
        uint8_t const u8llh = static_cast<uint8_t>(ptrChrBufIn[1]);
        uint8_t const u8lll = static_cast<uint8_t>(ptrChrBufIn[0]);
        uint64_t const u64Out = static_cast<uint64_t>(
            (static_cast<uint64_t>(u8hhh) << 56) |
            (static_cast<uint64_t>(u8hhl) << 48) |
            (static_cast<uint64_t>(u8hlh) << 40) |
            (static_cast<uint64_t>(u8hll) << 32) |
            (static_cast<uint64_t>(u8lhh) << 24) |
            (static_cast<uint64_t>(u8lhl) << 16) |
            (static_cast<uint64_t>(u8llh) << 8) |
            (static_cast<uint64_t>(u8lll) << 0)
            );
        ptrChrBufIn += sizeof(uint64_t);
        return u64Out;
    }

    int8_t
        popI8Le(
            char const*& ptrChrBufIn
        ) {
        return static_cast<int8_t>(popU8Le(ptrChrBufIn));
    }

    int16_t
        popI16Le(
            char const*& ptrChrBufIn
        ) {
        return static_cast<int16_t>(popU16Le(ptrChrBufIn));
    }

    int32_t
        popI32Le(
            char const*& ptrChrBufIn
        ) {
        return static_cast<int32_t>(popU32Le(ptrChrBufIn));
    }

    int64_t
        popI64Le(
            char const*& ptrChrBufIn
        ) {
        return static_cast<int64_t>(popU64Le(ptrChrBufIn));
    }

}

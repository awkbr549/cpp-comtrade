/**
 * @file utils.h
 * @brief Helper functions for common actions.
 *
 * @author Adam King
 * @date 2023-04-19
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "error.h"
#include "types.h"

namespace utils {

    error::enmErrorType
        openFile(
            std::string const strFileName,
            std::ios_base::openmode enmMode,
            std::ifstream& objIfsOut
        );

    error::enmErrorType
        trimWhitespace(
            std::string const strIn,
            std::string& strOut
        );

    error::enmErrorType
        tokenizeString(
            std::string const strIn,
            char const chrDelim,
            std::vector<std::string>& vctStrOut
        );

    uint8_t
        popU8Le(
            char const*& ptrChrBufIn
        );

    uint16_t
        popU16Le(
            char const*& ptrChrBufIn
        );

    uint32_t
        popU32Le(
            char const*& ptrChrBufIn
        );

    uint64_t
        popU64Le(
            char const*& ptrChrBufIn
        );

    int8_t
        popI8Le(
            char const*& ptrChrBufIn
        );

    int16_t
        popI16Le(
            char const*& ptrChrBufIn
        );

    int32_t
        popI32Le(
            char const*& ptrChrBufIn
        );

    int64_t
        popI64Le(
            char const*& ptrChrBufIn
        );

}

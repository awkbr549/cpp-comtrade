/**
 * @file error.cpp
 *
 * @author Adam King
 * @date 2023-04-19
 */

#include "error.h"

#include <iostream>
#include <vector>

namespace error {

    enmErrorType
        printCodeIfError(
            enmErrorType const enmErrCode
        ) {
        if (
            (0 > enmErrCode)
            || (enmErrorTypeCount <= enmErrCode)
            ) {
            std::cerr << "Invalid error code." << std::endl;
        }

        static std::vector<std::string> const vctStrMessages = {
            // enmErrorNone
            "No error.",
            // enmErrorInvalidArg
            "Invalid argument.",
            // enmErrorFileDne
            "File does not exist.",
            // enmErrorNotImpl
            "Feature not implemented.",
            // emErrorOutOfOrder
            "Sequence out of order."
        };

        if (enmErrorNone != enmErrCode) {
            std::cerr << "! ! ! ERROR ! ! ! " << vctStrMessages[static_cast<size_t>(enmErrCode)]
                << std::endl;
        }

        return enmErrCode;
    }
}

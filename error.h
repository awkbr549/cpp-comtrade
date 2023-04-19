/**
 * @file error.h
 * @brief Error code defintions and functions.
 *
 * @author Adam King
 * @date 2023-04-19
 */

#pragma once

namespace error {

    enum enmErrorType {
        enmErrorNone,
        enmErrorInvalidArg,
        enmErrorFileDne,
        enmErrorNotImpl,
        emErrorOutOfOrder,

        enmErrorTypeCount
    };

    enmErrorType
        printCodeIfError(
            enmErrorType const enmErrCode
        );
}

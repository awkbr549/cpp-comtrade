/**
 * @file main.cpp
 *
 * @author Adam King
 * @date 2023-04-19
 */

#include <string>

#include "comtrade.h"
#include "error.h"
#include "types.h"
#include "utils.h"

int
main(
    void
) {
    // Example usage
    //
    // CFG: C:/path/to/file.cfg
    // DAT: C:/path/to/file.dat
    //
    // strFileNamePrefix = "C:/path/to/file";
    //
    std::string const strFileNamePrefix
        = "C:/path/to/file";

    /* Parse configuration file */
    comtrade::stcConfigFileType stcCfg{};
    error::enmErrorType enmErrParse = comtrade::parseConfigFile(
        strFileNamePrefix,
        stcCfg
    );
    if (error::enmErrorNone != enmErrParse) {
        error::printCodeIfError(enmErrParse);
        return EXIT_FAILURE;
    }

    /* Parse data file */
    comtrade::stcDataFileType stcDat{};
    enmErrParse = comtrade::parseDataFile(
        stcCfg,
        stcDat
    );
    if (error::enmErrorNone != enmErrParse) {
        error::printCodeIfError(enmErrParse);
        return EXIT_FAILURE;
    }

    /* Print results */
    if (true) {
        // Print configuration parsing results
        if (true) {
            enmErrParse = comtrade::printConfigInfo(
                stcCfg
            );
            if (error::enmErrorNone != enmErrParse) {
                error::printCodeIfError(enmErrParse);
                return EXIT_FAILURE;
            }
        }

        // Print data parsing results (sample)
        if (true) {
            enmErrParse = comtrade::printDataInfo(
                stcCfg,
                stcDat,
                1
            );
            if (error::enmErrorNone != enmErrParse) {
                error::printCodeIfError(enmErrParse);
                return EXIT_FAILURE;
            }
        }

        // Print data parsing results (channel)
        if (true) {
            enmErrParse = comtrade::printDataInfo(
                stcCfg,
                stcDat,
                "C_BUS2_BUS2C_2_2C_BUS2_A"
            );
            if (error::enmErrorNone != enmErrParse) {
                error::printCodeIfError(enmErrParse);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

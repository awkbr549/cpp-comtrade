/**
 * @file comtrade.h
 * @brief Definitions and helper function for parsing IEEE Std C37.111-1999 (R2005) files.
 *
 * This code was made using the following document:
 *     Identification: IEEE Std C37.111-1999
 *     Title: Common Format for Transient Data Exchange (COMTRADE) for Power Systems
 *     Revision: R2005
 *     Date: 02 Feb 2005
 *     URL: https://ieeexplore.ieee.org/document/798772
 *
 * @author Adam King
 * @date 2023-04-19
 */

#pragma once

#include <string>
#include <vector>

#include "error.h"
#include "types.h"
#include "vectorMap.h"

namespace comtrade {

    // See section `5. Configuration file` for configuration file information

    // See section `6. Data file` for data file information

    enum enmChannelType {
        enmChannelAnalog,
        enmChannelDigital,

        enmChannelTypeCount
    };

    struct stcChannelInfoType {
        uint32_t u32Index;
        std::string strName;
        char chrPhase;
        std::string strCircuitId;
    };

    // 5.3.3 --> An, ch_id, ph, ccbm, uu, a, b, skew, min, max, primary, secondary, PS
    struct stcAnalogChannelInfoType {
        // generic channel info
        stcChannelInfoType stcChannelInfo;

        // analog-specific channel info
        std::string strUnit;
        float64_t f64ConvA;
        float64_t f64ConvB;

        // ignoring skew, min, max, primary, secondary, PS
        //
        // note that skew, primary, secondary, and PS would be needed if using real-world data
    };

    // 5.3.4 --> Dn, ch_id, ph, ccbm, y
    struct stcDigitalChannelInfoType {
        // generic channel info
        stcChannelInfoType stcChannelInfo;

        // digital-specific channel info
        bool bInServiceState;
    };

    struct stcSamplingRateInfoType {
        float64_t f64SamplesPerSec;
        uint64_t u64LastSampleNumber;
    };

    struct stcDateTimeType {

        struct stcDateType {
            uint16_t u16Year;
            uint8_t u8Month;
            uint8_t u8Day;
        } stcDate;

        struct stcTimeType {
            uint8_t u8Hour;
            uint8_t u8Minute;
            float64_t f64Second;
        } stcTime;

    };

    enum enmDataFileFormatType {
        enmDataFileFormatAscii,
        enmDataFileFormatBinary,

        enmDataFileFormatTypeCount
    };

    struct stcConfigFileType {
        bool bInit = false;

        std::string strCfgFileName;
        std::string strDatFileName;

        // 5.3.1 --> station_name, rec_dev_id, rev_year
        std::string strStationName;
        std::string strDeviceId;
        std::uint16_t u16Version;

        // 5.3.2 --> TT, ##A, ##D
        std::uint32_t u32NumChannels;
        std::uint32_t u32NumAnaChannels;
        std::uint32_t u32NumDigChannels;

        vm::clsVectorMap<std::string, stcAnalogChannelInfoType> objVmAnalogChannelInfo;
        vm::clsVectorMap<std::string, stcDigitalChannelInfoType> objVmDigitalChannelInfo;

        float32_t f32Frequency;
        uint32_t u32NumSamplingRates;
        std::vector<stcSamplingRateInfoType> vctSamplingRateInfo;

        stcDateTimeType stcDateTimeStart;
        stcDateTimeType stcDateTimeTrigger;

        enmDataFileFormatType enmDataFileFormat;
        float64_t f64TimeMult;
    };

    struct stcAnalogDataType {
        int16_t i16DataRaw;
        float64_t f64Data;
    };

    struct stcDigitalDataType {
        uint16_t u16DataRaw;
        std::vector<bool> vctData;
    };

    struct stcSampleDataType {
        uint32_t u32SampleNumber;
        float64_t f64TimestampUs;

        vm::clsVectorMap<std::string, stcAnalogDataType*> objVmSampleAnaData;
        vm::clsVectorMap<std::string, stcDigitalDataType*> objVmSampleDigData;
    };

    struct stcDataFileType {
        bool bInit = false;

        bool bSimpleSampling;
        uint64_t u64TotalSamples;
        uint32_t u32SampleSizeBytes;

        uint32_t u32PrevSampleNumber;

        // Storage by sample
        std::vector<stcSampleDataType> vctSampleData;

        // Storage by channel
        vm::clsVectorMap<std::string, std::vector<stcAnalogDataType*>*> objVmChanAnaData;
        vm::clsVectorMap<std::string, std::vector<stcDigitalDataType*>*> objVmChanDigData;
    };

    error::enmErrorType
        parseConfigFile(
            std::string const& strFileNamePrefix,
            stcConfigFileType& stcCfgOut
        );

    error::enmErrorType
        printConfigInfo(
            stcConfigFileType const& stcCfg
        );

    error::enmErrorType
        parseDataFile(
            stcConfigFileType const& stcCfgIn,
            stcDataFileType& stcDatOut
        );

    error::enmErrorType
        printDataInfo(
            stcConfigFileType const& stcCfg,
            stcDataFileType const& stcDat,
            uint64_t const u64SampleNumber
        );

    error::enmErrorType
        printDataInfo(
            stcConfigFileType const& stcCfg,
            stcDataFileType const& stcDat,
            std::string const& strChanName
        );

}

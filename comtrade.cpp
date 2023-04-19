/**
 * @file comtrade.cpp
 *
 * @author Adam King
 * @date 2023-04-19
 */

#include "comtrade.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include "utils.h"

namespace comtrade {

    namespace {

        // Private variables

        std::vector<std::string> const vctStrValidEngrUnits = {
            "V",
            "A"
        };

        std::map<std::string, float64_t> const mapUnitToConversion = {
            {"Y", 1.0e+24},
            {"Z", 1.0e+21},

            {"E", 1.0e+18},
            {"P", 1.0e+15},
            {"T", 1.0e+12},

            {"G", 1.0e+09},
            {"M", 1.0e+06},
            {"k", 1.0e+03},

            {"h", 1.0e+02},
            {"da", 1.0e+01},

            {"", 1.0e+00},

            {"d", 1.0e-01},
            {"c", 1.0e-02},
            {"m", 1.0e-03},

            {"u", 1.0e-06},
            {"�", 1.0e-06},

            {"n", 1.0e-09},
            {"p", 1.0e-12},
            {"f", 1.0e-15},
            {"a", 1.0e-18},
            {"z", 1.0e-21},
            {"y", 1.0e-24}
        };
    }

    error::enmErrorType
        parseConfigFile(
            std::string const& strFileNamePrefix,
            stcConfigFileType& stcCfgOut
        ) {
        if (strFileNamePrefix.empty()) {
            return error::enmErrorInvalidArg;
        }

        std::cout << "? ? ? INFO ? ? ? Parsing configuration file...";

        /* Un-initialize configuration data */
        stcCfgOut.bInit = false;

        /* Open configuration file */
        std::string strCfgFileName = std::string(strFileNamePrefix);
        strCfgFileName.append(".CFG");
        std::ifstream objIfsCfg;
        error::enmErrorType enmErrOpen = utils::openFile(
            strCfgFileName,
            std::ifstream::in,
            objIfsCfg
        );
        if (error::enmErrorNone != enmErrOpen) {
            objIfsCfg.close();
            return enmErrOpen;
        }

        /* Validate data file */
        std::string strDatFileName = std::string(strFileNamePrefix);
        strDatFileName.append(".DAT");
        std::ifstream objIfsDat;
        enmErrOpen = utils::openFile(
            strDatFileName,
            std::ifstream::in,
            objIfsDat
        );
        objIfsDat.close();
        if (error::enmErrorNone != enmErrOpen) {
            return enmErrOpen;
        }

        /* Save file names */
        stcCfgOut.strCfgFileName = strCfgFileName;
        stcCfgOut.strDatFileName = strDatFileName;

        /* Variables for parsing */
        error::enmErrorType enmErrRet = error::enmErrorNone;
        std::string strLine;
        char const chrDelim = ',';
        std::vector<std::string> vctStrTokens;

        /* Parse station, device, and version */
        std::getline(objIfsCfg, strLine);
        error::enmErrorType enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        stcCfgOut.strStationName = vctStrTokens[0];
        stcCfgOut.strDeviceId = vctStrTokens[1];
        stcCfgOut.u16Version = static_cast<uint16_t>(std::stoi(vctStrTokens[2]));

        /* Parse channel counts */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        stcCfgOut.u32NumChannels = static_cast<uint32_t>(std::stoi(vctStrTokens[0]));
        stcCfgOut.u32NumAnaChannels = static_cast<uint32_t>(std::stoi(vctStrTokens[1]));
        stcCfgOut.u32NumDigChannels = static_cast<uint32_t>(std::stoi(vctStrTokens[2]));

        /* Parse analog channel information */
        {
            size_t const sizNumAnaChan = static_cast<size_t>(stcCfgOut.u32NumAnaChannels);
            for (size_t sizIter = 0; sizNumAnaChan > sizIter; ++sizIter) {
                std::getline(objIfsCfg, strLine);
                enmErrTok = utils::tokenizeString(
                    strLine,
                    chrDelim,
                    vctStrTokens
                );
                if (error::enmErrorNone != enmErrTok) {
                    enmErrRet = enmErrTok;
                    goto cleanup;
                }

                stcAnalogChannelInfoType stcAnaChanInfo{};
                stcAnaChanInfo.stcChannelInfo.u32Index = static_cast<uint32_t>(std::stoi(vctStrTokens[0]));
                stcAnaChanInfo.stcChannelInfo.strName = vctStrTokens[1];
                stcAnaChanInfo.stcChannelInfo.chrPhase = static_cast<char>(vctStrTokens[2].front());
                stcAnaChanInfo.stcChannelInfo.strCircuitId = vctStrTokens[3];
                stcAnaChanInfo.strUnit = vctStrTokens[4];
                stcAnaChanInfo.f64ConvA = std::stod(vctStrTokens[5]);
                stcAnaChanInfo.f64ConvB = std::stod(vctStrTokens[6]);
                stcCfgOut.objVmAnalogChannelInfo.insert(
                    stcAnaChanInfo.stcChannelInfo.strName,
                    stcAnaChanInfo
                );

            }
        }

        /* Parse digital channel information */
        {
            size_t const sizNumDigChan = static_cast<size_t>(stcCfgOut.u32NumDigChannels);
            if (0 < sizNumDigChan) {
                std::cerr << "? ? ? INFO ? ? ? Digital channels not supported. Ignoring..." << std::endl;
            }
            for (size_t sizIter = 0; sizNumDigChan > sizIter; ++sizIter) {
                std::getline(objIfsCfg, strLine);
            }
        }

        /* Parse mains frequency */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        stcCfgOut.f32Frequency = std::stof(vctStrTokens[0]);

        /* Parse sampling rate count */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        stcCfgOut.u32NumSamplingRates = static_cast<uint32_t>(std::stoi(vctStrTokens[0]));

        /* Parse sampling rates and sample counts */
        {
            size_t const sizNumSamplingRates = static_cast<size_t>(stcCfgOut.u32NumSamplingRates);
            for (size_t sizIter = 0; sizNumSamplingRates > sizIter; ++sizIter) {
                std::getline(objIfsCfg, strLine);
                enmErrTok = utils::tokenizeString(
                    strLine,
                    chrDelim,
                    vctStrTokens
                );
                if (error::enmErrorNone != enmErrTok) {
                    enmErrRet = enmErrTok;
                    goto cleanup;
                }

                stcCfgOut.vctSamplingRateInfo.push_back(stcSamplingRateInfoType{});
                stcCfgOut.vctSamplingRateInfo[sizIter].f64SamplesPerSec = std::stod(vctStrTokens[0]);
                stcCfgOut.vctSamplingRateInfo[sizIter].u64LastSampleNumber = static_cast<uint64_t>(std::stoull(vctStrTokens[1]));
            }
        }

        /* Parse starting timestamp */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        // Parse starting date
        {
            std::vector<std::string> vctStrDate;
            enmErrTok = utils::tokenizeString(
                vctStrTokens[0],
                '/',
                vctStrDate
            );
            if (error::enmErrorNone != enmErrTok) {
                enmErrRet = enmErrTok;
                goto cleanup;
            }
            stcCfgOut.stcDateTimeStart.stcDate.u8Day = static_cast<uint8_t>(std::stoi(vctStrDate[0]));
            stcCfgOut.stcDateTimeStart.stcDate.u8Month = static_cast<uint8_t>(std::stoi(vctStrDate[1]));
            stcCfgOut.stcDateTimeStart.stcDate.u16Year = static_cast<uint16_t>(std::stoi(vctStrDate[2]));
        }
        // Parse starting time
        {
            std::vector<std::string> vctStrTime;
            enmErrTok = utils::tokenizeString(
                vctStrTokens[1],
                ':',
                vctStrTime
            );
            if (error::enmErrorNone != enmErrTok) {
                enmErrRet = enmErrTok;
                goto cleanup;
            }
            stcCfgOut.stcDateTimeStart.stcTime.u8Hour = static_cast<uint8_t>(std::stoi(vctStrTime[0]));
            stcCfgOut.stcDateTimeStart.stcTime.u8Minute = static_cast<uint8_t>(std::stoi(vctStrTime[1]));
            stcCfgOut.stcDateTimeStart.stcTime.f64Second = std::stod(vctStrTime[2]);
        }

        /* Parse trigger timestamp */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        // Parse trigger date
        {
            std::vector<std::string> vctStrDate;
            enmErrTok = utils::tokenizeString(
                vctStrTokens[0],
                '/',
                vctStrDate
            );
            if (error::enmErrorNone != enmErrTok) {
                enmErrRet = enmErrTok;
                goto cleanup;
            }
            stcCfgOut.stcDateTimeTrigger.stcDate.u8Day = static_cast<uint8_t>(std::stoi(vctStrDate[0]));
            stcCfgOut.stcDateTimeTrigger.stcDate.u8Month = static_cast<uint8_t>(std::stoi(vctStrDate[1]));
            stcCfgOut.stcDateTimeTrigger.stcDate.u16Year = static_cast<uint16_t>(std::stoi(vctStrDate[2]));
        }
        // Parse trigger time
        {
            std::vector<std::string> vctStrTime;
            enmErrTok = utils::tokenizeString(
                vctStrTokens[1],
                ':',
                vctStrTime
            );
            if (error::enmErrorNone != enmErrTok) {
                enmErrRet = enmErrTok;
                goto cleanup;
            }
            stcCfgOut.stcDateTimeTrigger.stcTime.u8Hour = static_cast<uint8_t>(std::stoi(vctStrTime[0]));
            stcCfgOut.stcDateTimeTrigger.stcTime.u8Minute = static_cast<uint8_t>(std::stoi(vctStrTime[1]));
            stcCfgOut.stcDateTimeTrigger.stcTime.f64Second = std::stod(vctStrTime[2]);
        }

        /* Parse data file format */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        if (0 == vctStrTokens[0].compare("ASCII")) {
            stcCfgOut.enmDataFileFormat = enmDataFileFormatAscii;
        }
        else if (0 == vctStrTokens[0].compare("BINARY")) {
            stcCfgOut.enmDataFileFormat = enmDataFileFormatBinary;
        }
        else {
            enmErrRet = error::enmErrorInvalidArg;
            goto cleanup;
        }

        /* Parse time base multiplier */
        std::getline(objIfsCfg, strLine);
        enmErrTok = utils::tokenizeString(
            strLine,
            chrDelim,
            vctStrTokens
        );
        if (error::enmErrorNone != enmErrTok) {
            enmErrRet = enmErrTok;
            goto cleanup;
        }
        stcCfgOut.f64TimeMult = std::stod(vctStrTokens[0]);

        /* Mark initialized */
        stcCfgOut.bInit = true;

    cleanup:
        std::cout << " Done." << std::endl;
        objIfsCfg.close();
        return printCodeIfError(enmErrRet);
    }

    error::enmErrorType
        printConfigInfo(
            stcConfigFileType const& stcCfg
        ) {
        std::cout << std::endl;

        std::cout << "Station: " << stcCfg.strStationName << std::endl;
        std::cout << "Device ID: " << stcCfg.strDeviceId << std::endl;
        std::cout << "Version: " << stcCfg.u16Version << std::endl;
        std::cout << std::endl;

        std::cout << "Total Channel Count: " << stcCfg.u32NumChannels << std::endl;
        std::cout << "Analog Channel Count: " << stcCfg.u32NumAnaChannels << std::endl;
        std::cout << "Digital Channel Count: " << stcCfg.u32NumDigChannels << std::endl;
        std::cout << std::endl;

        std::cout << "Analog Channel Info:" << std::endl;
        size_t const sizNumAnaChan = static_cast<size_t>(stcCfg.u32NumAnaChannels);
        for (size_t sizIter = 0; sizNumAnaChan > sizIter; ++sizIter) {
            std::cout << "\tIndex: " << stcCfg.objVmAnalogChannelInfo[sizIter].stcChannelInfo.u32Index << std::endl;
            std::cout << "\t\tName: " << stcCfg.objVmAnalogChannelInfo[sizIter].stcChannelInfo.strName << std::endl;
            std::cout << "\t\tPhase: " << stcCfg.objVmAnalogChannelInfo[sizIter].stcChannelInfo.chrPhase << std::endl;
            std::cout << "\t\tCircuit ID: " << stcCfg.objVmAnalogChannelInfo[sizIter].stcChannelInfo.strCircuitId << std::endl;
            std::cout << "\t\tUnit: " << stcCfg.objVmAnalogChannelInfo[sizIter].strUnit << std::endl;
            std::cout << "\t\tConversion Factor A: " << std::scientific << std::setprecision(10) << stcCfg.objVmAnalogChannelInfo[sizIter].f64ConvA << std::endl;
            std::cout << "\t\tConversion Factor B: " << std::scientific << std::setprecision(10) << stcCfg.objVmAnalogChannelInfo[sizIter].f64ConvB << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Mains Frequency (Hz): " << std::fixed << std::setprecision(6) << stcCfg.f32Frequency << std::endl;
        std::cout << "Sampling Rate Count: " << stcCfg.u32NumSamplingRates << std::endl;

        size_t const sizNumSamplingRates = static_cast<size_t>(stcCfg.u32NumSamplingRates);
        for (size_t sizIter = 0; sizNumSamplingRates > sizIter; ++sizIter) {
            std::cout << "\tIndex: " << (1 + sizIter) << std::endl;
            std::cout << "\t\tSampling Rate (Hz): " << std::fixed << std::setprecision(10) << stcCfg.vctSamplingRateInfo[sizIter].f64SamplesPerSec << std::endl;
            std::cout << "\t\tLast Sample: " << stcCfg.vctSamplingRateInfo[sizIter].u64LastSampleNumber << std::endl;
            std::cout << std::endl;
        }

        uint32_t const u32StartYear = static_cast<uint32_t>(stcCfg.stcDateTimeStart.stcDate.u16Year);
        uint32_t const u32StartMonth = static_cast<uint32_t>(stcCfg.stcDateTimeStart.stcDate.u8Month);
        uint32_t const u32StartDay = static_cast<uint32_t>(stcCfg.stcDateTimeStart.stcDate.u8Day);
        uint32_t const u32StartHour = static_cast<uint32_t>(stcCfg.stcDateTimeStart.stcTime.u8Hour);
        uint32_t const u32StartMinute = static_cast<uint32_t>(stcCfg.stcDateTimeStart.stcTime.u8Minute);
        float64_t const f64StartSecond = stcCfg.stcDateTimeStart.stcTime.f64Second;
        std::cout << "Start Date: " << std::setw(4) << std::setfill('0') << u32StartYear << "-"
            << std::setw(2) << std::setfill('0') << u32StartMonth << "-"
            << std::setw(2) << std::setfill('0') << u32StartDay
            << std::endl;
        std::cout << "Start Time: " << std::setw(2) << std::setfill('0') << u32StartHour << ":"
            << std::setw(2) << std::setfill('0') << u32StartMinute << ":"
            << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << f64StartSecond
            << std::endl;
        std::cout << std::endl;

        uint32_t const u32TriggerYear = static_cast<uint32_t>(stcCfg.stcDateTimeTrigger.stcDate.u16Year);
        uint32_t const u32TriggerMonth = static_cast<uint32_t>(stcCfg.stcDateTimeTrigger.stcDate.u8Month);
        uint32_t const u32TriggerDay = static_cast<uint32_t>(stcCfg.stcDateTimeTrigger.stcDate.u8Day);
        uint32_t const u32TriggerHour = static_cast<uint32_t>(stcCfg.stcDateTimeTrigger.stcTime.u8Hour);
        uint32_t const u32TriggerMinute = static_cast<uint32_t>(stcCfg.stcDateTimeTrigger.stcTime.u8Minute);
        float64_t const f64TriggerSecond = stcCfg.stcDateTimeTrigger.stcTime.f64Second;
        std::cout << "Trigger Date: " << std::setw(4) << std::setfill('0') << u32TriggerYear << "-"
            << std::setw(2) << std::setfill('0') << u32TriggerMonth << "-"
            << std::setw(2) << std::setfill('0') << u32TriggerDay
            << std::endl;
        std::cout << "Trigger Time: " << std::setw(2) << std::setfill('0') << u32TriggerHour << ":"
            << std::setw(2) << std::setfill('0') << u32TriggerMinute << ":"
            << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << f64TriggerSecond
            << std::endl;
        std::cout << std::endl;

        std::cout << "Data file format: ";
        if (enmDataFileFormatAscii == stcCfg.enmDataFileFormat) {
            std::cout << "ASCII" << std::endl;
        }
        else if (enmDataFileFormatBinary == stcCfg.enmDataFileFormat) {
            std::cout << "Binary" << std::endl;
        }
        else {
            return error::enmErrorInvalidArg;
        }

        std::cout << "Time Base (sec): " << std::fixed << std::setprecision(6) << stcCfg.f64TimeMult << std::endl;

        std::cout << std::endl;

        return error::enmErrorNone;
    }


    static error::enmErrorType
        parseAsciiDataFile(
            stcConfigFileType const& stcCfgIn,
            stcDataFileType& stcDatOut,
            std::ifstream& objIfsDat
        ) {
        // TODO
        return error::enmErrorNotImpl;
    }

    static error::enmErrorType
        parseBinaryDataFile(
            stcConfigFileType const& stcCfgIn,
            stcDataFileType& stcDatOut,
            std::ifstream& objIfsDat
        ) {
        /* 6.5 Binary data files */
        //
        // Notes
        //     - marshalling (little endian)
        //         - "the standard DOS format is to store the least significant byte (LSB) of the
        //           data first"
        //     - sample size
        //         - "the number of bytes required for each sample in the file will be:
        //             - (Ak * 2) + (2 * INT(Dm/16)) + 4 + 4"
        //         - "INT(Dm/16) is the number of status channels divided by 16 and rounded up to
        //           the next integer"
        // 
        // Sequence
        //     - sample number (uint32_t)
        //         - "unsigned binary form of four bytes"
        //     - timestamp (uint32_t)
        //         - "unsigned binary form of four bytes"
        //         - "hexadecimal 8000 is reserved to mark missing data"
        //     - analog channel sample data (int16_t)
        //         - "two's complement binary format of two bytes each"
        //     - status [digital] channel sample data (uint16_t, bitfield)
        //         - "stored in groups of two bytes for each 16 status channels, with the least
        //           significant bit of a word assigned to the smallest input channel number
        //           belonging to that group of 16 channels"

        if (stcDatOut.bSimpleSampling) {
            /* Calculate sample size */
            stcDatOut.u32SampleSizeBytes = static_cast<uint32_t>(
                // sample number
                (4)
                // timestamp
                + (4)
                // analog channels
                + (2 * stcCfgIn.u32NumAnaChannels)
                // digital channels
                + (2 * static_cast<uint32_t>(ceil(stcCfgIn.u32NumDigChannels / 16)))
                );

            /* Read and parse each sample */
            stcDatOut.u32PrevSampleNumber = 0;
            uint64_t const u64TotalSamp = stcDatOut.u64TotalSamples;
            char* const ptrChrBuf = new char[stcDatOut.u32SampleSizeBytes];

            uint64_t u64SplitCount = 0;

            for (size_t sizIter = 0; u64TotalSamp > sizIter; ++sizIter) {
                if (sizIter > (u64SplitCount * (u64TotalSamp / 20))) {
                    std::cout << ".";
                    ++u64SplitCount;
                }

                // Read sample
                objIfsDat.read(ptrChrBuf, stcDatOut.u32SampleSizeBytes);
                char const* ptrChrAt = ptrChrBuf;
                stcSampleDataType stcSampleData{};

                // Parse sample count
                stcSampleData.u32SampleNumber = utils::popU32Le(ptrChrAt);

                // Validate sample count
                if ((1 + stcDatOut.u32PrevSampleNumber) != stcSampleData.u32SampleNumber) {
                    return error::emErrorOutOfOrder;
                }
                ++stcDatOut.u32PrevSampleNumber;

                // Parse timestamp
                stcSampleData.f64TimestampUs = (stcCfgIn.f64TimeMult * utils::popU32Le(ptrChrAt));

                // Parse analog channel sample data
                uint32_t const u32NumAnaChannels = stcCfgIn.u32NumAnaChannels;
                for (size_t sizIterJ = 0; u32NumAnaChannels > sizIterJ; ++sizIterJ) {
                    stcAnalogDataType* const ptrStcAnaData = new stcAnalogDataType{};
                    ptrStcAnaData->i16DataRaw = utils::popI16Le(ptrChrAt);

                    stcAnalogChannelInfoType const stcAnaChanInfo = stcCfgIn.objVmAnalogChannelInfo[sizIterJ];
                    float64_t const f64DataTmp = (
                        (stcAnaChanInfo.f64ConvA * ptrStcAnaData->i16DataRaw)
                        + stcAnaChanInfo.f64ConvB
                        );

                    // Validate engineering unit
                    std::string const strUnit = stcAnaChanInfo.strUnit;
                    auto const objFindResult = std::find(
                        vctStrValidEngrUnits.begin(),
                        vctStrValidEngrUnits.end(),
                        strUnit.substr(strUnit.size() - 1)
                    );
                    if (vctStrValidEngrUnits.end() == objFindResult) {
                        // Invalid base unit
                        return error::enmErrorInvalidArg;
                    }
                    std::string const strPrefix = strUnit.substr(0, (strUnit.size() - 1));
                    if (0 == mapUnitToConversion.count(strPrefix)) {
                        // Invalid prefix
                        return error::enmErrorInvalidArg;
                    }

                    // Convert according to unit (e.g., A vs kA)
                    float64_t const f64EngUnitConv = mapUnitToConversion.at(strPrefix);
                    ptrStcAnaData->f64Data = (f64EngUnitConv * f64DataTmp);

                    stcChannelInfoType const stcChannelInfo = stcAnaChanInfo.stcChannelInfo;
                    std::string const strName = stcChannelInfo.strName;
                    stcSampleData.objVmSampleAnaData.insert(strName, ptrStcAnaData);

                    // Store analog data by channel
                    if (0 == stcDatOut.objVmChanAnaData.count(strName)) {
                        // Insert new channel
                        stcDatOut.objVmChanAnaData.insert(strName, new std::vector<stcAnalogDataType*>{});
                    }
                    stcDatOut.objVmChanAnaData[strName]->push_back(ptrStcAnaData);
                }

                // Store analog data by sample
                stcDatOut.vctSampleData.push_back(stcSampleData);

                // Parse digital channel sample data
                // TODO

                // Store digital data by sample
                // TODO
            }
            delete[] ptrChrBuf;
        }

        return error::enmErrorNone;
    }

    error::enmErrorType
        parseDataFile(
            stcConfigFileType const& stcCfgIn,
            stcDataFileType& stcDatOut
        ) {
        if (!stcCfgIn.bInit) {
            return error::enmErrorInvalidArg;
        }

        std::cout << "? ? ? INFO ? ? ? Parsing data file";

        /* Un-initialize configuration data */
        stcDatOut.bInit = false;

        /* Open data file */
        std::ifstream objIfsDat;
        error::enmErrorType enmErrOpen = utils::openFile(
            stcCfgIn.strDatFileName,
            (std::ifstream::binary | std::ifstream::in),
            objIfsDat
        );
        if (error::enmErrorNone != enmErrOpen) {
            objIfsDat.close();
            return enmErrOpen;
        }

        /* Variables for parsing */
        error::enmErrorType enmErrRet = error::enmErrorNone;
        stcDatOut.bSimpleSampling = false;
        stcDatOut.u64TotalSamples = 0;
        if (1 == stcCfgIn.vctSamplingRateInfo.size()) {
            // don't have to worry about reading different sampling rates and coordinating
            stcDatOut.bSimpleSampling = true;
            // total sample quantity matches last sample number
            stcDatOut.u64TotalSamples = stcCfgIn.vctSamplingRateInfo[0].u64LastSampleNumber;
        }
        if (!stcDatOut.bSimpleSampling) {
            std::cerr << "? ? ? INFO ? ? ? Multiple sampling rates not supported." << std::endl;
            enmErrRet = error::enmErrorNotImpl;
            goto cleanup;
        }

        switch (stcCfgIn.enmDataFileFormat) {
        case comtrade::enmDataFileFormatAscii: {
            enmErrRet = parseAsciiDataFile(stcCfgIn, stcDatOut, objIfsDat);
            break;
        }
        case comtrade::enmDataFileFormatBinary: {
            enmErrRet = parseBinaryDataFile(stcCfgIn, stcDatOut, objIfsDat);
            break;
        }
        default: {
            enmErrRet = error::enmErrorInvalidArg;
            goto cleanup;
            break;
        }
        }

        /* Mark initialized */
        stcDatOut.bInit = true;

    cleanup:
        std::cout << " Done." << std::endl;
        objIfsDat.close();
        return enmErrRet;
    }

    error::enmErrorType
        printDataInfo(
            stcConfigFileType const& stcCfg,
            stcDataFileType const& stcDat,
            uint64_t const u64SampleNumber
        ) {
        if (!stcCfg.bInit) {
            return error::enmErrorInvalidArg;
        }
        if (!stcDat.bInit) {
            return error::enmErrorInvalidArg;
        }
        if (
            (0 == u64SampleNumber)
            || (u64SampleNumber > stcDat.u64TotalSamples)
            ) {
            return error::enmErrorInvalidArg;
        }

        std::cout << std::endl;

        size_t const sizSampleIdx = static_cast<size_t>(u64SampleNumber - 1);
        std::cout << "Sample:\t\t" << stcDat.vctSampleData[sizSampleIdx].u32SampleNumber << std::endl;
        std::cout << "Time (us):\t" << stcDat.vctSampleData[sizSampleIdx].f64TimestampUs << std::endl;

        // Print analog samples
        auto const objVmSampleAnaData = stcDat.vctSampleData[sizSampleIdx].objVmSampleAnaData;
        for (size_t sizIter = 0; objVmSampleAnaData.size() > sizIter; ++sizIter) {
            stcAnalogChannelInfoType const stcAnaChanInfo = stcCfg.objVmAnalogChannelInfo[sizIter];
            stcChannelInfoType const stcChanInfo = stcAnaChanInfo.stcChannelInfo;
            std::string const strName = stcChanInfo.strName;
            std::cout << "Channel " << (1 + sizIter) << ":\t" << objVmSampleAnaData[sizIter]->f64Data
                << "\t" << strName << std::endl;
        }

        // Print digital samples
        // TODO

        std::cout << std::endl;

        return error::enmErrorNone;
    }

    error::enmErrorType
        printDataInfo(
            stcConfigFileType const& stcCfg,
            stcDataFileType const& stcDat,
            std::string const& strChanName
        ) {
        if (!stcCfg.bInit) {
            return error::enmErrorInvalidArg;
        }
        if (!stcDat.bInit) {
            return error::enmErrorInvalidArg;
        }
        if (
            (0 == stcDat.objVmChanAnaData.count(strChanName))
            && (0 == stcDat.objVmChanDigData.count(strChanName))
            ) {
            return error::enmErrorInvalidArg;
        }

        std::cout << std::endl;

        std::cout << "Channel:\t" << strChanName << std::endl;

        if (0 != stcDat.objVmChanAnaData.count(strChanName)) {
            // Print analog channel
            std::vector<stcAnalogDataType*> const vctAnaData = *stcDat.objVmChanAnaData[strChanName];
            uint64_t u64SampleIter = 0;
            for (stcAnalogDataType const* const ptrStcAnaData : vctAnaData) {
                ++u64SampleIter;
                std::cout << "Sample " << (u64SampleIter) << ":\t" << ptrStcAnaData->f64Data << std::endl;
                if (100 <= u64SampleIter) {
                    break;
                }
            }
        }
        else if (0 != stcDat.objVmChanDigData.count(strChanName)) {
            // Print digital channel
            // TODO
            return error::enmErrorNotImpl;
        }
        else {
            return error::enmErrorInvalidArg;
        }

        std::cout << std::endl;

        return error::enmErrorNone;
    }

}

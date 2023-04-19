/**
 * @file types.h
 * @brief Includes and definitions for fixed-width types.
 *
 * As a heuristic, all variable definitions should adhere to the following rules:
 *
 *     1) When using integer types, only specific-width values (`uint8_t`, `int32_t`, `float64_t`,
 *        etc.) or the type `size_t` type may be used, except where infeasible (e.g., `char`
 *        buffer when reading from istream.
 *
 *     2) Variables must be delcared constant whenever possible.
 *
 *     3) All integer variables must be prefixed according to the following (`Var` is whatever):
 *         3.1) uint8_t const u8Var;
 *         3.2) uint16_t const u16Var;
 *         3.3) uint32_t const u32Var;
 *         3.4) int8_t const i8Var;
 *         3.5) size_t const sizVar;
 *         3.6) etc
 *
 *     4) All floating point variables must be prefixed according to the following (`Var` is
 *        whatever):
 *         4.1) float32_t const f32Var;
 *         4.2) float64_t const f64Var;
 *
 *     5) All vector variables must be prefixed according to the following (`Var` is whatever):
 *         5.1) std::vector<uint8_t> const vctVar;
 *
 *     6) All struct definitions and variables must be prefixed according to the following (`Var`
 *        is whatever) (`Type` is necessary as-is):
 *         6.1) struct stcVarType { ... };
 *         6.2) stcVarType const stcVar{};
 *
 *     7) All enumeration definitions and variables must be prefixed according to the following
 *        (`Var` is whatever) (`Type` is necessary as-is):
 *         7.1) enum enmVarType { ... };
 *         7.2) enmVarType const enmVar;
 *
 *     8) All pointer types must be prefixed according to the following, and the type to which
 *        it points should be abbreviated as part of the variable name (`Var` is whatever):
 *         8.1) struct stcVarType { ... };
 *         8.2) stcVarType * const ptrStcVar = new stcVarType{};
 *
 *     8) All class definitions and instances must be prefixed according to the following (note
 *        the difference between `cls` and `obj`) (`Var` is whatever):
 *         8.1) class clsVar { };
 *         8.2) clsVarType const objVar();
 *
 *     9) When possible, an instance of a class should have the class name as an abbreviation in
 *        its name (`Var` is whatever):
 *         9.1) class clsFooBar { ... };
 *         9.2) clsFooBar const objFbVar; // `Fb` is abbreviation of `FooBar`
 *
 *    10) If the type of a variable is unknown (such as when using types from `std`), it is
 *        acceptable to simply prefix the variable name with `obj`:
 *        10.1) std::io_base::openmode objMode;
 *
 * @author Adam King
 * @date 2023-04-19
 */

#pragma once

#include <cstdint>

typedef float float32_t;
typedef double float64_t;

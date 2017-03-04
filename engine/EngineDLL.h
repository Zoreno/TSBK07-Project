/**
 * @file	EngineDLL.h
 * @Author	Joakim Bertils
 * @date	2017-03-04
 * @brief	Defines macros for exporting and importing items to/from dll. 
 */

#pragma once

#ifdef _ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

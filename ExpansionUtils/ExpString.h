#pragma once

#ifndef _EXP_STRING_H__
#define _EXP_STRING_H__

#ifdef EXPANSIONUTILS_EXPORTS
#define EXPANSIONUTILS_API __declspec(dllexport)
#else
#define EXPANSIONUTILS_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>

std::string WordUpcase(std::string toUpCase);

#endif //_EXP_STRING_H__
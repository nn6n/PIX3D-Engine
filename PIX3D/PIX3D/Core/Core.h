
#pragma once
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#ifdef PIX_DEBUG

#define PIX_ASSERT(cond) if(cond){} else { std::cout << "Assertion Failed!\n"; __debugbreak(); }
#define PIX_ASSERT_MSG(cond, msg) if(cond){} else { std::cout << msg << "\n"; __debugbreak(); }

#elif PIX_RELEASE

#define PIX_ASSERT

#endif



#else
#error This Engine Is Currently Supports Windows Platform Only!
#endif

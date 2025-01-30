#pragma once
#ifdef DLUAU_TIME_EXPORT
#define DLUAU_TIME_API extern "C" __declspec(dllexport)
#else
#define DLUAU_TIME_API extern "C" __declspec(dllimport)
#endif

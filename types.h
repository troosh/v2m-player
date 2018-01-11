#ifndef TYPES_H_
#define TYPES_H_

#ifdef _DEBUG
extern void printf2(const char *format, ...);
#else
#define printf2(...)
#endif

#endif

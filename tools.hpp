//     -*- mode:c++; tab-width:4 -*-
// File: tools.hpp ----------------------------------------------------
// Header file for the C++ tools library.
// Authors:  Alice E. Fischer and Michael J. Fischer.
// Modified September 2018.

#pragma once

// Use -DDEBUG on the g++ command line to build the debug version
#ifdef DEBUG
// Macro to print and then run a line of source code
#define v(a)                                                                   \
    cout << "\n[" #a "]" << endl;                                              \
    a

// Macro to print a variable
#define p(a)                                                                   \
    cout << #a " = " << a << endl;                                             \
    a

// Macro to print a key and value
#define kv(a, b) cout << "[" #a << "\t= " << b << " ]" << endl;

// Macro to print destructors
#define d(a) cout << "\n Deleting " << #a << " " << this << dec << "...";

// Macro to print a debug separator
#define separator()                                                            \
    cout << "\n";                                                              \
    cout << "---------------------------------------------------------------"; \
    cout << "\n";

#else
#define DEBUG 0
// alternative way to define them:
#define v(a) a

#define p(a) a

#define kv(a, b)

#define d(a)

#define separator()

#endif

// -------------------------------------------------------------------
// Local definitions.
// Please enter your own name.
// -------------------------------------------------------------------
#define NAME "Ferhat Erata"
#define CLASS "CPSC 454/554"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#include <cctype> // for isspace() and isdigit()
#include <cerrno>
#include <cmath>
#include <cstdarg> // for functions with variable # of arguments
#include <cstdio>  // for C compatability
#include <cstdlib> // for malloc() and calloc()
#include <cstring> // for time_t, time() and ctime()
#include <ctime>
#include <unistd.h>

namespace color {
inline const auto red = "\033[0;31m";
inline const auto green = "\033[0;32m";
inline const auto yellow = "\033[0;33m";
inline const auto blue = "\033[0;34m";
inline const auto magenta = "\033[0;35m";
inline const auto cyan = "\033[0;36m";
inline const auto bold = "\033[1m";
inline const auto reset_bold = "\033[21m";
inline const auto underline = "\033[4m";
inline const auto reset_underline = "\033[24m";
inline const auto reset = "\033[0m";
} // namespace color

using namespace std;

// -------------------------------------------------------------------
// Macros to make more convenient use of standard library functions.
// -------------------------------------------------------------------
#define DUMPp(p)                                                               \
    "\n" << hex << "   " #p " @ " << (unsigned)&p                              \
         << "   value = " << (unsigned)p << "   " #p " --> " << dec << *p
#define DUMPv(k)                                                               \
    "\n" << hex << "   " #k " @ " << (unsigned)&k << "   value = " << dec << k

// -------------------------------------------------------------------
// Routine screen and process management.-----------------------------
// -------------------------------------------------------------------
void fbanner(ostream& fout);
#define banner() fbanner(cout)
void fbye();
#define bye() fbye()
void delay(int);

// -------------------------------------------------------------------
// I/O Extension. ----------------------------------------------------
// -------------------------------------------------------------------
istream& flush(istream& is); // Used in cin >>x >>flush;

// -------------------------------------------------------------------
// Fatal error handling. ---------------------------------------------------
// -------------------------------------------------------------------
// The fatal family of tools are to provide a quick way
// to terminate a program with a descriptive message.
// While these tools should not be used in production code,
// they can be quite useful during program development
// and testing.

// The fatal() function (lower-case "f").
// This is called like C's printf() function to produce a
// formatted error comment.
void fatal(const char* format, ...);

// The Fatal() class for use with throw.
// This class is for fatal error exceptions.
//    The constructor takes a format argument followed by any number
//    of data arguments.
//    It formats and stores an error message of at most 255 bytes.
//    The what() member function returns the stored string.
class Fatal : public std::exception {
  private:
    char msg[256];

  public:
    explicit Fatal(const char* format, ...);
    [[nodiscard]] const char* what() const noexcept override { return msg; }
};

// -------------------------------------------------------------------
// Time and date. ----------------------------------------------------
// -------------------------------------------------------------------
void when(char* date, char* hour);
char* today(char* date);
char* oclock(char* hour);

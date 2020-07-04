//
// Created by Vladimir Shubarin on 10.05.2020.
//

#ifndef SPO_COMPILER__STACKTRACEGENERATOR_HPP_
#define SPO_COMPILER__STACKTRACEGENERATOR_HPP_
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>

/**
 * @brief Генерирует stack trace но ТОЛЬКО на Mac OS
 */
class StackTraceGenerator {
 private:

  //  this is a pure utils class
  //  cannot be instantiated
  //
  StackTraceGenerator() = delete;
  StackTraceGenerator(const StackTraceGenerator &) = delete;
  StackTraceGenerator &operator=(const StackTraceGenerator &) = delete;
  ~StackTraceGenerator() = delete;

 public:

  static std::vector<std::string> GetTrace() {
	//  record stack trace upto 128 frames
	int callstack[128] = {};

	// collect stack frames
	int frames = backtrace((void **)callstack, 128);

	// get the human-readable symbols (mangled)
	char **strs = backtrace_symbols((void **)callstack, frames);

	std::vector<std::string> stackFrames;
	stackFrames.reserve(frames);

	for (int i = 0; i < frames; ++i) {
	  char functionSymbol[1024] = {};
	  char moduleName[1024] = {};
	  int offset = 0;
	  char addr[48] = {};

	  /*

	   Typically this is how the backtrace looks like:

	   0   <app/lib-name>     0x0000000100000e98 _Z5tracev + 72
	   1   <app/lib-name>     0x00000001000015c1 _ZNK7functorclEv + 17
	   2   <app/lib-name>     0x0000000100000f71 _Z3fn0v + 17
	   3   <app/lib-name>     0x0000000100000f89 _Z3fn1v + 9
	   4   <app/lib-name>     0x0000000100000f99 _Z3fn2v + 9
	   5   <app/lib-name>     0x0000000100000fa9 _Z3fn3v + 9
	   6   <app/lib-name>     0x0000000100000fb9 _Z3fn4v + 9
	   7   <app/lib-name>     0x0000000100000fc9 _Z3fn5v + 9
	   8   <app/lib-name>     0x0000000100000fd9 _Z3fn6v + 9
	   9   <app/lib-name>     0x0000000100001018 main + 56
	   10  libdyld.dylib      0x00007fff91b647e1 start + 0

	   */

	  // split the string, take out chunks out of stack trace
	  // we are primarily interested in module, function and address
	  sscanf(strs[i], "%*s %s %s %s %*s %d", &moduleName, &addr, &functionSymbol, &offset);

	  int validCppName = 0;
	  //  if this is a C++ library, symbol will be demangled
	  //  on success function returns 0
	  //
	  char *functionName = abi::__cxa_demangle(functionSymbol, nullptr, nullptr, &validCppName);

	  char stackFrame[4096] = {};
	  if (validCppName == 0) // success
	  {
		sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
				moduleName, addr, functionName, offset);
	  } else {
		//  in the above traceback (in comments) last entry is not
		//  from C++ binary, last frame, libdyld.dylib, is printed
		//  from here
		sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
				moduleName, addr, functionName, offset);
	  }

	  if (functionName) {
		free(functionName);
	  }

	  std::string frameStr(stackFrame);
	  stackFrames.push_back(frameStr);
	}
	free(strs);

	return stackFrames;
  }
  struct StackTracePrinter {
	void operator()() const {
	  auto frames = StackTraceGenerator::GetTrace();
	  std::for_each(frames.cbegin(), frames.cend(),
					[](const std::string &frame) { std::cout << frame << std::endl; });
	}
  };

  static void printStack() {
	StackTracePrinter stPrinter;
	stPrinter();
  }
};

#endif //SPO_COMPILER__STACKTRACEGENERATOR_HPP_

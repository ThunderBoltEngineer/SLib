/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_SYSTEM
#define CHECKHEADER_SLIB_CORE_SYSTEM

#include "definition.h"

#include "string.h"

namespace slib
{
	
	typedef void (*SIGNAL_HANDLER)(int signal);
	
	class SLIB_EXPORT System
	{
	public:
		// System & Application Information
		static String getApplicationPath();

		static String getApplicationDirectory();

		static String getApplicationHomeDirectory();

		static String getCachesDirectory();

		static String getTempDirectory();

		static String getCurrentDirectory();

		static sl_bool setCurrentDirectory(const String& dir);
		
		
		static String getUserName();
		
		static String getFullUserName();


		// Tick count (milliseconds)
		static sl_uint32 getTickCount();
		
		static sl_uint64 getTickCount64();
	

		// Process & Thread
		static sl_uint32 getProcessId();

		static sl_uint32 getThreadId();

		static sl_bool createProcess(const String& pathExecutable, const String* command, sl_uint32 nCommands);

		static void exec(const String& pathExecutable, const String* command, sl_uint32 nCommands);

		static void exit(int code);

		static void sleep(sl_uint32 millis);

		static void yield();

		static void yield(sl_uint32 elapsed);
	
		
		// Error Handling
		static sl_uint32 getLastError();
		
		static String formatErrorCode(sl_uint32 errorCode);
		
		static void abort(const String& msg, const String& file, sl_uint32 line);

		static void setCrashHandler(SIGNAL_HANDLER handler);
		
		static void setDebugFlags();


		static void registerApplicationRunAtStartup(const String& path);

		static void registerApplicationRunAtStartup();

		static void unregisterApplicationRunAtStartup(const String& path);

		static void unregisterApplicationRunAtStartup();

	};
	
	class SLIB_EXPORT GlobalUniqueInstance : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		GlobalUniqueInstance();

		~GlobalUniqueInstance();

	public:
		static Ref<GlobalUniqueInstance> create(const String& name);

		static sl_bool exists(const String& name);

	};

}

#endif


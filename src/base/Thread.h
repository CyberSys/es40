/* ES40 emulator.
 * Copyright (C) 2007-2008 by the ES40 Emulator Project
 *
 * WWW    : http://www.es40.org
 * E-mail : camiel@es40.org
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * Although this is not required, the author would appreciate being notified of, 
 * and receiving any modifications you may make to the source code that might serve
 * the general public.
 *
 * Parts of this file based upon the Poco C++ Libraries, which is Copyright (C) 
 * 2004-2006, Applied Informatics Software Engineering GmbH. and Contributors.
 */

/**
 * $Id: Thread.h,v 1.1 2008/05/31 15:47:28 iamcamiel Exp $
 *
 * X-1.1        Camiel Vanderhoeven                             31-MAY-2008
 *      Initial version for ES40 emulator.
 **/

//
// Thread.h
//
// $Id: Thread.h,v 1.1 2008/05/31 15:47:28 iamcamiel Exp $
//
// Library: Foundation
// Package: Threading
// Module:  Thread
//
// Definition of the Thread class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_Thread_INCLUDED
#define Foundation_Thread_INCLUDED


#include "Foundation.h"
#include "Mutex.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Thread_WIN32.h"
#else
#include "Thread_POSIX.h"
#endif

class CRunnable;
class CThreadLocalStorage;


class CThread: private CThreadImpl
	/// This class implements a platform-independent
	/// wrapper to an operating system thread.
	///
	/// Every Thread object gets a unique (within
	/// its process) numeric thread ID.
	/// Furthermore, a thread can be assigned a name.
	/// The name of a thread can be changed at any time.
{
public:	
	enum Priority
		/// Thread priorities.
	{
		PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
		PRIO_LOW     = PRIO_LOW_IMPL,    /// A lower than normal thread priority.
		PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
		PRIO_HIGH    = PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
		PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
	};

	CThread();
		/// Creates a thread. Call start() to start it.
		
	CThread(const std::string& name);
		/// Creates a named thread. Call start() to start it.
		
	~CThread();
		/// Destroys the thread.

	int id() const;
		/// Returns the unique thread ID of the thread.

	std::string name() const;
		/// Returns the name of the thread.

	std::string getName() const;
		/// Returns teh name of the thread.

	void setName(const std::string& name);
		/// Sets the name of the thread.

	void setPriority(Priority prio);
		/// Sets the thread's priority.
		///
		/// Some platform only allow changing a thread's priority
		/// if the process has certain privileges.

	Priority getPriority() const;
		/// Returns the thread's priority.

	void start(CRunnable& target);
		/// Starts the thread with the given target.

	void join();
		/// Waits until the thread completes execution.	
		/// If multiple threads try to join the same
		/// thread, the result is undefined.
		
	void join(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Throws a TimeoutException if the thread
		/// does not complete within the specified time interval.
		
	bool tryJoin(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Returns true if the thread has finished,
		/// false otherwise.

	bool isRunning() const;
		/// Returns true if the thread is running.

	static void sleep(long milliseconds);
		/// Suspends the current thread for the specified
		/// amount of time.

	static void yield();
		/// Yields cpu to other threads.

	static CThread* current();
		/// Returns the Thread object for the currently active thread.
		/// If the current thread is the main thread, 0 is returned.

protected:
	CThreadLocalStorage& tls();
		/// Returns a reference to the thread's local storage.

	void clearTLS();
		/// Clears the thread's local storage.

	std::string makeName();
		/// Creates a unique name for a thread.
		
	static int uniqueId();
		/// Creates and returns a unique id for a thread.
		
private:
	CThread(const CThread&);
	CThread& operator = (const CThread&);

	int                 _id;
	std::string         _name;
	CThreadLocalStorage* _pTLS;
	mutable CFastMutex   _mutex;

	friend class CThreadLocalStorage;
//	friend class CPooledThread;
};


//
// inlines
//
inline int CThread::id() const
{
	return _id;
}


inline std::string CThread::name() const
{
	CFastMutex::CScopedLock lock(&_mutex);
	
	return _name;
}


inline std::string CThread::getName() const
{
	CFastMutex::CScopedLock lock(&_mutex);
	
	return _name;
}


inline bool CThread::isRunning() const
{
	return isRunningImpl();
}


inline void CThread::sleep(long milliseconds)
{
	sleepImpl(milliseconds);
}


inline void CThread::yield()
{
	yieldImpl();
}


inline CThread* CThread::current()
{
	return static_cast<CThread*>(currentImpl());
}

#endif // Foundation_Thread_INCLUDED

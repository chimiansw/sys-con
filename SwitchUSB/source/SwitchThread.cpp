#include "SwitchThread.h"
#include "switch.h"

void SwitchThread::ThreadLoop(void *argument)
{
    svcSleepThread(1e+7L);
    while (static_cast<SwitchThread *>(argument)->m_isRunning)
    {
        static_cast<SwitchThread *>(argument)->m_function(static_cast<SwitchThread *>(argument)->m_argument);
    }
}

SwitchThread::SwitchThread(ThreadFunc function, void *argument, size_t stackSize, int prio)
    : m_function(function), m_argument(argument)
{
    if (R_SUCCEEDED(threadCreate(&m_thread, &SwitchThread::ThreadLoop, this, NULL, stackSize, prio, -2)))
    {
        Start();
    }
}

SwitchThread::~SwitchThread()
{
    Close();
}

Result SwitchThread::Start()
{
    if (!m_isRunning)
    {
        m_isRunning = true;
        return threadStart(&m_thread);
    }
    else
        return 0;
}

Result SwitchThread::Close()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        svcCancelSynchronization(m_thread.handle);
        threadWaitForExit(&m_thread);
        return threadClose(&m_thread);
    }
    else
        return 0;
}
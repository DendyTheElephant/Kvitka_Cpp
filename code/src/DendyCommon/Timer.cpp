#include <DendyCommon/Timer.h>

void DendyCommon::CTimer::_UpdateElapsedTicks()
{
    if ( m_IsHighPerformanceClockIsSteady )
        m_ElapsedTicks += std::chrono::high_resolution_clock::now() - m_StartTicksHiRes;
    else
        m_ElapsedTicks += std::chrono::steady_clock::now() - m_StartTicks;
    _UpdateStartTicks();
}

void DendyCommon::CTimer::_UpdateStartTicks()
{
    if ( m_IsHighPerformanceClockIsSteady )
        m_StartTicksHiRes = std::chrono::high_resolution_clock::now();
    else
        m_StartTicks = std::chrono::steady_clock::now();
}

unsigned long int DendyCommon::CTimer::GetElapsedMicroseconds()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::microseconds>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::microseconds>( m_ElapsedTicks ).count( );
    }
}

unsigned long int DendyCommon::CTimer::GetElapsedNanoseconds()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::nanoseconds>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::nanoseconds>( m_ElapsedTicks ).count( );
    }
}

unsigned long int DendyCommon::CTimer::GetElapsedMiliseconds()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::milliseconds>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::milliseconds>( m_ElapsedTicks ).count( );
    }
}

unsigned long int DendyCommon::CTimer::GetElapsedSeconds()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::seconds>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::seconds>( m_ElapsedTicks ).count( );
    }
}


unsigned long int DendyCommon::CTimer::GetElapsedMinutes()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::minutes>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::minutes>( m_ElapsedTicks ).count( );
    }
}

unsigned long int DendyCommon::CTimer::GetElapsedHours()
{
    if ( m_IsPaused )
        return std::chrono::duration_cast<std::chrono::hours>( m_ElapsedTicks ).count( );
    else {
        _UpdateElapsedTicks( );
        return std::chrono::duration_cast<std::chrono::hours>( m_ElapsedTicks ).count( );
    }
}

void DendyCommon::CTimer::Reset()
{
    m_IsPaused = false;
    _UpdateStartTicks( );
    m_ElapsedTicks = std::chrono::duration<double>( 0 );
}

void DendyCommon::CTimer::Resume( ) {
    if ( m_IsPaused ) {
        _UpdateStartTicks( );
        m_IsPaused = false;
    }
}

void DendyCommon::CTimer::Pause()
{
    _UpdateElapsedTicks();
    m_IsPaused = true;
}
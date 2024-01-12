#pragma once

#include <chrono>

namespace DendyCommon
{

class CTimer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTicks; ///< Initial ticks
    std::chrono::duration<double> m_ElapsedTicks; ///< Cumuled ticks between starts and pauses
    bool m_IsPaused{ false }; ///< Status in pause
    bool m_IsHighPerformanceClockIsSteady{ false }; ///< Depending on the platform, use highest precision clock if it is consistent or basic consistent clock

private:
    /// Updates the elapsed ticks: adds to the elapsed ticksthe duration between now and previous ticks using the consistent clock. 
    /// And updates the start tick value.
    void _UpdateElapsedTicks();

    /// Sets start ticks to current tick value using the consistent clock.
    void _UpdateStartTicks();


public:
    CTimer( ): m_IsHighPerformanceClockIsSteady( std::chrono::high_resolution_clock::is_steady ) { Reset(); }

    /// Returns the elapsed time in microseconds since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedMicroseconds( );
    
    /// Returns the elapsed time in nanoseconds since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedNanoseconds( );
    
    /// Returns the elapsed time in milliseconds since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedMiliseconds( );
    
    /// Returns the elapsed time in seconds since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedSeconds( );
    
    /// Returns the elapsed time in minutes since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedMinutes( );
    
    /// Returns the elapsed time in hours since last Timer() OR Timer.reset().
    /// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
    /// If the timer is in pause: returns the elapsed time since it was paused!
    unsigned long int GetElapsedHours( );

    bool GetPausedState() const { return m_IsPaused; }

    /// Resets the timer (elapsed time -> near 0) and starts it if it was paused!
    void Reset();

    /// Resume the timer if it was paused!
    void Resume();

    /// Pauses the timer. This means that the elapsed time will not be updated: to unpause call Timer.resume() or Timer.reset()
    void Pause();
};

}
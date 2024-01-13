#pragma once

#include <string>

namespace DendyCommon
{

class CString
{
private:
    

private:
    /// Updates the elapsed ticks: adds to the elapsed ticksthe duration between now and previous ticks using the consistent clock. 
    /// And updates the start tick value.
    void _UpdateElapsedTicks();

    /// Sets start ticks to current tick value using the consistent clock.
    void _UpdateStartTicks();


public:
    CString() {}
    CString( CString const& string );
    CString( const char* string );
    CString( std::string string );

    void operator+=( CString const& string );
    void operator+=( const char* string );
    void operator+=( int_fast32_t integer );
    void operator+=( float a_float );
    void operator+=( bool a_boolean );

    CString operator+( CString const& a_string );
    CString operator+( const char* a_string );
    CString operator+( int_fast32_t a_integer );
    CString operator+( float a_float );
    CString operator+( bool a_boolean );

    bool operator==( CString const& a_string );
    bool operator==( const char* a_string );
    operator bool( ) const;

    static CString format( const char* a_format, ... );

    // /// Like the c_str() method, returns char string data (ending with '\0')
    // const char* asConstChar( ) const {
    //     return m_memory.data( );
    // }

    // /// Like the c_str() method, returns char string data (ending with '\0')
    // const char* asAllocatedConstChar( ) const {
    //     char* allocatedResult = new char[m_memory.length( ) + 1];
    //     strcpy_s( allocatedResult, m_memory.length( ) + 1, m_memory.c_str( ) ); // this puts a '\0' at the end ;)
    //     return allocatedResult;
    // }

    // /// Acces to the buffer pointer (WRITABLE)
    // char* asCharMutablePtr( ) {
    //     return const_cast<char*>( m_memory.data( ) );
    // }

    // std::string asStdString( ) const { 
    //     return m_memory;
    // }

    // uint_fast16_t len( ) const { 
    //     return static_cast<uint_fast16_t>( m_memory.length( ) );
    // }

    // bool find( dyString const& a_toFind ) const;
    // bool find( const char* a_toFind ) const;

    // void replace( dyString const& a_toFind, dyString const& a_newValue );
    // void replace( const char* a_toFind, const char* a_newValue );

    // /// \brief Eis phokoowin scheit gonna work as a python substring too [2:-1]
    // CString substring( int_fast16_t a_begin, int_fast16_t a_end ) const;

    // CString split( CString const& a_separator ) {
    //     /***/
    // }


};

}
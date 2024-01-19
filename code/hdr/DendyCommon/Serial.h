#pragma once

#include <string>
#include <iostream>

namespace DendyCommon
{

class CSerial
{
private:
    friend class std::hash<DendyCommon::CSerial>;

    inline static std::size_t m_SerialIncrement = 1;
    std::size_t m_SerialNumber{0};
    std::string m_Name;
    
public:
    // Forbid contructor by copy
    // CSerial(CSerial const&) = delete;
    // CSerial& operator=(CSerial const&) = delete;

    CSerial(CSerial const& original) {m_Name=original.m_Name; m_SerialNumber=original.m_SerialNumber;std::cout << "CopyCtor for CSerial" << *this << std::endl;}
    CSerial& operator=(CSerial const& original) {m_Name=original.m_Name; m_SerialNumber=original.m_SerialNumber;std::cout << "CopyCtor for CSerial" << *this << std::endl;return *this;}

    CSerial(): m_SerialNumber(m_SerialIncrement++) {}
    inline void SetName(std::string name) {m_Name = name;}
    inline std::string const& GetName() const {return m_Name;}
    //inline std::size_t GetSerialNumber() const {return m_SerialNumber;}

    // For Hashable functions
    bool operator==(CSerial const& other) const
    { 
        return (m_SerialNumber == other.m_SerialNumber);
    }

    friend std::ostream& operator<<(std::ostream& os, CSerial const& serial) 
    {
        os << std::string("{") << serial.m_Name << std::string("|") << std::to_string(serial.m_SerialNumber) << std::string("}");
        return os;
    }
};

}

// For Hashable functions
namespace std
{
template <>
struct hash<DendyCommon::CSerial>
{
    auto operator()(const DendyCommon::CSerial &serial) const -> size_t {return hash<size_t>()(serial.m_SerialNumber);}
};
}  // namespace std
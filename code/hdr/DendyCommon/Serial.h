#pragma once

#include <string>

namespace DendyCommon
{

class CSerial
{
private:
    inline static std::size_t m_SerialIncrement = 1;
    std::size_t m_SerialNumber{0};
    std::string m_Name;
    
public:
    CSerial(): m_SerialNumber(m_SerialIncrement++) {}
    inline void SetName(std::string name) {m_Name = name;}
    inline std::string const& GetName() const {return m_Name;}
    inline std::size_t GetSerialNumber() const {return m_SerialNumber;}

    // Hashable functions
    bool operator==(CSerial const& other) const
    { 
        return (m_SerialNumber == other.GetSerialNumber());
    }
};

struct CSerialHasher
{
    std::size_t operator()(CSerial const& serial) const
    {
        return serial.GetSerialNumber();
    }
};

// std::ostream& operator<<(std::ostream &out, CSerial const& data)
// {
//     out << data.GetName() << std::to_string(data.GetSerial());
//     return out;
// }


}
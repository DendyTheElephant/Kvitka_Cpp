#pragma once

#include <string>

namespace DendyCommon
{

class CSerial
{
private:
    inline static long long unsigned int m_SerialIncrement = 1;
    long long unsigned int m_Serial{0};
    std::string m_Name;
    
public:
    CSerial(): m_Serial(m_SerialIncrement++) {;}
    inline void SetName(std::string const& name) {m_Name=name;}
    inline std::string GetName() const {return m_Name;}
    inline long long unsigned int GetSerial() const {return m_Serial;}
};

// std::ostream& operator<<(std::ostream &out, CSerial const& data)
// {
//     out << data.GetName() << std::to_string(data.GetSerial());
//     return out;
// }


}
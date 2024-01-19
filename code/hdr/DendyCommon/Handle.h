#pragma once

#include <string>
#include <iostream>
#include <array>

namespace DendyCommon
{

class CHandle
{
private:
    friend class std::hash<DendyCommon::CHandle>;

    inline static std::size_t m_UIDIncrement = 1;
    std::size_t m_UID{0};
    std::string m_Name;
    
public:
    // Forbid contructor by copy
    // CHandler(CSerial const&) = delete;
    // CSerial& operator=(CSerial const&) = delete;

    CHandle(CHandle const& original) {m_Name=original.m_Name; m_UID=original.m_UID;std::cout << "CopyCtor for CHandler" << *this << std::endl;}
    CHandle& operator=(CHandle const& original) {m_Name=original.m_Name; m_UID=original.m_UID;std::cout << "CopyCtor for CHandler" << *this << std::endl;return *this;}

    CHandle(): m_UID(m_UIDIncrement++) {}
    inline void SetName(std::string name) {m_Name = name;}
    inline std::string const& GetName() const {return m_Name;}
    //inline std::size_t GetSerialNumber() const {return m_UID;}

    // For Hashable functions
    bool operator==(CHandle const& handle) const
    { 
        return (m_UID == handle.m_UID);
    }

    friend std::ostream& operator<<(std::ostream& os, CHandle const& handle) 
    {
        os << std::string("{") << handle.m_Name << std::string("|") << std::to_string(handle.m_UID) << std::string("}");
        return os;
    }
};

}

// For Hashable functions
namespace std
{
template <>
struct hash<DendyCommon::CHandle>
{
    auto operator()(const DendyCommon::CHandle &handle) const -> size_t {return hash<size_t>()(handle.m_UID);}
};
}  // namespace std
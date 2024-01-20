#pragma once

//#include <string>
//#include <iostream>
#include <vector>
#include <memory>

namespace DendyEngine::ECS
{

enum class EComponentType
{
    Position1,
    Position2
};

template<typename T, EComponentType TEnumType>
class CGameComponent
{
public:
    static constexpr EComponentType Type = TEnumType;//static_cast<std::size_t>(TEnumType);
};


struct STestStructure1 : CGameComponent<STestStructure1,EComponentType::Position1>
{

};

struct STestStructure2 : CGameComponent<STestStructure2,EComponentType::Position2>
{

};



class CECSEngine
{
private:

public:
    CECSEngine(){}

    template<EComponentType TComponentTypes>
    int _GetGameObjectsSetWithComponents()
    {
        std::cout << typeid(TComponentTypes).name() << std::endl;
        return 1;
    }

    template<EComponentType... TComponentTypes>
    void GetGameObjectsSetWithComponents()
    {
        
        std::vector<EComponentType> VecTypes;
        ((VecTypes.push_back(TComponentTypes)),...);
        std::cout << (_GetGameObjectsSetWithComponents<TComponentTypes>() , ...);

        // std::vector<std::string> VecTypes;
        // ((VecTypes.push_back(typeid(TComponentTypes).name())),...);
        // std::cout << (_GetGameObjectsSetWithComponents<TComponentTypes>() , ...);

        //return *static_cast<EntitySet<Ts...>*>(mEntitySets[EntitySet<Ts...>::Type].get());
        // for(const auto TComponentType : {TComponentTypes...})
        // {
        //     std::cout << TComponentType.type().name() << std::endl;
        // }
    }
};

}
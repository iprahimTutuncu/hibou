#pragma once
#include "data.h"

class ComponentDataHolder
{
    public:
        ComponentDataHolder()
        :mask(0){
        };

        template<typename T>
        void setComponent(DataType target, T& comp){
            Data* curr = components[static_cast<unsigned int>(target)];
            if (curr)
                delete curr;

            components[static_cast<unsigned int>(target)] = static_cast<Data*>(&comp);
            if(!hasComponent(target))
                mask += 1 << static_cast<unsigned int>(target);
        }

        template<typename T>
        void setComponent(DataType target, T&& comp){
            Data* curr = components[static_cast<unsigned int>(target)];
            if (curr)
                delete curr;

            T* temp = new T(std::move(comp));
            components[static_cast<unsigned int>(target)] = static_cast<Data*>(temp);
            if(!hasComponent(target))
                mask += 1 << static_cast<unsigned int>(target);
        }

        template<typename T>
        T* getComponent(DataType target){
            return static_cast<T*>(components[static_cast<unsigned int>(target)]);
        }

        bool hasComponent(DataType target){
            unsigned int value = 1 << static_cast<unsigned int>(target);

            return (value & mask) != 0 ? true : false;
        }

        ~ComponentDataHolder() {
            for(int i = 0; i < 20; i++)
                if(components[i])
                    delete components[i];
        }

    private:
        Data* components[20] = {nullptr};
        unsigned int mask{0};

};



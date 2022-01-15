#ifndef EMERALDDB_CLASS_REGISTER_H
#define EMERALDDB_CLASS_REGISTER_H

#include <memory>
#include <unordered_map>

// Pay Attention: static parameter will not be first initialized when linked as static-library in executable file
#define REGISTER_CLASS(DeriveName, Base, Derive, ...)                                   \
    namespace {                                                                         \
    static EMDB::ClassRegister<Base, Derive, ##__VA_ARGS__> Derive##Reg(DeriveName,     \
                                                                        ##__VA_ARGS__); \
    }

// Keep the target shared or object if you only want to use GET_REGISTER_CLASS function
#define GET_REGISTER_CLASS(Base, DeriveName) \
    (EMDB::ClassContainer<Base>::GetClass<Base>(DeriveName))

// GET_REGISTER_DERIVE_CLASS worked ok even in static library target
#define GET_REGISTER_DERIVE_CLASS(Base, Derive, DeriveName) \
    (EMDB::ClassContainer<Base>::GetClass<Derive>(DeriveName))

namespace EMDB {

template<typename Base>
class ClassContainer {
public:
    static void AddClass(const std::string& name, std::shared_ptr<Base> base)
    {
        GetContainer().insert({name, base});
    }

    template<typename Derive>
    static std::shared_ptr<Derive> GetClass(const std::string& name)
    {
        auto classPtr = GetContainer().find(name);
        if (classPtr == GetContainer().end()) {
            return nullptr;
        }

        return std::dynamic_pointer_cast<Derive>(classPtr->second);
    }
    static std::unordered_map<std::string, std::shared_ptr<Base>>& GetContainer()
    {
        static std::unordered_map<std::string, std::shared_ptr<Base>> _container;
        return _container;
    }
};

template<typename Base, typename Derive, typename... Args>
class ClassRegister {
public:
    explicit ClassRegister(const std::string& name, Args... args)
    {
        static_assert(std::is_base_of<Base, Derive>::value);
        auto ptr = std::shared_ptr<Base>(new Derive(std::forward<Args>(args)...));
        ClassContainer<Base>::AddClass(name, ptr);
    }
};
}  // namespace EMDB

#endif  // EMERALDDB_CLASS_REGISTER_H

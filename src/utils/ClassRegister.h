#ifndef EMERALDDB_CLASS_REGISTER_H
#define EMERALDDB_CLASS_REGISTER_H

#define REGISTER_CLASS(DeriveName, Base, Derive, ...)                                         \
    namespace {                                                                               \
    static ClassRegister<Base, Derive, ##__VA_ARGS__> Derive##Reg(DeriveName, ##__VA_ARGS__); \
    }

#define GET_REGISTER_CLASS(Base, DeriveName) (ClassContainer<Base>::GetClass<Base>(DeriveName))
#define GET_REGISTER_CLASS(Base, Derive, DeriveName) (ClassContainer<Base>::GetClass<Derive>(DeriveName))

template <typename Base>
class ClassContainer {
public:
    static void AddClass(const std::string &name, std::shared_ptr<Base> base)
    {
        GetContainer().insert({ name, base });
    }

    template <typename Derive>
    static std::shared_ptr<Derive> GetClass(const std::string &name)
    {
        auto classPtr = GetContainer().find(name);
        if (classPtr == GetContainer().end()) {
            return nullptr;
        }

        return std::dynamic_pointer_cast<Derive>(classPtr->second);
    }
    static std::unordered_map<std::string, std::shared_ptr<Base> > &GetContainer()
    {
        static std::unordered_map<std::string, std::shared_ptr<Base> > _container;
        return _container;
    }
};

template <typename Base, typename Derive, typename... Args>
class ClassRegister {
public:
    explicit ClassRegister(const std::string &name, Args... args)
    {
        static_assert(std::is_base_of<Base, Derive>::value);
        auto ptr = std::shared_ptr<Base>(new Derive(std::forward<Args>(args)...));
        ClassContainer<Base>::AddClass(name, ptr);
    }
};

#endif  // EMERALDDB_CLASS_REGISTER_H

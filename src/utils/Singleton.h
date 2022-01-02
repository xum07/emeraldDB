
namespace EMDB {

template<class T>
class Singleton {
public:
    static T& Instance()
    {
        static T instance;
        return instance;
    }

    Singleton(T&&) = delete;
    Singleton(const T&) = delete;
    void operator=(const T&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
};

}  // namespace EMDB
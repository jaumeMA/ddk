
namespace ddk
{

template<typename Hasher>
size_t hash(const Hasher& i_hasher,const std::string& i_value)
{
    return hash(i_value);
}

#if !defined(WIN32)

constexpr size_t hash(const int& i_value)
{
    return static_cast<size_t>(i_value);
}
constexpr size_t hash(const unsigned long& i_value)
{
    return static_cast<size_t>(i_value);
}
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const int& i_value)
{
    return static_cast<size_t>(i_value);
}
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const unsigned long& i_value)
{
    return static_cast<size_t>(i_value);
}

#endif

constexpr size_t hash(const time_t& i_value)
{
    return static_cast<size_t>(i_value);
}
template<typename UnderlyingType,typename T>
constexpr size_t hash(const Id<UnderlyingType,T>& i_id)
{
    return hash(i_id.getValue());
}
template<typename Hasher>
constexpr size_t hash(const Hasher& i_hasher,const time_t& i_value)
{
    return static_cast<size_t>(i_value);
}
template<typename Hasher,typename UnderlyingType,typename T>
constexpr size_t hash(const Hasher& i_hasher,const Id<UnderlyingType,T>& i_id)
{
    return hash(i_id.getValue());
}
template<typename Arg,typename ... Args>
constexpr size_t hash_combine(Arg&& i_arg,Args&& ... i_args)
{
    const size_t argsHash[] = { hash(std::forward<Args>(i_args)) ... };
    size_t res = hash(std::forward<Arg>(i_arg));

    for(size_t index = 0; index < mpl::num_types<Args...>; ++index)
    {
        res ^= argsHash[index] + 0x9e3779b9 + (res << 6) + (res >> 2);
    }

    return res;
}

constexpr builtin_hasher::builtin_hasher(size_t i_seed)
: m_hash(i_seed)
{
}
template<typename T>
constexpr builtin_hasher::builtin_hasher(T&& i_value)
: m_hash(hash(i_value))
{
}
template<typename ... Args>
size_t builtin_hasher::operator()(Args&& ... i_args) const
{
    m_hash = hash_combine(m_hash,std::forward<Args>(i_args)...);

    return m_hash;
}
constexpr size_t builtin_hasher::get() const
{
    return m_hash;
}

template<typename ... Args>
size_t commutative_builtin_hasher::operator()(Args&& ... i_args) const
{
    //by now we develop commutative hashes by means of commutative operands
    m_hash += (hash(std::forward<Args>(i_args)) + ...);

    return m_hash;
}

}
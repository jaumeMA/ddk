
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
template <class T>
constexpr uint32_t hash_combine(uint32_t& combined,const T& v)
{
    combined ^= (hash(v) + 0x9e3779b9) + (combined << 6) + (combined >> 2);

    return combined;
}
template <class T>
constexpr uint64_t hash_combine(uint64_t& combined,const T& v)
{
    combined ^= (hash(v) + 0x9e3779b9) + (combined << 6) + (combined >> 2);

    return combined;
}
template <typename Hasher,typename T>
constexpr uint32_t hash_combine(const Hasher& i_hasher,const T& v)
{
    size_t combined = i_hasher.get();

    combined ^= (hash(i_hasher,v) + 0x9e3779b9) + (combined << 6) + (combined >> 2);

    return combined;
}

constexpr builtin_hasher::builtin_hasher(size_t i_seed)
    : m_hash(i_seed)
{
}
template<typename ... Args>
size_t builtin_hasher::operator()(Args&& ... i_args) const
{
    const size_t accHash[] = { hash_combine(*this,std::forward<Args>(i_args)) ... };

    m_hash = accHash[mpl::num_types<Args...>-1];

    return m_hash;
}
constexpr size_t builtin_hasher::get() const
{
    return m_hash;
}

template<typename ... Args>
size_t commutative_builtin_hasher::operator()(Args&& ... i_args) const
{
    return (hash(*this,std::forward<Args>(i_args)) + ...);
}

}
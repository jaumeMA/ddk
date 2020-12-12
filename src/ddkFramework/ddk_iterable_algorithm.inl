
namespace ddk
{
namespace algo
{

template<typename PredicateA,typename PredicateB>
template<typename PPredicateA,typename PPredicateB>
swap<PredicateA,PredicateB>::swap(PPredicateA&& i_predicateA,PPredicateB&& i_predicateB)
: m_predicateA(std::forward<PPredicateA>(i_predicateA))
, m_predicateB(std::forward<PPredicateB>(i_predicateB))
{
}

}
}
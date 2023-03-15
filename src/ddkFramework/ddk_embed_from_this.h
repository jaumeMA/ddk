#pragma once

#include "ddk_embedded_reference.h"
#include "ddk_point_from_this.h"

namespace ddk
{

template<typename T, typename TT>
class embed_from_this
{
	typedef TT interface_type;

	template<typename TTT, typename TTTT>
	friend inline embedded_ref<TTTT> lend(embed_from_this<TTT,TTTT>& i_ref);
	template<typename TTT,typename TTTT>
	friend inline embedded_ref<const TTTT> lend(const embed_from_this<TTT,TTTT>& i_ref);

public:
	embed_from_this() = default;
};

template<typename T>
class point_from_this<embedded_ptr<T>> : public embed_from_this<T,T>
{
public:
	using embed_from_this<T,T>::embed_from_this;
};

}
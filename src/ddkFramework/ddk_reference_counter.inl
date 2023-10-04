
#include "ddk_pointer_exceptions.h"

namespace ddk
{

template<typename T,typename Deleter,typename ReferenceCounter>
share_control_block<T,Deleter,ReferenceCounter>::share_control_block(T* i_ptr,const Deleter& i_deleter)
: m_data(i_ptr,i_deleter)
{
}
template<typename T,typename Deleter,typename ReferenceCounter>
share_control_block<T,Deleter,ReferenceCounter>& share_control_block<T,Deleter,ReferenceCounter>::operator=(const share_control_block& other)
{
	m_data.set_first(other.m_data.get_first());
	m_data.set_second(other.m_data.get_second());

	return *this;
}
template<typename T,typename Deleter,typename ReferenceCounter>
void share_control_block<T,Deleter,ReferenceCounter>::destroy_shared_resource(short i_tagCategory)
{
	const bool hasWeakReferences = this->hasWeakReferences();

	if(T* ptr = m_data.get_first())
	{
		const Deleter _deleter = m_data.get_second();

		if(hasWeakReferences == false)
		{
			if(i_tagCategory == ReferenceAllocationType::Dynamic)
			{
				delete this;
			}
			else if(i_tagCategory == ReferenceAllocationType::Contiguous)
			{
				this->~share_control_block<T,Deleter,ReferenceCounter>();
			}
		}

		_deleter.deallocate(ptr);
	}
	else
	{
		throw null_pointer_exception{"Trying to deallocate null pointer."};
	}
}

template<typename T,typename Deleter>
unique_control_block<T,Deleter>::unique_control_block(T* i_ptr,const Deleter& i_deleter)
: m_data(i_ptr,i_deleter)
{
}
template<typename T,typename Deleter>
void unique_control_block<T,Deleter>::destroy_unique_resource(short i_tagCategory)
{
	if(T* ptr = m_data.get_first())
	{
		const Deleter _deleter = m_data.get_second();

		if(i_tagCategory == ReferenceAllocationType::Dynamic)
		{
			delete this;
		}
		else if(i_tagCategory == ReferenceAllocationType::Contiguous)
		{
			this->~unique_control_block<T,Deleter>();
		}

		_deleter.deallocate(ptr);
	}
	else
	{
		throw null_pointer_exception{ "Trying to deallocate null pointer." };
	}
}

}
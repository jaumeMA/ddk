#include "test_utils.h"
#include <utility>

DefaultType::DefaultType(int i_value)
: m_value(i_value)
{
}
bool DefaultType::operator==(int i_value) const
{
	return m_value == i_value;
}
int DefaultType::getValue() const
{
	return m_value;
}
void DefaultType::setValue(int i_value)
{
	m_value = i_value;
}
bool DefaultType::operator==(const DefaultType& other) const
{
	return m_value == other.m_value;
}
bool DefaultType::operator!=(const DefaultType& other) const
{
	return m_value != other.m_value;
}
bool DefaultType::operator<(const DefaultType& other) const
{
	return m_value < other.m_value;
}
bool DefaultType::operator>(const DefaultType& other) const
{
	return m_value > other.m_value;
}

size_t ConstructionDeletionBalancer::s_constructionDeletionBalance = 0;

ConstructionDeletionBalancer::ConstructionDeletionBalancer(int i_value)
: DefaultType(i_value)
{
	s_constructionDeletionBalance++;
}
ConstructionDeletionBalancer::ConstructionDeletionBalancer(const ConstructionDeletionBalancer& other)
: DefaultType(other)
{
	s_constructionDeletionBalance++;
}
ConstructionDeletionBalancer::ConstructionDeletionBalancer(ConstructionDeletionBalancer&& other)
: DefaultType(std::move(other))
{
}
ConstructionDeletionBalancer::~ConstructionDeletionBalancer()
{
	s_constructionDeletionBalance--;
}
bool ConstructionDeletionBalancer::isBalanced()
{
	return s_constructionDeletionBalance == 0;
}
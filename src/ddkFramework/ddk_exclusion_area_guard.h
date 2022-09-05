#pragma once

#include "ddk_exclusion_area.h"

namespace ddk
{

struct guard_reader
{
public:
	guard_reader(exclusion_area& m_exclArea);
	~guard_reader();

private:
	exclusion_area& m_exclArea;
};

struct guard_writer
{
public:
	guard_writer(exclusion_area& m_exclArea);
	~guard_writer();

private:
	exclusion_area& m_exclArea;
};

}
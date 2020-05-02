#pragma once

#include <SpikingNetworks/core/Macros.h>

namespace SpikingNetworks::exception
{

	SN_EXCEPTION(CellError)
	SN_EXCEPTION(SegmentError)
	SN_EXCEPTION(InvalidParameter)
	SN_EXCEPTION(ObjectPropertyRequired)
	SN_EXCEPTION(ObjectPropertyUnknown)

}
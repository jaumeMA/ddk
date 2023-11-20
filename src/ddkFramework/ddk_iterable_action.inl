
#include "ddk_variant.h"
#include "ddk_scoped_enum.h"
#include "ddk_error.h"
#include "ddk_result.h"

namespace ddk
{

SCOPED_ENUM_DECL(EraseActionError,
                 NonExistingValue,
                 ErasingFromConstantIterable);
typedef error<EraseActionError> erase_error;
typedef result<void,erase_error> erase_result;

SCOPED_ENUM_DECL(AddActionError,
                NonConvertibleType,
                AddingToConstantIterable,
                AddedItemFiltered);
typedef error<AddActionError> add_error;
typedef result<void,add_error> add_result;

SCOPED_ENUM_DECL(ShiftError,
                 Error);

struct shift_error : public error<ShiftError>
{
public:
    shift_error(const ShiftError& i_error, int i_pendingShift);
    shift_error(const ShiftError& i_error, const std::string& i_errorDesc, long long int i_pendingShift);
    shift_error(const shift_error&) = default;

    shift_error& operator=(const shift_error&) = default;

    long long int get_pending_shift() const;

private:
    long long int _pendingShift;
};
typedef result<void,shift_error> shift_result;

SCOPED_ENUM_DECL(StopError,
                 Error);

struct stop_error : public error<StopError>
{
public:
    stop_error(const StopError& i_error, int i_code = 0);
    stop_error(const StopError& i_error, const std::string& i_errorDesc, int i_code = 0);
    stop_error(const stop_error&) = default;

    stop_error& operator=(const stop_error&) = default;

    template<typename Enum>
    inline Enum get_code() const
    {
        return static_cast<Enum>(_code);
    }

private:
    int _code;
};

template<typename T,typename TT>
inline T action_conversion(const TT& i_action)
{
    return ddk::visit([](auto&& i_value) -> T
    {
        return i_value;
    },i_action);
}

const extern stop_action stop_iteration;
const extern go_to_begin_action go_to_begin;
const extern go_to_end_action go_to_end;
const extern forward_action go_next_place;
const extern backward_action go_prev_place;

}
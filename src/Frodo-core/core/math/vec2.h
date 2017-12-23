#include "mathcommon.h"

namespace fd {
namespace core {
namespace math {

template<typename T = float32>
class vec2_t {
public:
	T x;
	T y;

public:
	vec2_t();
	vec2_t(T x, T y);

	vec2_t& Add(const vec2_t& v);
	vec2_t& Add(T v);
	vec2_t& Sub(const vec2_t& v);
	vec2_t& Sub(T v);
	vec2_t& Mul(const vec2_t& v);
	vec2_t& Mul(T v);
	vec2_t& Div(const vec2_t& v);
	vec2_t& Div(T v);
};

typedef vec2_t<float32> vec2;
typedef vec2_t<float64> vec2d;
typedef vec2_t<int32> vec2i;
typedef vec2_t<int64> vec2l;

}}}

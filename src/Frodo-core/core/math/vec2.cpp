#include "vec2.h"

namespace fd {
namespace core {
namespace math {

template<typename T>
vec2_t<T>::vec2_t() : x(0), y(0) { }

template<typename T>
vec2_t<T>::vec2_t(T x, T y) : x(x), y(y) {}


#pragma region vec-vec
//Asss
//float32
template<> vec2_t<float32>& vec2_t<float32>::Add(const vec2_t<float32>& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	
	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Add(const vec2_t<float64>& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_add_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Add(const vec2_t<int32>& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

template<> vec2_t<int64>& vec2_t<int64>::Add(const vec2_t<int64>& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//sub
//float32
template<> vec2_t<float32>& vec2_t<float32>::Sub(const vec2_t<float32>& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Sub(const vec2_t<float64>& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_sub_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Sub(const vec2_t<int32>& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

template<> vec2_t<int64>& vec2_t<int64>::Sub(const vec2_t<int64>& v) {
	__int64 a = v.x;
	__int64 b = v.y;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//Mul
//float32
template<> vec2_t<float32>& vec2_t<float32>::Mul(const vec2_t<float32>& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Mul(const vec2_t<float64>& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_mul_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Mul(const vec2_t<int32>& v) {
	__m128i vxmm = _mm_set_epi32(0, v.y, 0, v.x);
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];

	return *this;
}

//int64
template<> vec2_t<int64>& vec2_t<int64>::Mul(const vec2_t<int64>& v) {
	x *= v.x;
	y *= v.y;

	return *this;
}

//Div
//float32
template<> vec2_t<float32>& vec2_t<float32>::Div(const vec2_t<float32>& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Div(const vec2_t<float64>& v) {
	__m128d vxmm = _mm_set_pd(v.y, v.x);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_div_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Div(const vec2_t<int32>& v) {
	x /= v.x;
	y /= v.y;

	return *this;
}

template<> vec2_t<int64>& vec2_t<int64>::Div(const vec2_t<int64>& v) {
	x /= v.x;
	y /= v.y;

	return *this;
}

#pragma endregion

#pragma region vec-shit

//Add
//float32
template<> vec2_t<float32>& vec2_t<float32>::Add(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Add(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_add_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Add(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v, v);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int64
template<> vec2_t<int64>& vec2_t<int64>::Add(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_add_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//Sub
//float32
template<> vec2_t<float32>& vec2_t<float32>::Sub(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Sub(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_sub_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Sub(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v, v);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[1];

	return *this;
}

//int64
template<> vec2_t<int64>& vec2_t<int64>::Sub(int64 v) {
	__int64 a = v;
	__int64 b = v;
	__m128i vxmm = _mm_set_epi64x(b, a);
	a = x;
	b = y;
	__m128i xmm = _mm_set_epi64x(b, a);
	xmm = _mm_sub_epi64(xmm, vxmm);

	x = xmm.m128i_i64[0];
	y = xmm.m128i_i64[1];

	return *this;
}

//Mul
//float32
template<> vec2_t<float32>& vec2_t<float32>::Mul(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Mul(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_mul_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Mul(int32 v) {
	__m128i vxmm = _mm_set_epi32(0, v, 0, v);
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, vxmm);

	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];

	return *this;
}

//int64
template<> vec2_t<int64>& vec2_t<int64>::Mul(int64 v) {
	x *= v;
	y *= v;

	return *this;
}

//Div
//float32
template<> vec2_t<float32>& vec2_t<float32>::Div(float32 v) {
	__m128 vxmm = _mm_set_ps(0, 0, v, v);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);

	x = xmm.m128_f32[0];
	y = xmm.m128_f32[1];

	return *this;
}

//float64
template<> vec2_t<float64>& vec2_t<float64>::Div(float64 v) {
	__m128d vxmm = _mm_set_pd(v, v);
	__m128d xmm = _mm_set_pd(y, x);
	xmm = _mm_div_pd(xmm, vxmm);

	x = xmm.m128d_f64[0];
	y = xmm.m128d_f64[1];

	return *this;
}

//int32
template<> vec2_t<int32>& vec2_t<int32>::Div(int32 v) {
	x /= v;
	y /= v;

	return *this;
}

template<> vec2_t<int64>& vec2_t<int64>::Div(int64 v) {
	x /= v;
	y /= v;

	return *this;
}


#pragma endregion

template vec2_t<float32>;
template vec2_t<float64>;
template vec2_t<int32>;
template vec2_t<int64>;

}}}
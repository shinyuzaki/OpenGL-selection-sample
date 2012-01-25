#ifndef _OPT_CG_h
#define _OPT_CG_h

#include <math.h>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <strstream>
#include <vector>

#ifdef WIN32
#pragma warning(disable:4200)
#pragma warning(disable:4786)
#ifdef _AFXDLL
#include <afx.h>
#else // _AFXDLL
#include <windows.h>
#endif // _AFXDLL
#endif // WIN32

#ifndef uint
	typedef unsigned int uint;
#endif // UINT

#ifndef uchar
	typedef unsigned char uchar;
#endif // UCHAR
#ifndef byte
	typedef unsigned char byte;
#endif // byte

#ifndef ulong
	typedef unsigned long ulong;
#endif // ULONG

#ifndef ushort
	typedef unsigned short ushort;
#endif // USHORT

#if _MSC_VER > 1300
	typedef long long dlong;
	typedef unsigned long long udlong;
#endif

#ifndef max
	#define max(a, b)	((a) > (b) ? (a) : (b))
#endif
#ifndef MAX
	#define MAX(a, b)	((a) > (b) ? (a) : (b))
#endif	
#ifndef min
	#define min(a, b)	((a) < (b) ? (a) : (b))
#endif
#ifndef MIN
	#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif
#ifndef M_PI
	#define M_PI		3.141592653f
#endif
#ifndef M_2_PI
	#define M_2_PI		0.636619772f		// 2 / PI
#endif
#ifndef M_PI_4
	#define M_PI_4		0.785398163f		// PI / 4
#endif
#ifndef M_PI_2
	#define M_PI_2		1.570796326f		// PI / 2
#endif
#ifndef M_PI_3_4
	#define M_PI_3_4	2.356194490f		// PI * 3/4
#endif
#ifndef M_1_2PI
	#define M_1_2PI		0.159154943f		// 1 / (2 * PI)
#endif
#ifndef M_1_4PI
	#define M_1_4PI		0.079577471f		// 1 / (4 * PI)
#endif

namespace CG
{
	inline float pi()  { return 3.14159265358979323846f; }
	inline float infinity()	{ return 1.0e+38f; }
	inline float epsiron() { return 1.0e-7f; }

	template <class T>
		inline const T& MIN_T(const T& a, const T& b) { return a < b ? a : b; }
	template <class T>
		inline const T& MAX_T(const T& a, const T& b) { return a > b ? a : b; }

	template <class T>
		inline T radians(const T& x) { return x*T(pi())/T(180); }
	template <class T>
		inline T degrees(const T& x) { return x*T(180)/T(pi()); }
	template <class T>
		inline T clamp(const T& a, const T& low, const T& high) {
			T ret = a;
			if (a < low) ret = low;
			else if (high < a) ret = high;
			return ret;
		}
	template <class T>
		inline T sign(const T& x) { return (x<T(0)) ? T(-1) : T(1); }
	template <class T>
		inline T round(const T& x) { return floor(x+T(0.5)); }
	template <class T>
		inline T step(const T& _min, const T& value) { return (value < _min) ? T(0) : T(1); }
	template <class T>
		inline T smoothstep(const T& _min, const T& _max, const T& value) {
			T v = clamp((value-_min)/(_max-_min),T(0),T(1));
			return v*v*(T(3)-T(2)*v);
		}
	template <class T>
		inline bool is_zero(const T& x) { return fabs(x) < T(epsiron()); }

	template <class T>
		inline int min_comp_index(const T t[3])
		{
			if (t[0] < t[1]) {
				if (t[0] < t[2])
					return 0;
				return 2;
			}
			if (t[1] < t[2])
				return 1;
			return 2;
		}
		
	template <class T>
		inline int min_abs_comp_index(const T t[3])
		{
			if (fabs(t[0]) < fabs(t[1])) {
				if (fabs(t[0]) < fabs(t[2]))
					return 0;
				return 2;
			}
			if (fabs(t[1]) < fabs(t[2]))
				return 1;
			return 2;
		}
	template <class T>
		inline int max_comp_index(const T t[3])
		{
			if (t[0] > t[1]) {
				if (t[0] > t[2])
					return 0;
				return 2;
			}
			if (t[1] > t[2])
				return 1;
			return 2;
		}
	template <class T>
		inline int max_abs_comp_index(const T t[3])
		{
			if (fabs(t[0]) > fabs(t[1])) {
				if (fabs(t[0]) > fabs(t[2]))
					return 0;
				return 2;
			}
			if (fabs(t[1]) > fabs(t[2]))
				return 1;
			return 2;
		}
	template <class T>
		inline T min_comp(const T t[3])
		{
			if (t[0] < t[1]) {
				if (t[0] < t[2])
					return t[0];
				return t[2];
			}
			if (t[1] < t[2])
				return t[1];
			return t[2];
		}
	template <class T>
		inline T max_comp(const T t[3])
		{
			if (t[0] > t[1]) {
				if (t[0] > t[2])
					return t[0];
				return t[2];
			}
			if (t[1] > t[2])
				return t[1];
			return t[2];
		}
	// add yuza
	template <class T>
		T to_pow2(const T& v)
		{
			return T(pow(2.0, ceil(log(double(v))/log(2.0))));
		}
	template <class T, int L>
			const char* string_to_numeric_array(const char* str, T* p, bool fill = false)
		{
			int i;
			for (i = 0; i < L && *str ; i++) {
				for (; *str && !::isdigit(uint(*str)) && *str != '+' && *str != '-' && *str != '.' && *str != 'e'; str++);
				p[i] = T(atof(str));
				for (; *str && (::isdigit(uint(*str)) || *str == '+' || *str == '-' || *str == '.' || *str == 'e'); str++);
			}
			if (fill) {
				int j;
				for (j = i; j < L; j++) {
					p[j] = p[i];
				}
			}
			return str;
		}
	template <class T>
		T from_string(const char* str)
		{
			std::istrstream s(str);
			T p;
			s >> p;
			return p;
		}
	template <class T>
	T from_string(const std::string& str)
		{
			return from_string<T>(str.c_str());
		}
	template <class T>
		std::string to_string(const T& p)
		{
#if defined(XRPOBJECT_PANASONIC_3DDTV)
			char result[512], buf[256];
			strcpy(result, "");

			double tmp = (double)p;

			sprintf(buf, "%g", tmp);
			strcat(result, buf);
			return std::string(result);
#else
			std::ostrstream s;
			s << p;
			return std::string(s.str(), s.pcount());
#endif
		}
	template <class T, int L>
		std::string array_to_string(const T* p, const char delim = ',')
		{
#if defined(XRPOBJECT_PANASONIC_3DDTV)
			char result[512], buf[256];
			strcpy(result, "");
			int i;

			double tmp;

			for (i = 0; i < L; i++, p++) {
				tmp = (double)(*p);
				strcpy(buf, "");
				sprintf(buf, "%g", tmp);
				strcat(result, buf);
				if (i != L - 1) {
					strcpy(buf, "");
					//sprintf(buf, &delim);
					sprintf(buf, ",");
					strcat(result, buf);
				}
			}
			return std::string(result);
#else
			std::ostrstream s;
			int i;
			for (i = 0; i < L; i++, p++) {
				s << *p;
				if (i != L - 1) {
					s << delim;
				}
			}

			return std::string(s.str(), s.pcount());
#endif
		}
	template <class T, int L>
		bool inverse_matrix(const T* src, T* dst)
	{
		// use gauss-jordan method
		int i, j, k, idk, idx[L];
		T a[L][L+L], amax=0, x1;
		const T* m[L];
		T* result[L];

		for (i = 0; i < L; i++) {
			m[i] = src + L * i;
			result[i] = dst + L * i;
		}

		for (i = 0; i < L; i++) {
			idx[i] = i;
			for (j = 0; j < L; j++) {
				a[i][j] = m[i][j];
				a[i][j+L] = ((i == j) ? T(1) : 0);
			}
		}
		for (k = 0; k < L; k++) {
			// pivotting
			while (1) {
				if (k == L-1) break; // pivot end
				amax = fabs(a[k][k]);
				idk = k;
				for (i = k + 1; i < L; i++) {
					x1 = fabs(a[i][k]);
					if (x1 > amax) {
						idk = i;
						amax = x1;
					}
				}
				if (amax <= epsiron()) {
					//cerr << "pivot error." << endl;
					return false;
				}
				if (idk == k) break; // pivot end
				for (j = 0; j < L+L; j++) {
					std::swap(a[idk][j], a[k][j]);
				}
				std::swap(idx[idk], idx[k]);

				break; // pivot end
			}

			// gauss jordan
			x1 = T(1.0 / double(a[k][k]));
			for (j = 0; j < L+L; j++) a[k][j] = a[k][j]*x1;
			a[k][k] = 1;
			for (i = 0; i < L; i++) {
				if (i == k) continue;
				x1 = a[i][k];
				for (j = 0; j < L+L; j++) {
					if (j == k) {
						a[i][k] = 0;
						continue;
					}
					a[i][j] = a[i][j]-x1*a[k][j];
				}
			}
		}
		for (i = 0; i < L; i++)
			for (j = 0; j < L; j++)
				result[i][j] = a[i][j+L];

		return true;
	}
	// add yuza

	template <class T> struct vec2;
	template <class T> struct vec3;
	template <class T> struct vec4;
	// add yuza
	template <class T> struct mat2;
	// add yuza
	template <class T> struct mat3;
	template <class T> struct mat4;
	template <class T> struct rotation;
	template <class T> struct quat;

	template <class T>
	struct vec2
	{
		T x, y;

		vec2() {}
		explicit vec2(const T& _x, const T& _y) : x(_x), y(_y) {}
		// mod yuza
		vec2(const T& f) : x(f), y(f) {}
		//explicit vec2(const T& f) : x(f), y(f) {}
		// mod yuza
		explicit vec2(const T* _v) : x(_v[0]), y(_v[1]) {}
		vec2(const vec2<float>& _v) : x(T(_v.x)), y(T(_v.y)) {}
		vec2(const vec2<double>& _v) : x(T(_v.x)), y(T(_v.y)) {}
		operator T* () { return &x; }
		operator const T*() const { return &x; }
		// add yuza
		vec2(const vec3<float>& _v) : x(_v.x), y(_v.y) {}
		explicit vec2(const char* str, const vec2<T>& _v = vec2<T>(T(0)))
		{
			*this = _v;
			string_to_numeric_array<T,2>(str, *this);
		}
#ifdef WIN32
		vec2(const POINT &p) : x(T(p.x)), y(T(p.y)) {}
		operator POINT() const
		{
			POINT p;
			p.x = int(x);
			p.y = int(y);
			return p;
		}
#endif // WIN32
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,2>(*this, delim);
		}
		// add yuza

		bool operator== (const vec2& v) const { return x==v.x && y==v.y; }
		bool operator!= (const vec2& v) const { return !((*this) == v); }
		//	ベクトル同士の大小比較は、x,yの順番に比較する辞書引きのような順番と規定します。
		// 一般にこれをlexicographical orderと呼んでいるそうです。
		bool operator<  (const vec2& v) const
		{
			if (x < v.x)
				return true;
			if (x == v.x)
				return y < v.y;
			return false;
		}
		bool operator>  (const vec2& v) const
		{
			if (x > v.x)
				return true;
			if (x == v.x)
				return y > v.y;
			return false;
		}
		bool operator<= (const vec2& v) const { return !(*this>v); }
		bool operator>= (const vec2& v) const { return !(*this<v); }
		bool operator== (const T& f) const { return is_zero(x-f)&&is_zero(y-f); }
		bool operator!= (const T& f) const { return !((*this)==f); }
		bool operator<  (const T& f) const { return x < f && y < f; }
		bool operator>  (const T& f) const { return x > f && y > f; }

		vec2& operator= (const T& f) { x=f, y=f; return *this; }
		vec2& operator+=(const T& s) { x+=s, y+= s; return *this; }
		vec2& operator-=(const T& s) { x-=s, y-= s; return *this; }
		vec2& operator*=(const T& s) { x*=s, y*= s; return *this; }
		vec2& operator/=(const T& s) { x/=s, y/= s; return *this; }
		vec2& operator+=(const vec2& v) { x+=v.x, y+=v.y; return *this; }
		vec2& operator-=(const vec2& v) { x-=v.x, y-=v.y; return *this; }
		vec2& operator*=(const vec2& v) { x*=v.x, y*=v.y; return *this; }
		vec2& operator/=(const vec2& v) { x/=v.x, y/=v.y; return *this; }
		vec2 operator-  () const { return vec2(-x,-y); }
		vec2 operator + (const T& s) const { return vec2(x+s,y+s); }
		vec2 operator - (const T& s) const { return vec2(x-s,y-s); }
		vec2 operator * (const T& s) const { return vec2(x*s,y*s); }
		vec2 operator / (const T& s) const { return vec2(x/s,y/s); }
		vec2 operator + (const vec2& v) const { return vec2(x+v.x,y+v.y); }
		vec2 operator - (const vec2& v) const { return vec2(x-v.x,y-v.y); }
		vec2 operator * (const vec2& v) const { return vec2(x*v.x,y*v.y); }
		vec2 operator / (const vec2& v) const { return vec2(x/v.x,y/v.y); }
		T length2() const { return x*x + y*y; }
		T length() const { return sqrt(length2()); }
		vec2 normalize() const { T d = length(); return (d > T(0)) ? *this/d : *this; } 
		vec2& normalize() { T d = length(); if (d > T(0)) (*this)/=d; return *this; }
		vec2 normalvec() const { return vec2(-y,x); }
		T dot(const vec2<T>& b) const { return x*b.x + y*b.y; }
		T cross(const vec2& b) const { return x*b.y - y*b.x; }
		T max_component() const {
			return MAX(x,y);
		}
		T min_component() const {
			return MIN(x,y);
		}
	};

	template <class T>
		inline void normalize(vec2<T>& V) {
			T d = V.length();
			if (d > T(0))
				V /= d;
		}
	template <class T> inline vec2<T> operator+  (const T& f, const vec2<T>& v) { return v+f; }
	template <class T> inline vec2<T> operator-  (const T& f, const vec2<T>& v) { return vec2<T>(f-v[0],f-v[1],f-v[2]); }
	template <class T> inline vec2<T> operator*  (const T& f, const vec2<T>& v) { return v*f; }
	template <class T> inline vec2<T> operator/  (const T& f, const vec2<T>& v) { return vec2<T>(f/v[0],f/v[1],f/v[2]); }
	template <class T> inline bool	  operator== (const T& f, const vec2<T>& v) { return v==f; }
	template <class T> inline bool	  operator!= (const T& f, const vec2<T>& v) { return v!=f; }
	template <class T> inline T dot(const vec2<T>& a, const vec2<T>& b) { return a.dot(b); }
	template <class T> inline T cross(const vec2<T>& a, const vec2<T>& b) { return a.cross(b); }

	// add yuza
#ifdef _AFXDLL
	template <class T>
		inline CArchive& operator<<(CArchive& s, const vec2<T>& v)
	{
		s << v.x;
		s << v.y;
		return s;
	}

	template <class T>
		inline CArchive& operator>>(CArchive& s, vec2<T>& v)
	{
		s >> v.x;
		s >> v.y;
		return s;
	}
#endif // _AFXDLL
	// add yuza

	template <class T>
	struct vec3
	{
		T x, y, z;

		vec3() {}
		vec3(const T& f) : x(f), y(f), z(f) {}
		explicit vec3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
		explicit vec3(const T* _v) : x(_v[0]), y(_v[1]), z(_v[2]) {}
		vec3(const vec3<float>& _v) : x(T(_v.x)), y(T(_v.y)), z(T(_v.z)) {}
		vec3(const vec3<double>& _v) : x(T(_v.x)), y(T(_v.y)), z(T(_v.z)) {}
		vec3(const vec2<T>& v, const T& _z = T(0)) : x(v.x), y(v.y), z(_z) {}
		vec3(const vec4<T>& v);
		// add yuza
		explicit vec3(const char* str, const vec3<T>& _v = vec3<T>(T(0)))
		{
			*this = _v;
			string_to_numeric_array<T,3>(str, *this);
		}
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,3>(*this, delim);
		}
		// add yuza
		operator T* () { return &x; }
		operator const T*() const { return &x; }
		bool operator== (const vec3& v) const { return is_zero(x-v.x) && is_zero(y-v.y) && is_zero(z-v.z); }
		bool operator!= (const vec3& v) const { return !((*this) == v); }
		//	ベクトル同士の大小比較は、x,y,zの順番に比較する辞書引きのような順番と規定します。
		// 一般にこれをlexicographical orderと呼んでいるそうです。
		bool operator<  (const vec3& v) const
		{ 
			if (x < v.x)
				return true;
			if (x == v.x)
			{
				if (y < v.y)
					return true;
				if (y == v.y)
					return z < v.z;
			}
			return false;
		}
		bool operator>  (const vec3& v) const
		{
			if (x > v.x)
				return true;
			if (x == v.x)
			{
				if (y > v.y)
					return true;
				if (y == v.y)
					return z > v.z;
			}
			return false;
		}
		bool operator<= (const vec3& v) const { return !(*this>v); }
		bool operator>= (const vec3& v) const { return !(*this<v); }

		bool operator== (const T& f) const { return is_zero(x-f)&&is_zero(y-f)&& is_zero(z-f); }
		bool operator!= (const T& f) const { return !((*this)==f); }
		//	ベクトルとスカラーの比較は、よくif (v < f) {}という構文を使うので
		//	各項目とスカラー値との比較した結果のアンドと規定しました。
		//	つまり、すべての値がある値より大きいか小さいかを比較することとしました。
		//
		bool operator<  (const T& f) const { return x < f && y < f && z < f; }
		bool operator>  (const T& f) const { return x > f && y > f && z > f; }

		//	代入演算子
		vec3<T>& operator= (const T& f) { x=f, y=f, z=f; return *this; }
		vec3<T>& operator+=(const T& s) { x+=s, y+=s, z+=s; return *this; }
		vec3<T>& operator-=(const T& s) { x-=s, y-=s, z-=s; return *this; }
		vec3<T>& operator*=(const T& s) { x*=s, y*=s, z*=s; return *this; }
		vec3<T>& operator/=(const T& s) { T i=T(1)/s; x*=i, y*=i, z*=i; return *this; }
		vec3<T>& operator+=(const vec3& v) { x+=v.x, y+=v.y, z+=v.z; return *this; }
		vec3<T>& operator-=(const vec3& v) { x-=v.x, y-=v.y, z-=v.z; return *this; }
		vec3<T>& operator*=(const vec3& v) { x*=v.x, y*=v.y, z*=v.z; return *this; }
		vec3<T>& operator/=(const vec3& v) { x/=v.x, y/=v.y, z/=v.z; return *this; }

		//	単項演算子
		vec3<T> operator-  () const { return vec3(-x,-y,-z); }

		//	２項演算子
		vec3<T> operator + (const T& s) const { return vec3<T>(x+s,y+s,z+s); }
		vec3<T> operator - (const T& s) const { return vec3<T>(x-s,y-s,z-s); }
		vec3<T> operator * (const T& s) const { return vec3<T>(x*s,y*s,z*s); }
		vec3<T> operator / (const T& s) const { T i = T(1)/s; return vec3(x*i,y*i,z*i); }
		vec3<T> operator + (const vec3& v) const { return vec3<T>(x+v.x,y+v.y,z+v.z); }
		vec3<T> operator - (const vec3& v) const { return vec3<T>(x-v.x,y-v.y,z-v.z); }
		vec3<T> operator * (const vec3& v) const { return vec3<T>(x*v.x,y*v.y,z*v.z); }
		vec3<T> operator / (const vec3& v) const { return vec3<T>(x/v.x,y/v.y,z/v.z); }

		//	操作用関数
		T length2() const { return x*x + y*y + z*z; }
		T length() const { return sqrt(length2()); }
		T average() const { return (x+y+z)/T(3); }
		T composite() const { return x+y+z; }
		vec3 normalize() const { T d = length(); return (d > 0) ? (*this)/d : *this; }
		vec3& normalize() { T d = length(); if (d>T(0)) (*this)/=d; return *this; }
		T max_component() const {
			T ret = x;
			ret = MAX(ret,y);
			ret = MAX(ret,z);
			return ret;
		}
		T min_component() const {
			T ret = x;
			ret = MIN(ret,y);
			ret = MIN(ret,z);
			return ret;
		}
		T dot(const vec3& v) const { return x*v.x + y*v.y + z*v.z; }
		T dot(const vec4<T>& v) const;
		vec3<T> cross(const vec3& v) const { return vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
		vec3<T> transform_direction(const mat4<T>& m);
	};

	template <class T> inline T distance(const vec3<T>& p1, const vec3<T>& p2) { return (p1-p2).length(); }
	// ２点間の距離を計算します。
	template <class T> inline T distance2(const vec3<T>& p1, const vec3<T>& p2) { return (p1-p2).length2(); }
	// ２点間の距離の二乗を計算します。

	template <class T> inline vec3<T> operator+ (const T& f, const vec3<T>& v) { return v+f; }
	template <class T> inline vec3<T> operator- (const T& f, const vec3<T>& v) { return vec3<T>(f-v[0],f-v[1],f-v[2]); }
	template <class T> inline vec3<T> operator* (const T& f, const vec3<T>& v) { return v*f; }
	template <class T> inline vec3<T> operator/ (const T& f, const vec3<T>& v) { return vec3<T>(f/v[0],f/v[1],f/v[2]); }
	template <class T> inline bool operator== (const T& f, const vec3<T>& v) { return v==f; }
	template <class T> inline bool operator!= (const T& f, const vec3<T>& v) { return v!=f; }
	template <class T> inline T dot(const vec3<T>& a, const vec3<T>& b) { return a.dot(b); }
	template <class T> inline vec3<T> cross(const vec3<T>& a, const vec3<T>& b) { return a.cross(b); }

	// 入力されたベクトルを正規化し、正規化するときに求められた長さを戻します。
	template <class T> inline T normalize(vec3<T>& V) { T d = V.length(); if (d > T(0)) V /= d; return d; }

	template <class T> vec3<T> RGB_TO_CIEXYZ(const vec3<T>& rgb)
	{
		return vec3<T>(	T(0.67)*rgb.x + T(0.21)*rgb.y + T(0.14)*rgb.z,
						T(0.33)*rgb.x + T(0.71)*rgb.y + T(0.08)*rgb.z,
						T(0.00)*rgb.x + T(0.08)*rgb.y + T(0.78)*rgb.z);
	}

	template <class T> vec3<T> CIEXYZ_TO_RGB(const vec3<T>& xyz)
	{
		return vec3<T>(	T( 1.730)*xyz.x + T(-0.482)*xyz.y + T(-0.261)*xyz.z,
						T(-0.814)*xyz.x + T( 1.652)*xyz.y + T(-0.023)*xyz.z,
						T( 0.083)*xyz.x + T(-0.169)*xyz.y + T( 1.284)*xyz.z);
	}

	// add yuza
#ifdef _AFXDLL
	template <class T>
		inline CArchive& operator<<(CArchive& s, const vec3<T>& v)
	{
		s << v.x;
		s << v.y;
		s << v.z;
		return s;
	}

	template <class T>
		inline CArchive& operator>>(CArchive& s, vec3<T>& v)
	{
		s >> v.x;
		s >> v.y;
		s >> v.z;
		return s;
	}
#endif // _AFXDLL
	// add yuza

	template <class T>
	struct vec4
	{
		T x, y, z, w;

		vec4() {}
		explicit vec4(const T& f)  : x(f), y(f), z(f), w(f) {}
		explicit vec4(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) {}
		explicit vec4(const T *v)  : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
		explicit vec4(const vec2<T>& v, const T& _z = T(0), const T& _w = T(1))  : x(v.x), y(v.y), z(_z), w(_w) {}
		explicit vec4(const vec3<T>& v, const T& _w = T(1))  : x(v.x), y(v.y), z(v.z), w(_w) {}
		vec4(const vec4<float>& _v) : x(T(_v.x)), y(T(_v.y)), z(T(_v.z)), w(T(_v.w)) {}
		vec4(const vec4<double>& _v) : x(T(_v.x)), y(T(_v.y)), z(T(_v.z)), w(T(_v.w)) {}
		// add yuza
		explicit vec4(const char* str, const vec4<T>& _v = vec4<T>(T(0)))
		{
			*this = _v;
			string_to_numeric_array<T,4>(str, *this);
		}
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,4>(*this, delim);
		}
		bool operator<  (const vec4& v) const
		{ 
			if (x < v.x)
				return true;
			if (x == v.x)
			{
				if (y < v.y)
					return true;
				if (y == v.y)
				{
					if (z < v.z)
						return true;
					return w < v.w;
				}
			}
			return false;
		}
		bool operator>  (const vec4& v) const
		{
			if (x > v.x)
				return true;
			if (x == v.x)
			{
				if (y > v.y)
					return true;
				if (y == v.y)
				{
					if (z > v.z)
						return true;
					return w > v.w;
				}
			}
			return false;
		}
		bool operator<= (const vec4& v) const { return !(*this>v); }
		bool operator>= (const vec4& v) const { return !(*this<v); }
		// add yuza
		operator T* () { return &x; }
		operator const T*() const { return &x; }
		bool operator ==(const vec4<T>& v) const { return is_zero(x-v.x)&&is_zero(y-v.y)&&is_zero(z-v.z)&&is_zero(w-v.w); }
		bool operator !=(const vec4<T>& v) const { return !((*this) == v); }
		vec4<T>& operator= (const T& f) { x=f, y=f, z=f, w=T(1); return *this; }
		vec4<T>& operator+=(const T& f) { x+=f, y+=f, z+=f, w+=f; return *this; }
		vec4<T>& operator-=(const T& f) { x-=f, y-=f, z-=f, w-=f; return *this; }
		vec4<T>& operator*=(const T& f) { x*=f, y*=f, z*=f, w*=f; return *this; }
		vec4<T>& operator/=(const T& f) { T i = 1/f; return (*this)*=i; }
		vec4<T>& operator+=(const vec4<T>& v) { x+=v.x, y+=v.y, z+=v.z, w+=v.w; return *this; }
		vec4<T>& operator-=(const vec4<T>& v) { x-=v.x, y-=v.y, z-=v.z, w-=v.w; return *this; }
		vec4<T>& operator*=(const vec4<T>& v) { x*=v.x, y*=v.y, z*=v.z, w*=v.w; return *this; }
		vec4<T>& operator/=(const vec4<T>& v) { x/=v.x, y/=v.y, z/=v.z, w/=v.w; return *this; }
		vec4<T> operator-  () const { return vec4f(-x,-y,-z,-w); }
		vec4<T> operator + (const T& f) const { return vec4<T>(x+f,y+f,z+f,w+f); }
		vec4<T> operator - (const T& f) const { return vec4<T>(x-f,y-f,z-f,w-f); }
		vec4<T> operator * (const T& f) const { return vec4<T>(x*f,y*f,z*f,w*f); }
		vec4<T> operator / (const T& f) const { T i = 1/f; return *this*i; }
		vec4<T> operator + (const vec4<T>& v) const { return vec4<T>(x+v.x, y+v.y, z+v.z, w+v.w); }
		vec4<T> operator - (const vec4<T>& v) const { return vec4<T>(x-v.x, y-v.y, z-v.z, w-v.w); }
		vec4<T> operator * (const vec4<T>& v) const { return vec4<T>(x*v.x, y*v.y, z*v.z, w*v.w); }
		vec4<T> operator / (const vec4<T>& v) const { return vec4<T>(x/v.x, y/v.y, z/v.z, w/v.w); }
		T length() const { return sqrt(x*x + y*y + z*z); }
		vec4 normalize() const { T d = length(); return d > 0.0f ? (*this)/d : *this; }
		T dot(const vec3<T>& b) const { return x*b.x + y*b.y + z*b.z + w; }
		T dot(const vec4<T>& b) const { return x*b.x + y*b.y + z*b.z + w*b.w; }
	};
	template <class T> inline T vec3<T>::dot(const vec4<T>& v) const { return v.dot(*this); }
	template <class T> inline vec4<T> operator* (const T& f, const vec4<T>& v) { return v*f; }

	template <class T>
		inline std::ostream& operator<< (std::ostream& os, vec4<T>& v) {
			os<<v.x<<v.y<<v.z<<v.w;
			return os;
		}
	template <class T>
		inline std::istream& operator>> (std::istream& is, vec4<T>& v) {
			is>>v.x>>v.y>>v.z>>v.w;
			return is;
		}

	template <class T>
		inline T det2x2(const T& a, const T& b, const T& c, const T& d) { return a*d - b*c; }

	template <class T>
		inline T det3x3(const T& a0, const T& a1, const T& a2,
						const T& b0, const T& b1, const T& b2,
						const T& c0, const T& c1, const T& c2) {
			return	a0*det2x2(b1,b2,c1,c2) - a1*det2x2(b0,b2,c0,c2) + a2*det2x2(b0,b1,c0,c1);
		}
	template <class T>
		inline vec3<T>::vec3(const vec4<T>& v) { T i = T(1)/v.w; x=v.x*i, y=v.y*i, z=v.z*i; }

	// add yuza
#ifdef _AFXDLL
	template <class T>
		inline CArchive& operator<<(CArchive& s, const vec4<T>& v)
	{
		s << v.x;
		s << v.y;
		s << v.z;
		s << v.w;
		return s;
	}

	template <class T>
		inline CArchive& operator>>(CArchive& s, vec4<T>& v)
	{
		s >> v.x;
		s >> v.y;
		s >> v.z;
		s >> v.w;
		return s;
	}
#endif // _AFXDLL
	// add yuza

	template <class T>
	struct rotation
	{
		T		angle;
		vec3<T>	axis;

		rotation() : angle(T(0)), axis(T(1),T(0),T(0)) {}
		explicit rotation(const T rot[4]) : angle(rot[0]), axis(&rot[1]) {}
		rotation(const T& _angle, const T& _x, const T& _y, const T& _z)
			: angle(_angle)
		{
			axis = vec3f(_x, _y, _z).normalize();
		}
		rotation(const T& _angle, const vec3<T>& _axis)
			: angle(_angle)
		{
			axis = _axis.normalize();
		}
		rotation(const vec3<T> & from_vec, const vec3<T> & to_vec)
		{
			this->axis = from_vec.cross(to_vec);
			normalize(this->axis);
			this->angle = acos(from_vec.dot(to_vec)/(from_vec.length()*to_vec.length()));
		}
        explicit rotation(const quat<T>& q);
		// add yuza
		explicit rotation(const char* str, const rotation<T>& _r = rotation<T>(0,0,0,1))
			: angle(_r.angle)
			, axis(_r.axis)
		{
			str = string_to_numeric_array<T,1>(str, &angle);
			str = string_to_numeric_array<T,3>(str, axis);
			normalize(this->axis);
		}
		std::string to_string(const char delim = ',') const
		{
			return
				array_to_string<T,1>(&angle, delim) +
				delim +
				array_to_string<T,3>(axis, delim);
		}
		// add yuza
		rotation<T> & operator*=(const rotation<T>& rot);
		rotation<T> operator * (const rotation<T>& rot) const;
        rotation inverse() const { return rotation(-angle,axis); }
		rotation operator-() const { return rotation(-angle,axis); }
	};

	// add yuza
	template <class T>
	struct mat2
	{
		vec2<T> _0, _1;
		mat2() {}
		explicit mat2(const vec2<T>& v0, const vec2<T>& v1) : _0(v0), _1(v1) {}
		explicit mat2(const T& _00, const T& _01,
					  const T& _10, const T& _11) : _0(_00,_01), _1(_10,_11) {}
		explicit mat2(const T& f) : _0(f,0), _1(0,f) {}
		explicit mat2(const T* v[2] ) : _0(v[0]), _1(v[1]) {}
		explicit mat2(const T v[2][2] ) : _0(v[0]), _1(v[1]) {}
		explicit mat2(const char* str, const mat2<T>& _v = mat2<T>(T(1)))
		{
			*this = _v;
			string_to_numeric_array<T,4>(str, *this);
		}
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,4>(*this, delim);
		}

		vec2<T>& operator[] (int i) { return (&_0)[i]; }
		const vec2<T>& operator[] (int i) const { return (&_0)[i]; }
		mat2<T>& operator*= (const mat2<T>& m);
		mat2<T> operator* (const mat2<T>& m) const;
		operator T* () { return _0; }
		operator const T* () const { return _0; }
		void identity() { (*this) = mat2<T>(1); }
		void load_identity() { (*this) = T(1); }
		mat2<T> inverse() const
		{
			mat2<T> result;
			inverse_matrix<T,2>(*this, result);
			return result;
		}
	};
	template <class T>
	inline vec2<T> operator* (const vec2<T>& v, const mat2<T>& m) {
		return vec2<T>(v.x*m._0.x + v.y*m._1.x,
					   v.x*m._0.y + v.y*m._1.y);
	}
	template <class T>
	inline vec2<T> operator* (const mat2<T>& m, const vec2<T>& v) {
		return vec2<T>(v.x*m._0.x + v.y*m._0.x,
					   v.x*m._1.y + v.y*m._1.y);
	}

	template <class T>
	inline mat2<T>& mat2<T>::operator*= (const mat2<T>& m) { _0 = _0*m; _1 = _1*m; return *this; }

	template <class T>
	inline mat2<T> mat2<T>::operator* (const mat2<T>& m) const { return mat2(_0*m,_1*m); }
	// add yuza

	template <class T>
	struct mat3
	{
		vec3<T> _0, _1, _2;
		mat3() {}
		explicit mat3(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2 ) : _0(v0), _1(v1), _2(v2) {}
		explicit mat3(const vec3<T>& v) : _2(v) {
			normalize(_2);
			vec3<T> t = _2;
			t[min_abs_comp_index<T>(_2)] = 1.0f;
			_0 = t.cross(_2);
			normalize(_0);
			_1 = _2.cross(_0);
		}
		explicit mat3(const T& _00, const T& _01, const T& _02,
					  const T& _10, const T& _11, const T& _12,
					  const T& _20, const T& _21, const T& _22) : _0(_00,_01,_02), _1(_10,_11,_12), _2(_20,_21,_22) {}
		explicit mat3(const T& f) : _0(f,0,0), _1(0,f,0), _2(0,0,T(1)) {}
		explicit mat3(const T* v[3] ) : _0(v[0]), _1(v[1]), _2(v[2]) {}
		explicit mat3(const T v[3][3] ) : _0(v[0]), _1(v[1]), _2(v[2]) {}
		explicit mat3(const quat<T>& q);

		vec3<T>& operator[] (int i) { return (&_0)[i]; }
		const vec3<T>& operator[] (int i) const { return (&_0)[i]; }
		mat3<T>& operator*= (const mat3<T>& m);
		mat3<T> operator* (const mat3<T>& m) const;
		vec2<T> transform_point(const vec2<T>& v) const
		{
			return vec2<T>(v.x*_0.x + v.y*_1.x + _2.x, v.x*_0.y + v.y*_1.y + _2.y);
		}
		vec2<T> transform_direction(const vec2<T>& v) const
		{
			return vec2<T>(v.x*_0.x + v.y*_1.x, v.x*_0.y + v.y*_1.y);
		}
		vec3<T> transform_direction(const vec3<T>& v) const
		{
			return vec3<T>(	v.x*_0.x + v.y*_1.x + v.z*_2.x,
							v.x*_0.y + v.y*_1.y + v.z*_2.y,
							v.x*_0.z + v.y*_1.z + v.z*_2.z);
		}
		static mat3<T> translate(const T& tx, const T& ty)
		{
			return mat3<T>(T(1),T(0),T(0),
						   T(0),T(1),T(0),
						   tx,  ty,  T(1));
		}
		static mat3<T> rotate(const T& angle)
		{
			T c = T(cos(angle)), s = T(sin(angle));
			return mat3<T>(   c,   s,T(0),
						     -s,   c,T(0),
						   T(0),T(0),T(1));
		}
		static mat3<T> scale(const T& sx, const T& sy)
		{
			return mat3<T>(vec3<T>(sx,T(0),T(0)),vec3<T>(T(0),sy,T(0)),vec3<T>(T(0),T(0),T(1)));
		}
		// add yuza
		explicit mat3(const char* str, const mat3<T>& _v = mat3<T>(T(1)))
		{
			*this = _v;
			string_to_numeric_array<T,9>(str, *this);
		}
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,9>(*this, delim);
		}
		operator T* () { return _0; }
		operator const T* () const { return _0; }
		void identity() { (*this) = mat3<T>(1); }
		void load_identity() { (*this) = T(1); }
		mat3<T> inverse() const
		{
			mat3<T> result;
			inverse_matrix<T,3>(*this, result);
			return result;
		}
		// add yuza

		// add katsu->
		T determinant() const {
			return det3x3(_0[0],_0[1],_0[2],_1[0],_1[1],_1[2],_2[0],_2[1],_2[2]);
		}
		// add katsu<-
	};
	template <class T>
	inline vec2<T> transform_direction(const vec2<T>& v, const mat3<T> m)
	{
		return vec2<T>(v.x*m._0.x + v.y*m._1.x,
					   v.x*m._0.y + v.y*m._1.y);
	}

	template <class T>
	inline vec2<T> operator* (const vec2<T>& v, const mat3<T>& m)
	{
		return vec2<T>(v.x*m._0.x + v.y*m._1.x + m._2.x,
					   v.x*m._0.y + v.y*m._1.y + m._2.y);
	}

	template <class T>
	inline vec3<T> operator* (const vec3<T>& v, const mat3<T>& m) {
		return vec3<T>(v.x*m._0.x + v.y*m._1.x + v.z*m._2.x,
					   v.x*m._0.y + v.y*m._1.y + v.z*m._2.y,
					   v.x*m._0.z + v.y*m._1.z + v.z*m._2.z);
	}
	// add yuza
	template <class T>
	inline vec2<T> operator* (const mat3<T>& m, const vec2<T>& v)
	{
		return vec2<T>(v.x*m._0.x + v.y*m._0.x + m._0.x,
					   v.x*m._1.y + v.y*m._1.y + m._1.y);
	}

	template <class T>
	inline vec3<T> operator* (const mat3<T>& m, const vec3<T>& v) {
		return vec3<T>(v.x*m._0.x + v.y*m._0.x + v.z*m._0.x,
					   v.x*m._1.y + v.y*m._1.y + v.z*m._1.y,
					   v.x*m._2.z + v.y*m._2.z + v.z*m._2.z);
	}
	// add yuza

	template <class T>
	inline mat3<T>& mat3<T>::operator*= (const mat3<T>& m) { _0 = _0*m; _1 = _1*m; _2 = _2*m; return *this; }

	template <class T>
	inline mat3<T> mat3<T>::operator* (const mat3<T>& m) const { return mat3(_0*m,_1*m,_2*m); }

	template <class T>
		inline std::ostream& operator<< (std::ostream& os, mat3<T>& m) {
			os<<m._0<<m._1<<m._2;
			return os;
		}
	template <class T>
		inline std::istream& operator>> (std::istream& is, mat3<T>& m) {
			is>>m._0>>m._1>>m._2;
			return is;
		}




	template <class T>
	struct mat4
	{
		vec4<T> _0, _1, _2, _3;
	public:
		mat4() {}
		explicit mat4(const vec4<T>& v0, const vec4<T>& v1, const vec4<T>& v2, const vec4<T>& v3) : _0(v0), _1(v1), _2(v2), _3(v3) {}
		explicit mat4(const T& _00, const T& _01, const T& _02, const T& _03,
					  const T& _10, const T& _11, const T& _12, const T& _13,
					  const T& _20, const T& _21, const T& _22, const T& _23,
					  const T& _30, const T& _31, const T& _32, const T& _33)
				: _0(_00,_01,_02,_03)
				, _1(_10,_11,_12,_13)
				, _2(_20,_21,_22,_23)
				, _3(_30,_31,_32,_33) {}
		mat4(const T& f)
			: _0(f,T(0),T(0),T(0))
			, _1(T(0),f,T(0),T(0))
			, _2(T(0),T(0),f,T(0))
			, _3(T(0),T(0),T(0),T(1)) {}
		explicit mat4(const vec3<T>& v) : _0(1,0,0,0), _1(0,1,0,0), _2(0,0,1,0), _3(v) {}
		explicit mat4(const T* v) : _0(&v[0]), _1(&v[4]), _2(&v[8]), _3(&v[12]) {}
		explicit mat4(const T& angle, const vec3<T>& axis) {
			vec3<T> v = axis.normalize();
			T dx = v[0], dy = v[1], dz = v[2];
			T c = cos(angle);
			T s = sin(angle);
			T dx2 = dx*dx, dy2 = dy*dy, dz2 = dz*dz;
			T dxdy = dx*dy, dydz = dy*dz, dzdx = dz*dx;
			T a = 1-c;
			_0 = vec4<T>(dx2+(1-dx2)*c, dxdy*a+dz*s,   dzdx*a-dy*s,   0);
			_1 = vec4<T>(dxdy*a-dz*s,	dy2+(1-dy2)*c, dydz*a+dx*s,   0);
			_2 = vec4<T>(dzdx*a+dy*s,	dydz*a-dx*s,   dz2+(1-dz2)*c, 0);
			_3 = vec4<T>(0,				0,			   0,			  1);
		}
		explicit mat4(const T& sx, const T& sy, const T& sz) : _0(sx,0,0,0), _1(0,sy,0,0), _2(0,0,sz,0), _3(0,0,0,1) {}
		explicit mat4(const T* v[4])  : _0(v[0]), _1(v[1]), _2(v[2]), _3(v[3]) {}
		explicit mat4(const T v[][4]) : _0(v[0]), _1(v[1]), _2(v[2]), _3(v[3]) {}
		explicit mat4(const T& angle, const T& dx1, const T& dy1, const T& dz1, const T& dx2, const T& dy2, const T& dz2) {
			T c = cos(angle), s = sin(angle);
			T dx3 = dy1*dz2 - dy2*dz1;
			T dy3 = dz1*dx2 - dz2*dx1;
			T dz3 = dx1*dy2 - dx2*dy1;
			T dx1dx1 = dx1*dx1, dx1dy1 = dx1*dy1;
			T dy1dy1 = dy1*dy1, dy1dz1 = dy1*dz1;
			T dz1dz1 = dz1*dz1, dz1dx1 = dz1*dx1;
			T dx3dx3 = dx3*dx3, dx3dy3 = dx3*dy3;
			T dy3dy3 = dy3*dy3, dy3dz3 = dy3*dz3;
			T dz3dz3 = dz3*dz3, dz3dx3 = dz3*dx3;
			T dsx = dx1*s + dx2; 
			T dsy = dy1*s + dy2;
			T dsz = dz1*s + dz2;
			_0 = vec4<T>(dx1dx1*c + dx2*dsx + dx3dx3,	dx1dy1*c + dy2*dsx + dx3dy3,	dz1dx1*c + dz2*dsx + dz3dx3,	0);
			_1 = vec4<T>(dx1dy1*c + dx2*dsy + dx3dy3,	dy1dy1*c + dy2*dsy + dy3dy3,	dy1dz1*c + dz2*dsy + dy3dz3,	0);
			_2 = vec4<T>(dz1dx1*c + dx2*dsz + dz3dx3,	dy1dz1*c + dy2*dsz + dy3dz3,	dz1dz1*c + dz2*dsz + dz3dz3,	0);
			_3 = vec4<T>(0,								0,								0,								1);
		}
		explicit mat4(const T& angle, const vec3<T>& a1, const vec3<T>& a2) {
			T c = cos(angle), s = sin(angle);
			vec3<T> a3 = cross(a1,a2);
			T dx1dx1 = a1.x*a1.x, dx1dy1 = a1.x*a1.y;
			T dy1dy1 = a1.y*a1.y, dy1dz1 = a1.y*a1.z;
			T dz1dz1 = a1.z*a1.z, dz1dx1 = a1.z*a1.x;
			T dx3dx3 = a3.x*a3.x, dx3dy3 = a3.x*a3.y;
			T dy3dy3 = a3.y*a3.y, dy3dz3 = a3.y*a3.z;
			T dz3dz3 = a3.z*a3.z, dz3dx3 = a3.z*a3.x;
			vec3<T> ds = a1*s + a2; 
			_0 = vec4<T>(dx1dx1*c+a2[0]*ds[0]+dx3dx3, dx1dy1*c+a2[1]*ds[0]+dx3dy3, dz1dx1*c+a2[2]*ds[0]+dz3dx3,	0);
			_1 = vec4<T>(dx1dy1*c+a2[1]*ds[1]+dx3dy3, dy1dy1*c+a2[1]*ds[1]+dy3dy3, dy1dz1*c+a2[2]*ds[1]+dy3dz3,	0);
			_2 = vec4<T>(dz1dx1*c+a2[2]*ds[2]+dz3dx3, dy1dz1*c+a2[1]*ds[2]+dy3dz3, dz1dz1*c+a2[2]*ds[2]+dz3dz3,	0);
			_3 = vec4<T>(0,							 0,							  0,							1);
		}
		explicit mat4(const vec3<T>& scale, const vec3<T>& shear, const rotation<T>& rotate, const vec3<T>& translate, const vec4<T>& perspective);
		explicit mat4(const quat<T>& q);
		explicit mat4(const rotation<T>& r) { *this = mat4(r.angle,r.axis); }
		mat4(const mat4<float>& m) : _0(m._0), _1(m._1), _2(m._2), _3(m._3) {}
		mat4(const mat4<double>& m) : _0(m._0), _1(m._1), _2(m._2), _3(m._3) {}
		// add yuza
		explicit mat4(const char* str, const mat4<T>& _v = mat4<T>(T(1)))
		{
			*this = _v;
			string_to_numeric_array<T,16>(str, *this);
		}
		std::string to_string(const char delim = ',') const
		{
			return array_to_string<T,16>(*this, delim);
		}
		// add yuza

		operator T* () { return _0; }
		operator const T* () const { return _0; }

#if defined(KFBX_SDK)
		operator KFbxMatrix() const
		{
			return KFbxMatrix(
				fbxDouble4(_0[0], _0[1], _0[2], _0[3]),
				fbxDouble4(_1[0], _1[1], _1[2], _1[3]),
				fbxDouble4(_2[0], _2[1], _2[2], _2[3]),
				fbxDouble4(_3[0], _3[1], _3[2], _3[3]));
		}
		operator KFbxXMatrix() const
		{
			KFbxXMatrix m;
			m[0][0] = _0[0];
			m[0][1] = _0[1];
			m[0][2] = _0[2];
			m[0][3] = _0[3];

			m[1][0] = _1[0];
			m[1][1] = _1[1];
			m[1][2] = _1[2];
			m[1][3] = _1[3];

			m[2][0] = _2[0];
			m[2][1] = _2[1];
			m[2][2] = _2[2];
			m[2][3] = _2[3];

			m[3][0] = _3[0];
			m[3][1] = _3[1];
			m[3][2] = _3[2];
			m[3][3] = _3[3];

			return m;
			//return KFbxXMatrix(
			//	fbxDouble4(_0[0], _0[1], _0[2], _0[3]),
			//	fbxDouble4(_1[0], _1[1], _1[2], _1[3]),
			//	fbxDouble4(_2[0], _2[1], _2[2], _2[3]),
			//	fbxDouble4(_3[0], _3[1], _3[2], _3[3]));
		}
#endif // defined(KFBX_SDK)

		vec4<T>& operator [] (int i) { return (&_0)[i]; }
		const vec4<T>& operator[] (int i) const { return (&_0)[i]; }

		bool operator== (const mat4& m) const { return _0 == m._0 && _1 == m._1 && _2 == m._2 && _3 == m._3; }
		bool operator!= (const mat4& m) const { return !(*this == m); }
		mat4& operator= (const T& f) {
			_0=vec4<T>(f,0,0,0);
			_1=vec4<T>(0,f,0,0);
			_2=vec4<T>(0,0,f,0);
			_3=vec4<T>(0,0,0,f);
			return *this;
		}
		mat4& operator= (const T v[4][4]) {
			_0=vec4<T>(v[0]);
			_1=vec4<T>(v[1]);
			_2=vec4<T>(v[2]);
			_3=vec4<T>(v[3]);
			return *this;
		}
		mat4& operator+= (const T& f) {
			_0+=vec4<T>(f,0,0,0);
			_1+=vec4<T>(0,f,0,0);
			_2+=vec4<T>(0,0,f,0);
			_3+=vec4<T>(0,0,0,f);
			return *this;
		}
		mat4& operator-= (const T& f) {
			_0 -= vec4<T>(f,0,0,0);
			_1 -= vec4<T>(0,f,0,0);
			_2 -= vec4<T>(0,0,f,0);
			_3 -= vec4<T>(0,0,0,f);
			return *this;
		}
		mat4& operator*= (const T& f) {
			_0 *= f;
			_1 *= f;
			_2 *= f;
			_3 *= f;
			return *this;
		}
		mat4& operator/= (const T& f) {
			T i = 1/f;
			_0 *= i;
			_1 *= i;
			_2 *= i;
			_3 *= i;
			return *this;
		}
		mat4& operator+= (const mat4& m) { _0+= m._0; _1+=m._1; _2+=m._2; _3+=m._3; return *this; }
		mat4& operator-= (const mat4& m) { _0-= m._0; _1-=m._1; _2-=m._2; _3-=m._3; return *this; }
		mat4& operator*= (const mat4& m);
		mat4& operator/= (const mat4& m);
		mat4 operator- () const { return mat4<T>(-_0,-_1,-_2,-_3); }
		mat4 operator* (const T& f) const { return mat4<T>(_0*f, _1*f, _2*f, _3*f); }
		mat4 operator/ (const T& f) const { T i = 1/f; return (*this)*i; }
		mat4 operator+ (const mat4& m) const { return mat4<T>(_0+m._0, _1+m._1, _2+m._2, _3+m._3); }
		mat4 operator- (const mat4& m) const { return mat4<T>(_0-m._0, _1-m._1, _2-m._2, _3-m._3); }
		mat4 operator* (const mat4& m) const;
		mat4 operator/ (const mat4& m) const;
		bool unmatrix(vec3<T>& t, rotation<T>& r, vec3<T>& s) const;
		bool unmatrix(vec3<T>& scale, vec3<T>& shear, rotation<T>& rot, vec3<T>& translate, vec4<T>& pers) const;

		void identity() { (*this) = mat4<T>(1); }
		void load_identity() { (*this) = T(1); }
		
		mat4<T> inverse() const
		{
#if 1
			mat4<T> result;
			inverse_matrix<T,4>(*this, result);
			return result;
#else
			mat4<T> a(*this);
			mat4<T> b = T(1);
			int i, j, i1;
			for (j = 0; j < 4; j++)
			{
				i1 = j;
				for (i = j+1; i < 4; i++)
					if (fabs(a[i][j]) > fabs(a[i1][j]))
						i1 = i;
				std::swap(a[i1], a[j]);
				std::swap(b[i1], b[j]);
				b[j] /= a[j][j];
				a[j] /= a[j][j];
				for (i = 0; i < 4; i++)
				{
					if (i != j)
					{
						b[i] -= b[j]*a[i][j];
						a[i] -= a[j]*a[i][j];
					}
				}
			}
			return b;
#endif
		}
		T determinant() const {
			return	_0[0]*det3x3(_1[1],_1[2],_1[3],
								 _2[1],_2[2],_2[3],
								 _3[1],_3[2],_3[3]) -
					_0[1]*det3x3(_1[0],_1[2],_1[3],
		   						 _2[0],_2[2],_2[3],
		   						 _3[0],_3[2],_3[3]) +
					_0[2]*det3x3(_1[0],_1[1],_1[3],
		   						 _2[0],_2[1],_2[3],
		   						 _3[0],_3[1],_3[3]) -
					_0[3]*det3x3(_1[0],_1[1],_1[2],
								 _2[0],_2[1],_2[2],
		   						 _3[0],_3[1],_3[2]);
		}
		mat4 transpose()
		{
			return	mat4<T>(vec4<T>(_0[0], _1[0], _2[0], _3[0]),
							vec4<T>(_0[1], _1[1], _2[1], _3[1]),
							vec4<T>(_0[2], _1[2], _2[2], _3[2]),
							vec4<T>(_0[3], _1[3], _3[3], _3[3]) );
		}
		mat4 inverse_simple()
		{
			T X = -(_0.x*_3.x+_0.y*_3.y+_0.z*_3.z);
			T Y = -(_1.x*_3.x+_1.y*_3.y+_1.z*_3.z);
			T Z = -(_2.x*_3.x+_2.y*_3.y+_2.z*_3.z);
			return mat4<T>(	vec4<T>(_0[0], _1[0], _2[0], T(0)),
							vec4<T>(_0[1], _1[1], _2[1], T(0)),
							vec4<T>(_0[2], _1[2], _2[2], T(0)),
							vec4<T>(    X,	   Y,	  Z, T(1)));
		}
		static mat4 translate(const T& tx, const T& ty, const T& tz) { return mat4<T>(vec3<T>(tx,ty,tz)); }
		static mat4 rotate(const T& angle, const vec3<T>& axis) { return mat4<T>(angle,axis); }
		static mat4 scale(const T& sx, const T& sy, const T& sz) { return mat4<T>(sx,sy,sz); }
		static mat4 skew(const T& angle, const vec3<T>& a1, const vec3<T>& a2) { return mat4<T>(angle,a1,a2); }
		void transform_points(int npoints, const vec4<T> src[], vec4<T> dst[]) const
		{
			float m00 = _0[0], m01 = _0[1], m02 = _0[2], m03 = _0[3];
			float m10 = _1[0], m11 = _1[1], m12 = _1[2], m13 = _1[3];
			float m20 = _2[0], m21 = _2[1], m22 = _2[2], m23 = _2[3];
			float m30 = _3[0], m31 = _3[1], m32 = _3[2], m33 = _3[3];
			for (int i = 0; i < npoints; ++i)
			{
				T x = src[i].x, y = src[i].y, z = src[i].z, w = src[i].w;
				dst[i].x = x*m00 + y*m10 + z*m20 + w*m30;
				dst[i].y = x*m01 + y*m11 + z*m21 + w*m31;
				dst[i].z = x*m02 + y*m12 + z*m22 + w*m32;
				dst[i].w = x*m03 + y*m13 + z*m23 + w*m33;
			}
		}
	};

	template <class T>
	inline vec4<T> operator* (const vec4<T>& v, const mat4<T>& m)
	{
		T x = v.x, y = v.y, z = v.z, w = v.w;
		return vec4<T>(	x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0],
						x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1],
						x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2],
						x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3]);
	}
	template <class T>
	inline vec4<T> operator* (const mat4<T>& m, const vec4<T>& v)
	{
		T x = v.x, y = v.y, z = v.z, w = v.w;
		return vec4<T>(	m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3]*w,
						m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3]*w,
						m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3]*w,
						m[3][0]*x + m[3][1]*y + m[3][2]*z + m[3][3]*w);
	}

	template <class T> inline vec3<T> vec3<T>::transform_direction(const mat4<T>& m)
	{
		T lx = this->x, ly = this->y, lz = this->z;
		return vec3<T>(	lx*m[0][0] + ly*m[1][0] + lz*m[2][0],
						lx*m[0][1] + ly*m[1][1] + lz*m[2][1],
						lx*m[0][2] + ly*m[1][2] + lz*m[2][2]);
	}

	template <class T> inline vec3<T> transform_direction(const vec3<T>& v, const mat4<T>& m)
	{
		T lx = v.x, ly = v.y, lz = v.z;
		return vec3<T>(	lx*m[0][0] + ly*m[1][0] + lz*m[2][0],
						lx*m[0][1] + ly*m[1][1] + lz*m[2][1],
						lx*m[0][2] + ly*m[1][2] + lz*m[2][2]);
	}

	// mat4f im: 変換したいマトリックスの逆マトリックスである必要があります。
	// vec3f v: 変換したいベクトル
	template <class T> inline vec3<T> transform_normal(const mat4<T>& im, const vec3<T>& v)
	{
		T lx = v.x, ly = v.y, lz = v.z;
		return vec3<T>(	im[0][0]*lx + im[0][1]*ly + im[0][2]*lz,
						im[1][0]*lx + im[1][1]*ly + im[1][2]*lz,
						im[2][0]*lx + im[2][1]*ly + im[2][2]*lz);
	}

	template <class T>
	inline vec3<T> operator* (const vec3<T>& v, const mat4<T>& m) {
			vec3<T> r;
			T lx = v.x, ly = v.y, lz = v.z;
			r.x = lx*m[0][0] + ly*m[1][0] + lz*m[2][0] + m[3][0];
			r.y = lx*m[0][1] + ly*m[1][1] + lz*m[2][1] + m[3][1];
			r.z = lx*m[0][2] + ly*m[1][2] + lz*m[2][2] + m[3][2];
			T w = lx*m[0][3] + ly*m[1][3] + lz*m[2][3] + m[3][3];
			if (w != T(1))
				r /= w;
			return r;
		}
	template <class T>
		mat4<T>& mat4<T>::operator*= (const mat4<T>& m) { _0 = _0*m; _1 =_1*m; _2 =_2*m; _3 =_3*m; return *this; }
	template <class T>
		mat4<T>& mat4<T>::operator/= (const mat4<T>& m) { mat4<T> i = m.inverse(); return (*this) *= i; }
	template <class T>
		mat4<T> mat4<T>::operator* (const mat4<T>& m) const { return mat4<T>(_0*m, _1*m, _2*m, _3*m); }
	template <class T>
		mat4<T> mat4<T>::operator/ (const mat4<T>& m) const { mat4<T> i = m.inverse(); return (*this)*i; }
	template <class T>
		mat4<T> operator* (const T& f, const mat4<T>& m) { return m*f; }
	template <class T>
		mat4<T> operator/ (const T& f, const mat4<T>& m) { mat4<T> i = m.inverse(); return f*m; }

	template <class T>
		inline std::ostream& operator<< (std::ostream& os, mat4<T>& m) {
			os<<m._0<<m._1<<m._2<<m._3;
			return os;
		}
	template <class T>
		inline std::istream& operator>> (std::istream& is, mat4<T>& m) {
			is>>m._0>>m._1>>m._2>>m._3;
			return is;
		}

	template <class T>
	class ray3 {
	public:
		vec3<T> origin;
		vec3<T> direction;
		T tnear, tfar;
		ray3() {}
		ray3(const vec3<T>& o, const vec3<T>& d, const T tmin = epsiron(), const T tmax = infinity())
			: origin(o), direction(d), tnear(tmin), tfar(tmax) {}
		vec3<T> operator() (const T& t) const { return origin + direction*t; }
	};

	template <class T>
	class bbox1
	{
	public:
		T min_, max_;
	public:
		bbox1() : min_(infinity()), max_(-infinity()) {}
		bbox1(const T& p) : min_(p), max_(p) {}
		bbox1(const T& __min, const T& __max) : min_(__min), max_(__max) {}
		~bbox1() {}
		const T& vmin() const { return min_; }
		const T& vmax() const { return max_; }
		T& vmin() { return min_; }
		T& vmax() { return max_; }
		void clear()
		{
			min_ = infinity();
			max_ = -infinity();
		}
		T delta() const { return max_-min_; }
		T center() const { return (max_+min_)*0.5f; }
		bool empty() const
		{
			return min_ == infinity() && max_ == -infinity();
		}
		operator T*() { return min_; }
		operator const T* () const { return min_; }
		bbox1<T>& operator= (const T& point) { min_ = point, max_ = point; return *this; }
		bbox1<T>& operator|= (const T& p)
		{
			min_ = MIN(min_,p);
			max_ = MAX(max_,p);
			return *this;
		}
		bbox1<T>& operator|= (const bbox1<T>& b)
		{
			*this |= b.min_;
			*this |= b.max_;
			return *this;
		}
		friend bbox1<T> operator | (const bbox1<T>& a, const bbox1<T>& b)
		{
			bbox1<T> x = a;
			x |= b;
			return x;
		}
		friend bool operator == (const bbox1<T>& a, const bbox1<T>& b) { return a.min_ == b.min_ && a.max_ == b.max_; }
		friend bool operator != (const bbox1<T>& a, const bbox1<T>& b) { return !(a==b); }
		bool operator<  (const bbox1<T>& b) const
		{
			if (min_ < b.min_) {
				return true;
			}
			if (min_ == b.min_) {
				return max_ < b.max_;
			}
			return false;
		}
		bool operator>  (const bbox1<T>& b) const
		{
			if (min_ > b.min_) {
				return true;
			}
			if (min_ == b.min_) {
				return max_ > b.max_;
			}
			return false;
		}
		bool include(const T& pt) const
		{
			return	min_ <= pt && pt <= max_;
		}
		bool include2(const T& pt) const
		{
			return	min_ < pt && pt < max_;
		}
		bool intersection(const bbox1<T>& b) const
		{
			if (max_ < b.min_ || b.max_ < min_)
				return false;
			return true;
		}
		bool intersection2(const bbox1<T>& b) const
		{
			if (max_ <= b.min_ || b.max_ <= min_)
				return false;
			return true;
		}
		bbox1<T> inflate(const T& d) const
		{
			return bbox1<T>(min_ - d, max_ + d);
		}
	};

	template <class T>
	class bbox2
	{
	public:
		vec2<T> min_, max_;
	public:
		bbox2() : min_(infinity()), max_(-infinity()) {}
		bbox2(const vec2<T>& p) : min_(p), max_(p) {}
		bbox2(const vec2<T>& __min, const vec2<T>& __max) : min_(__min), max_(__max) {}
		bbox2(const T& xmin, const T& ymin, const T& xmax, const T& ymax) : min_(vec2<T>(xmin,ymin)), max_(vec2<T>(xmax,ymax)) {}
		~bbox2() {}
		const vec2<T>& vmin() const { return min_; }
		const vec2<T>& vmax() const { return max_; }
		vec2<T>& vmin() { return min_; }
		vec2<T>& vmax() { return max_; }
		void clear()
		{
			min_ = vec2<T>(T(infinity()),T(infinity()));
			max_ = vec2<T>(T(-infinity()),T(-infinity()));
		}
		vec2<T> delta() const { return max_-min_; }
		vec2<T> center() const { return (max_+min_)*0.5f; }
		bool empty() const
		{
			return min_ == infinity() && max_ == -infinity();
		}
		operator T*() { return min_; }
		operator const T* () const { return min_; }
		bbox2<T>& operator= (const vec2<T>& point) { min_ = point, max_ = point; return *this; }
		bbox2<T>& operator|= (const vec2<T>& p)
		{
			min_ = vec2<T>(MIN(min_[0],p[0]),MIN(min_[1],p[1]));
			max_ = vec2<T>(MAX(max_[0],p[0]),MAX(max_[1],p[1]));
			return *this;
		}
		bbox2<T>& operator|= (const bbox2<T>& b)
		{
			min_ = vec2<T>(MIN(min_[0],b.min_[0]),MIN(min_[1],b.min_[1]));
			max_ = vec2<T>(MAX(max_[0],b.max_[0]),MAX(max_[1],b.max_[1]));
			return *this;
		}
		friend	bbox2<T> operator | (const bbox2<T>& a, const bbox2<T>& b)
		{
			return bbox2<T>(vec2<T>(MIN(a.min_[0],b.min_[0]),MIN(a.min_[1],b.min_[1])),
							vec2<T>(MAX(a.max_[0],b.max_[0]),MAX(a.max_[1],b.max_[1])));
		}
		friend bool operator == (const bbox2<T>& a, const bbox2<T>& b) { return a.min_ == b.min_ && a.max_ == b.max_; }
		friend bool operator != (const bbox2<T>& a, const bbox2<T>& b) { return !(a==b); }
		bbox2<T> operator * (const mat3<T>& m) const
		{
			bbox2<T> b;
			b |= vec2<T>(min_.x,min_.y)*m;
			b |= vec2<T>(max_.x,min_.y)*m;
			b |= vec2<T>(max_.x,max_.y)*m;
			b |= vec2<T>(min_.x,max_.y)*m;
			return b;
		}
		bool include(const vec2<T>& pt) const
		{
			return	min_.x <= pt.x && pt.x <= max_.x &&
					min_.y <= pt.y && pt.y <= max_.y;
		}
		bool intersection(const bbox2<T>& b) const
		{
			if (max_.x < b.min_.x || b.max_.x < min_.x)
				return false;
			if (max_.y < b.min_.y || b.max_.y < min_.y)
				return false;
			return true;
		}
	};

	template <class T>
	struct bbox3
	{
		vec3<T> min_, max_;
		bbox3() : min_(infinity()), max_(-infinity()) {}
		explicit bbox3(const vec3<T>& p) : min_(p), max_(p) {}
		explicit bbox3(const vec3<T>& __min, const vec3<T>& __max) : min_(__min), max_(__max) {}
		explicit bbox3(const T& xmin, const T& ymin, const T& zmin, const T& xmax, const T& ymax, const T& zmax) : min_(vec3<T>(xmin,ymin,zmin)), max_(vec3<T>(xmax,ymax,zmax)) {}
		explicit bbox3(const bbox2<T>& b, const T& zmin, const T& zmax) : min_(b.vmin().x,b.vmin().y,zmin), max_(b.vmax().x,b.vmax().y,zmax) {}
		explicit bbox3(const T* v) : min_(v[0],v[2],v[4]), max_(v[1],v[3],v[5]) {}
		explicit bbox3(int nverts, vec3<T> verts[])
			: min_(verts[0])
			, max_(verts[0])
		{
			for (int i = 1; i < nverts; ++i)
			{
				min_ = vec3<T>(MIN(min_.x,verts[i].x),MIN(min_.y,verts[i].y),MIN(min_.z,verts[i].z));
				max_ = vec3<T>(MAX(max_.x,verts[i].x),MAX(max_.y,verts[i].y),MAX(max_.z,verts[i].z));
			}
		}
		~bbox3() {}
		const vec3<T>& vmin() const { return min_; }
		const vec3<T>& vmax() const { return max_; }
		vec3<T>& vmin() { return min_; }
		vec3<T>& vmax() { return max_; }

		const vec3<T>& Min() const { return min_; }
		const vec3<T>& Max() const { return max_; }
		vec3<T>& Min() { return min_; }
		vec3<T>& Max() { return max_; }

		operator T*() { return min_; }
		operator const T* () const { return min_; }
		void clear() {
			min_ = vec3<T>(infinity());
			max_ = vec3<T>(-infinity());
		}
		bool empty() const
		{
			return min_ == infinity() && max_ == -infinity();
		}
		bool include(const vec3<T>& pt) const
		{
			return	min_.x <= pt.x && pt.x <= max_.x &&
					min_.y <= pt.y && pt.y <= max_.y &&
					min_.z <= pt.z && pt.z <= max_.z;
		}
		vec3<T> delta() const { return max_-min_; }
		vec3<T> center() const
		{
			return vec3<T>(	(min_.x+max_.x)*0.5f,
							(min_.y+max_.y)*0.5f,
							(min_.z+max_.z)*0.5f);
		}
		bbox3<T>& operator = (const vec3<T>& p) { min_ = p, max_ = p; return *this; }
		bbox3<T>& operator += (const vec3<T>& d) { min_ -= d; max_ += d; return *this; }
		bbox3<T>& operator |= (const vec3<T>& p)
		{
			min_ = vec3<T>(MIN(min_.x,p.x),MIN(min_.y,p.y),MIN(min_.z,p.z));
			max_ = vec3<T>(MAX(max_.x,p.x),MAX(max_.y,p.y),MAX(max_.z,p.z));
			return *this;
		}
		bbox3<T>& operator |= (const bbox3<T>& b)
		{
			min_ = vec3<T>(MIN(min_.x,b.min_.x),MIN(min_.y,b.min_.y),MIN(min_.z,b.min_.z));
			max_ = vec3<T>(MAX(max_.x,b.max_.x),MAX(max_.y,b.max_.y),MAX(max_.z,b.max_.z));
			return *this;
		}
		bbox3<T> operator * (const mat4<T>& m) const
		{
			vec3<T> p0(vec3<T>(min_.x,min_.y,min_.z)*m);
			vec3<T> p1(vec3<T>(max_.x,min_.y,min_.z)*m);
			vec3<T> p2(vec3<T>(max_.x,max_.y,min_.z)*m);
			vec3<T> p3(vec3<T>(min_.x,max_.y,min_.z)*m);
			vec3<T> p4(vec3<T>(min_.x,min_.y,max_.z)*m);
			vec3<T> p5(vec3<T>(max_.x,min_.y,max_.z)*m);
			vec3<T> p6(vec3<T>(max_.x,max_.y,max_.z)*m);
			vec3<T> p7(vec3<T>(min_.x,max_.y,max_.z)*m);
			return bbox3<T>(
				MIN(MIN(MIN(p0.x,p1.x),MIN(p2.x,p3.x)),MIN(MIN(p4.x,p5.x),MIN(p6.x,p7.x))),
				MIN(MIN(MIN(p0.y,p1.y),MIN(p2.y,p3.y)),MIN(MIN(p4.y,p5.y),MIN(p6.y,p7.y))),
				MIN(MIN(MIN(p0.z,p1.z),MIN(p2.z,p3.z)),MIN(MIN(p4.z,p5.z),MIN(p6.z,p7.z))),
				MAX(MAX(MAX(p0.x,p1.x),MAX(p2.x,p3.x)),MAX(MAX(p4.x,p5.x),MAX(p6.x,p7.x))),
				MAX(MAX(MAX(p0.y,p1.y),MAX(p2.y,p3.y)),MAX(MAX(p4.y,p5.y),MAX(p6.y,p7.y))),
				MAX(MAX(MAX(p0.z,p1.z),MAX(p2.z,p3.z)),MAX(MAX(p4.z,p5.z),MAX(p6.z,p7.z))));
		}
		bool intersection(const bbox3<T>& b) const
		{
			if (max_.x < b.min_.x || b.max_.x < min_.x)
				return false;
			if (max_.y < b.min_.y || b.max_.y < min_.y)
				return false;
			if (max_.z < b.min_.z || b.max_.z < min_.z)
				return false;
			return true;
		}

		bool intersection(const vec3<T>& start, const vec3<T>& dir, T* t_min, T* t_max) const
		{
			T mind, maxd, inv;
			for (int i = 0; i < 3; ++i)
			{
				if (fabs(dir[i]) > epsiron()) {
					inv = T(1)/dir[i];
					mind = (min_[i]-start[i])*inv;
					maxd = (max_[i]-start[i])*inv;
					if (mind > maxd)
						std::swap(mind,maxd);
					*t_min = max(*t_min,mind);
					*t_max = min(*t_max,maxd);
					if (*t_min > *t_max)
						return false;
				}
				else
				{
					if ( start[i] < min_[i] || max_[i] < start[i] )
						return false;
				}
			}
			return true;
		}

		bool intersection(ray3<T>& ray) const {
			T tnear = ray.tnear, tfar = ray.tfar;
			T inv, tmin, tmax;
			if (fabs(ray.direction[0]) > T(epsiron())) {
				inv = 1/ray.direction[0];
				tmin = (min_[0]-ray.origin[0])*inv;
				tmax = (max_[0]-ray.origin[0])*inv;
				if (tmin > tmax)
					std::swap(tmin,tmax);
				tnear = CG::MAX_T(tmin,tnear);
				tfar = CG::MIN_T(tmax,tfar);
				if (tnear > tfar)
					return false;
			}
			else {
				if (ray.origin[0] < min_[0] || max_[0] < ray.origin[0])
					return false;
			}
			if (fabs(ray.direction[1]) > T(epsiron())) {
				inv = 1/ray.direction[1];
				tmin = (min_[1]-ray.origin[1])*inv;
				tmax = (max_[1]-ray.origin[1])*inv;
				if (tmin > tmax)
					std::swap(tmin,tmax);
				tnear = CG::MAX_T(tmin,tnear);
				tfar = CG::MIN_T(tmax,tfar);
				if (tnear > tfar)
					return false;
			}
			else {
				if (ray.origin[1] < min_[1] || max_[1] < ray.origin[1])
					return false;
			}
			if (fabs(ray.direction[2]) > T(epsiron())) {
				inv = 1/ray.direction[2];
				tmin = (min_[2]-ray.origin[2])*inv;
				tmax = (max_[2]-ray.origin[2])*inv;
				if (tmin > tmax)
					std::swap(tmin,tmax);
				tnear = CG::MAX_T(tmin,tnear);
				tfar = CG::MIN_T(tmax,tfar);
				if (tnear > tfar)
					return false;
			}
			else {
				if (ray.origin[2] < min_[2] || max_[2] < ray.origin[2])
					return false;
			}
			ray.tnear = tnear;
			ray.tfar = tfar;
			return true;
		}
		friend	bbox3<T> operator | (const bbox3<T>& a, const bbox3<T>& b) {
			return bbox3<T>(vec3<T>(MIN(a.min_[0],b.min_[0]),MIN(a.min_[1],b.min_[1]),MIN(a.min_[2],b.min_[2])),
							vec3<T>(MAX(a.max_[0],b.max_[0]),MAX(a.max_[1],b.max_[1]),MAX(a.max_[2],b.max_[2])));
		}
	};

	template <class T>
		inline T ptlined(const vec3<T>& q, const vec3<T>& p0, const vec3<T>& p1) {
			vec3<T> p1p0 = p1 - p0;
			vec3<T> qp0 = q - p0;
			T a = p1p0.length2();
			T b = dot(p1p0,qp0);
			vec3<T> v = p0 - q + p1p0*(a/b);
			return v.length();
		}
	template <class T>
		inline vec3<T> rotate(const vec3<T>& Q, const T& angle, const vec3<T>& P0, const vec3<T>& P1) {
			vec3<T> v = P1 - P0;
			return Q*mat4<T>(angle,v);
		}
	template <class T>
		inline vec3<T> faceforward(const vec3<T>& N, const vec3<T>& I, const vec3<T>& Nref) {
			return sign((-I).dot(Nref))*N;
		}
	template <class T>
		inline vec3<T> reflect(const vec3<T>& I, const vec3<T>& N) {
			return I - T(2)*I.dot(N)*N;
		}
	template <class T>
		inline vec3<T> refract(const vec3<T>& I, const vec3<T>& N, const T& eta) {
			T IdotN = I.dot(N);
			T k = T(1) - eta*eta*(T(1) - IdotN*IdotN);
			return k < T(0) ? vec3<T>(T(0)) : I*eta - N*(eta*IdotN + sqrt(k));
		}
	template <class T>
		inline bool fresnel(const vec3<T>& I, const vec3<T>& N, const T& eta, T& Kr, T& Kt)
		{
			T IdotN = I.dot(N);
			T k = T(1) - eta*eta*(T(1)-IdotN*IdotN);
			if (k < T(0))
			{
				Kr = T(1);
				Kt = T(0);
				return false;
			}
			k = sqrt(k);
			T a = k + eta*IdotN;
			T t = -(eta*k + IdotN) / (eta*k - IdotN);
			T s = a / (k - eta*IdotN);
			Kr = T(0.5)*(s*s + t*t);
			Kt = T(1) - Kr;
			return true;
		}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class T>
		inline bool fresnel(const vec3<T>& I, const vec3<T>& N, const T& eta, T& Kr, T& Kt, vec3<T>& RR, vec3<T>& TT) {
			T IdotN = I.dot(N);
			T k = T(1) - eta*eta*(T(1) - IdotN*IdotN);
			if (k >= T(0))
			{
				k = sqrt(k);
				T a = k + eta*IdotN;
				TT = eta*I - a*N;
				RR = I - T(2)*IdotN*N;
				T t = -(eta*k + IdotN) / (eta*k - IdotN);
				T s = a / (k - eta*IdotN);
				Kr = T(0.5)*(s*s + t*t);
				Kt = T(1) - Kr;
				return true;
			}
			RR = I - (T(2)*IdotN)*N;
			Kr = T(1);
			Kt = T(0);
			return false;
		}

//////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	struct quat
	{
		T		w;
		vec3<T> v;

		quat(const T& _w = T(1)) : w(_w), v(T(0)) {}
		explicit quat(const T& _w, const T& _x, const T& _y, const T& _z) : w(_w), v(_x,_y,_z) {}
		explicit quat(const T& _w, const vec3<T>& _v) : w(_w), v(_v) {}
		explicit quat(const rotation<T>& rot);
		explicit quat(const mat3<T>& m);
		explicit quat(const mat4<T>& m);
		explicit quat(const vec3<T>& dir, const vec3<T>& up);
		T length2() const { return w*w+v.length2(); }
		T length() const { return sqrt(length2()); }
		T norm2() const { return length2(); }
		T norm() const { return length(); }
		quat normalize() const { T d = this->length(); return d > T(0) ? *this/d : *this; }
		quat conjugate() const { return quat(w,-v); }
		quat inverse() const { T n2 = norm2(); return this->conjugate()/n2; }
		quat operator+ (const quat<T>& q) const { return quat(w+q.w,v+q.v); }
		quat operator- (const quat<T>& q) const { return quat(w-q.w,v-q.v); }
		quat operator* (const quat<T>& q) const { return quat(w*q.w-v.dot(q.v),w*q.v+v*q.w+v.cross(q.v)).normalize(); }
		quat operator/ (const quat<T>& q) const { return *this*q.inverse(); }
		quat operator* (const T& f) const { return quat(w*f,v*f); }
		quat operator/ (const T& f) const { T inv = T(1)/f; return *this*inv; }
		quat operator- () const { return quat(-w,-v); }
		quat& operator+=(const quat<T>& q) { w+=q.w, v+=q.v; return *this; }
		quat& operator-=(const quat<T>& q) { w-=q.w, v-=q.v; return *this; }
		quat& operator*=(const quat<T>& q) { *this = *this*q; return *this; }
		quat& operator/=(const quat<T>& q) { *this = *this/q; return *this; }
		quat& operator= (const T& f) { w = f, v = T(0); return *this; }
		quat& operator*=(const T& f) { *this = *this*f; return *this; }
		quat& operator/=(const T& f) { *this = *this/f; return *this; }

		vec3<T> transform(const vec3<T>& _v)
		{
			quat q = (*this)*quat(T(0),_v)*quat(w,-v);
			return q.v;
		}
		T dot(const quat<T>& q) const { return w*q.w+v.dot(q.v); }
	};

	template <class T> inline quat<T>::quat(const rotation<T>& rot)
	{
		vec3<T> normal = rot.axis.normalize();
		T ha = rot.angle*T(0.5);
		this->w = cos(ha);
		this->v = normal*sin(ha);
	}

	template <class T> inline quat<T>::quat(const mat3<T>& m)
	{
		T diagonal, s;
		diagonal = m[0][0] + m[1][1] + m[2][2];
		// check the diagonal
		if (diagonal > T(0))
		{
			s = sqrt(diagonal + T(1));
			this->w = s*T(0.5);
			s = 0.5f/s;
			this->v.x = (m[1][2] - m[2][1]) * s;
			this->v.y = (m[2][0] - m[0][2]) * s;
			this->v.z = (m[0][1] - m[1][0]) * s;
		}
		else
		{
			//size_t i, j, k;
			//static const size_t next[3] = { 1, 2, 0 };
			int i, j, k;
			static const int next[3] = { 1, 2, 0 };
			// diagonal is negative
			i = 0;
			if (m[1][1] > m[0][0]) { i = 1; }
			if (m[2][2] > m[i][i]) { i = 2; }
			j = next[i];
			k = next[j];
			s = T(sqrt((m[i][i] - (m[j][j] + m[k][k])) + T(1)));
			this->v[i] = s*T(0.5);

			s = T(0.5)/s;
			this->w		= (m[j][k] - m[k][j])*s;
			this->v[j]	= (m[i][j] + m[j][i])*s;
			this->v[k]	= (m[i][k] + m[k][i])*s;
		}
	}

	template <class T> inline quat<T>::quat(const mat4<T>& m)
	{
		T diagonal, s;
		diagonal = m[0][0] + m[1][1] + m[2][2];
		// check the diagonal
		if (diagonal > T(0))
		{
			s = sqrt(diagonal + T(1));
			this->w = s*T(0.5);
			s = 0.5f/s;
			this->v.x = (m[1][2] - m[2][1]) * s;
			this->v.y = (m[2][0] - m[0][2]) * s;
			this->v.z = (m[0][1] - m[1][0]) * s;
		}
		else
		{
			int i, j, k;
			static const size_t next[3] = { 1, 2, 0 };
			// diagonal is negative
			i = 0;
			if (m[1][1] > m[0][0]) { i = 1; }
			if (m[2][2] > m[i][i]) { i = 2; }
			j = next[i];
			k = next[j];
			s = T(sqrt((m[i][i] - (m[j][j] + m[k][k])) + T(1)));
			this->v[i] = s*T(0.5);
			s = T(0.5)/s;
			this->w		= (m[j][k] - m[k][j])*s;
			this->v[j]	= (m[i][j] + m[j][i])*s;
			this->v[k]	= (m[i][k] + m[k][i])*s;
		}
	}

	template <class T> inline quat<T>::quat(const vec3<T>& dir, const vec3<T>& up)
	{
		vec3<T> z = dir;
		CG::normalize(z);
		vec3<T> x, y;
		x = up.cross(z);
		y = z.cross(x);
		CG::normalize(x);
		CG::normalize(y);
		mat3<T> mat(x,y,z);
		*this = quat<T>(mat);
	}

	template <class T> inline quat<T> slerp(const quat<T>& a, const quat<T>& b, const T& t)
	{
		const quat<T>& p = a; // just an alias to match q
		// calc cosine theta
		T cosom = dot(a,b);

		// adjust signs (if necessary)
		quat<T> q;
		if ((cosom < T(0)))
		{
			cosom = -cosom;
			q = -b;
		}
		else
		{
			q = b;
		}

		// Calculate coefficients
		T scaleP, scaleQ;
		if ((T(1) - cosom) > (T)0.0001) // 0.0001 -> some epsillon
		{
			// Standard case (slerp)
			T omega, sinom;
			omega = acos( cosom ); // extract theta from dot product's cos theta
			sinom = sin( omega );
			scaleP  = sin((T(1) - t)*omega)/sinom;
			scaleQ  = sin(t*omega)/sinom;
		}
		else
		{
			// Very close, do linear interp (because it's faster)
			scaleP = T(1) - t;
			scaleQ = t;
		}
		//return scaleP*p + scaleQ*q;
		return p * scaleP + q * scaleQ;
	}

	template <class T>
	class plane3 {
	protected:
		vec3<T>	m_normal;
		T	m_distance;
	public:
		enum { kBehind = -1, kCoinciding = 0, kInfront = 1, kSpanning = 2};
		plane3() {}
		plane3(const vec3<T>& a, const vec3<T>& b, const vec3<T>& c)
		{
			vec3<T> ab = b-a, ac = c-a;
			m_normal = CG::cross(ab,ac);
			normalize(m_normal);
			m_distance = m_normal.dot(a);
		}
		plane3(const vec3<T>& point, const vec3<T>& normal)
			: m_normal(normal)
			, m_distance(point.dot(normal)) {}

		vec3<T>& normal() { return m_normal; }
		const vec3<T>& normal() const { return m_normal; }
		T& distance() { return m_distance; }
		T distance(const vec3<T>& pt) const { return m_normal.dot(pt)-m_distance; }
		long which_side(const vec3<T>& P) const
		{
			T result = distance(P);
			if (result > epsiron())
				return kInfront;
			else if (result < -epsiron())
				return kBehind;
			return kCoinciding;
		}
		long which_side(const std::vector< vec3<T> >& points) const
		{
			long numFront = 0, numBehind = 0;
			std::vector<vec3<T> >::const_iterator it;
			for (it = points.begin(); it != points.end(); ++it) {
				long side = which_side(*it);
				if (side > 0)
					++numFront;
				else if (side < 0)
					++numBehind;
			}
			if (numFront > 0 && numBehind == 0)
				return kInfront;
			else if (numFront == 0 && numBehind > 0)
				return kBehind;
			else if (numFront == 0 && numBehind == 0)
				return kCoinciding;
			return kSpanning;
		}
		T depth(const T& x, const T& y) const {
			return (m_distance-m_normal.x*x-m_normal.y*y)/m_normal.z;
		}
	};

	template<class T>
	int sign_int(const T& x)
	{
		if (x > T(0))
			return 1;
		else if (x < T(0))
			return -1;
		return 0;
	}

	template<class T>
	int lex_cmp_x_points(const T& x1, const T& y1, const T& w1,
						 const T& x2, const T& y2, const T& w2)
	{
		int s = sign_int(x1*w2-x2*w1);
		return s!=0 ? s : sign_int(y1*w2-y2*w1);
	}

	template<class T>
	int lex_cmp_y_points(const T& x1, const T& y1, const T& w1,
						 const T& x2, const T& y2, const T& w2)
	{
		int s = sign_int(y1*w2-y2*w1);
		return s!=0 ? s : sign_int(x1*w2-x2*w1);
	}

	template <class T> struct hpoint2;

	template <class T>
	int compare(const hpoint2<T>& a, const hpoint2<T>& b) { return lex_cmp_x_points(a.x,a.y,a.w,b.x,b.y,b.w); }

	template <class T>
	struct hpoint2
	{
		T x, y, w;
		hpoint2() {}
		explicit hpoint2(const T& _x, const T& _y, const T& _w = T(1)) : x(_x), y(_y), w(_w) {}
		explicit hpoint2(const vec3<float>& v) : x(T(v[0])), y(T(v[1])), w(T(1)) {}
		operator T* () { return &x; }
		operator const T* () const { return &x; }
		double xcoord() const { return double(x)/double(w); }
		double ycoord() const { return double(y)/double(w); }
		hpoint2 operator + (const T& f) const { return hpoint2(x+f,y+f,w+f); }
		hpoint2 operator - (const T& f) const { return hpoint2(x-f,y-f,w-f); }
		hpoint2 operator * (const T& f) const { return hpoint2(x*f,y*f,w*f); }
		hpoint2 operator + (const hpoint2& v) const { return hpoint2(x+v.x,y+v.y,w+v.w); }
		hpoint2 operator - (const hpoint2& v) const { return hpoint2(x-v.x,y-v.y,w-v.w); }
		hpoint2 operator * (const hpoint2& v) const { return hpoint2(x*v.x,y*v.y,w*v.w); }
		hpoint2 cross(const hpoint2<T>& b) const { return hpoint2<T>(y*b.w-w*b.y,w*b.x-x*b.w,x*b.y-y*b.x); }
		T dot(const hpoint2<T>& b) const { return x*b.x+y*b.y+w*b.w; }
		bool operator== (const hpoint2& b) const { return compare((*this),b) == 0; }
		bool operator!= (const hpoint2& b) const { return !((*this)==b); }
		friend T orientation(const hpoint2<T>& p, const hpoint2<T>& q, const hpoint2<T>& r) { return dot(cross(p,q),r); }
	};

	typedef vec2<float>		vec2f;
	typedef vec3<float>		vec3f;
	typedef vec4<float>		vec4f;
	// add yuza
	typedef mat2<float>		mat2f;
	// add yuza
	typedef mat3<float>		mat3f;
	typedef mat4<float>		mat4f;
	typedef ray3<float>		ray3f;
	typedef bbox1<float>	bbox1f;
	typedef bbox2<float>	bbox2f;
	typedef bbox3<float>	bbox3f;
	typedef plane3<float>	plane3f;

	typedef vec2<double>	vec2d;
	typedef vec3<double>	vec3d;
	typedef vec4<double>	vec4d;
	// add yuza
	typedef mat2<double>	mat2d;
	// add yuza
	typedef mat3<double>	mat3d;
	typedef mat4<double>	mat4d;
	typedef ray3<double>	ray3d;
	typedef bbox1<double>	bbox1d;
	typedef bbox2<double>	bbox2d;
	typedef bbox3<double>	bbox3d;
	typedef plane3<double>	plane3d;

	typedef rotation<float>		rotationf;
	typedef rotation<double>	rotationd;
	typedef quat<float>			quatf;
	typedef quat<double>		quatd;


	template<class T> inline T lerp(const T& a, const T& b, float t) { return a + (b-a)*t; }
	template<class T> inline T lerp(const T& a, const T& b, double t) { return a + (b-a)*t; }
	template<class T> inline T lerpv(const T& a, const T& b, const T& t) { return a + (b-a)*t; }
	template <class T> inline T bilinear(float s, float t, const T& a0, const T& a1, const T& a2, const T& a3)
	{
		return CG::lerp(CG::lerp(a0,a1,s),CG::lerp(a2,a3,s),t);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	template <class T>
		inline T area(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2) { return (v1-v0).cross(v2-v1).length()*T(0.5); }
	template <class T>
		inline T get_angle(const vec3<T>& c, const vec3<T>& r, const vec3<T>& l)
	{
		vec3<T> v1 = r - c;
		vec3<T> v2 = l - c;		
		T l1 = v1.length();
		T l2 = v2.length();
		T v1dotv2 = v1.dot(v2);
		T a = clamp(v1dotv2/(l1*l2),T(-1),T(1));
		if((v1.x*v2.y - v1.y*v2.x) < T(0))
			return T(2)*T(pi()) - acos(a);
		return acos(a);
	}

	template <class T>
		inline T get_angle(const vec3<T>& v1, const vec3<T>& v2)
	{
		T l1 = v1.length();
		T l2 = v2.length();
		T v1dotv2 = v1.dot(v2);
		T a = clamp(v1dotv2/(l1*l2),T(-1),T(1));
		return acos(a);
	}

	class b_istrstream : public std::istrstream
	{
	public:
		b_istrstream(char* buffer) : std::istrstream(buffer) {}
		void load(bool& val) { read((char*)&val,sizeof(val)); }
		void load(short& val) { read((char*)&val,sizeof(val)); }
		void load(unsigned short& val ) { read((char*)&val,sizeof(val)); }
		void load(int& val ) { read((char*)&val,sizeof(val)); }
		void load(unsigned int& val ) { read((char*)&val,sizeof(val)); }
		void load(long& val ) { read((char*)&val,sizeof(val)); }
		void load(unsigned long& val ) {
			read((char*)&val,sizeof(val));
		}
		void load(float& val ) {
			read((char*)&val,sizeof(val));
		}
		void load(double& val ) {
			read((char*)&val,sizeof(val));
		}
		void load(long double& val ) {
			read((char*)&val,sizeof(val));
		}
	};

	class b_ostrstream : public std::ostrstream
	{
	public:
		b_ostrstream() : ostrstream() {}
		void save(const bool& val) {
			write((const char*)&val,sizeof(val));
		}
		void save(const short& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const unsigned short& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const int& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const unsigned int& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const long& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const unsigned long& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const float& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const double& val ) {
			write((const char*)&val,sizeof(val));
		}
		void save(const long double& val ) {
			write((const char*)&val,sizeof(val));
		}
	};

	template <class T> inline mat3<T>::mat3(const quat<T>& q)
	{
		T x = q.v.x, y = q.v.y, z = q.v.z, w = q.w;
		T xx = x*x;
		T yy = y*y;
		T zz = z*z;
		_0 = vec3<T>(T(1)-T(2)*(yy+zz),	T(2)*(x*y + w*z),	T(2)*(x*z-w*y));
		_1 = vec3<T>(T(2)*(x*y-w*z),	T(1)-T(2)*(xx+zz),	T(2)*(y*z+w*x));
		_2 = vec3<T>(T(2)*(x*z+w*y),	T(2)*(y*z-w*x),		T(1)-T(2)*(xx+yy));
	}

	template <class T>
	inline mat4<T>::mat4(const vec3<T>& scale, const vec3<T>& shear, const rotation<T>& rotate, const vec3<T>& translate, const vec4<T>& perspective)
	{
		mat4<T> s(scale[0],scale[1],scale[2]);
		mat4<T> shear_xy(1,		  0,0,0,
						shear[0],1,0,0,
						0,		  0,1,0,
						0,		  0,0,1);
		mat4<T> shear_xz(1,       0,0,0,
						0,	      1,0,0,
						shear[1],0,1,0,
						0,       0,0,1);
		mat4<T> shear_yz(1,       0,0,0,
						0,	      1,0,0,
						0,shear[2],1,0,
						0,       0,0,1);
		quat<T> q = quat<T>(rotate);
		mat4<T> r = mat4<T>(q);
		mat4<T> t(translate);
		mat4<T> p(1, 0, 0, perspective[0],
				  0, 1, 0, perspective[1],
				  0, 0, 1, perspective[2],
				  0, 0, 0, perspective[3]);
		(*this) = s*shear_xy*shear_xz*shear_yz*rx*ry*rz*t*p;
	}
	template <class T> inline mat4<T>::mat4(const quat<T>& q)
	{
		T x = q.v.x, y = q.v.y, z = q.v.z, w = q.w;
		T xx = x*x;
		T yy = y*y;
		T zz = z*z;
		_0 = vec4<T>(T(1)-T(2)*(yy+zz),		T(2)*(x*y+w*z),		T(2)*(x*z-w*y),		T(0));
		_1 = vec4<T>(T(2)*(x*y-w*z),		T(1)-T(2)*(xx+zz),	T(2)*(y*z+w*x),		T(0));
		_2 = vec4<T>(T(2)*(x*z+w*y),		T(2)*(y*z-w*x),		T(1)-T(2)*(xx+yy),	T(0));
		_3 = vec4<T>(T(0),					T(0),				T(0),				T(1));
	}

	template <class T> inline rotation<T>::rotation(const quat<T>& q)
	{
		T len2 = q.v.length2();
		if (len2 < epsiron())
		{
			this->axis	= vec3<T>(T(1),T(0),T(0));
			this->angle	= T(0);
		}
		else
		{
			T inv = T(1)/sqrt(len2);
			this->axis = q.v*inv;
			this->angle = T(2)*acos(q.w);
		}
	}

	template <class T> inline rotation<T> & rotation<T>::operator*=(const rotation<T>& rot)
	{
		return *this = rotation<T>(quat<T>(*this)*quat<T>(rot));
	}

	template <class T> inline rotation<T> rotation<T>::operator * (const rotation<T>& rot) const
	{
		return rotation<T>(quat<T>(*this)*quat<T>(rot));
	}

	template <class T> inline bool mat4<T>::unmatrix(vec3<T>& t, rotation<T>& r, vec3<T>& s) const
	{
		if (fabs((*this).determinant()) < epsiron())
		{
			return false;
		}

		mat4<T> tmp_matrix(*this);
		size_t i, j;

		//
		// Ignore perspective.
		//
		tmp_matrix[0][3] = tmp_matrix[1][3] = tmp_matrix[2][3] = 0.0;

		//
		// Normalize the matrix.
		//
		//for (i = 0; i < 4; i++)
		//	tmp_matrix[i] /= tmp_matrix[3][3];
		tmp_matrix /= tmp_matrix[3][3];

		t.x = tmp_matrix[3][0];
		t.y = tmp_matrix[3][1];
		t.z = tmp_matrix[3][2];

		tmp_matrix[3][0] = tmp_matrix[3][1] = tmp_matrix[3][2] = T(0);
		vec3<T> row_0(tmp_matrix[0][0], tmp_matrix[0][1], tmp_matrix[0][2]);
		vec3<T> row_1(tmp_matrix[1][0], tmp_matrix[1][1], tmp_matrix[1][2]);
		vec3<T> row_2(tmp_matrix[2][0], tmp_matrix[2][1], tmp_matrix[2][2]);

		s.x = row_0.length();
		normalize(row_0);
		s.y = row_1.length();
		normalize(row_1);
		s.z = row_2.length();
		normalize(row_2);
		for (j = 0; j < 3; j++)
		{
			tmp_matrix[0][j] = row_0[j];
			tmp_matrix[1][j] = row_1[j];
			tmp_matrix[2][j] = row_2[j];
		}

		//
		// At this point, the matrix (in rows[]) is orthonormal.
		// That is, each row has length one and are mutually perpendicular.
		// Is it needed to calculate the det of this matrix (1 or -1) for checking
		// coordinate system flip ? (According to VRML standard, scale > 0.0)
		// Calculate quaternion rotation from this matrix.
		//
		quat<T> quat(tmp_matrix);

		//
		// now convert back to axis/angle.
		//
		r = rotation<T>(quat);

		return true;
	}

	template <class T>
	inline bool mat4<T>::unmatrix(vec3<T>& scale, vec3<T>& shear, rotation<T>& rotate, vec3<T>& translate, vec4<T>& perspective) const
	{
		// reference. Graphics Gem II pp.320-323
		const mat4<T> &mat = *this;
		mat4<T> locmat;
		mat4<T> pmat;
		mat3<T> row;
		vec3<T> p;
		locmat = mat;

		// Normalize the matrix
		int i;
		for (i = 0; i < 4; i++)
			locmat[i] /= locmat[3][3];

		pmat = locmat;
		pmat[0][3] = T(0);
		pmat[1][3] = T(0);
		pmat[2][3] = T(0);
		pmat[3][3] = T(1);
		if (pmat.determinant() == T(0))
			return false;
 		/* First, isolate perspective.  This is the messiest. */
 		if ( locmat[0][3] != T(0) || locmat[1][3] != T(0) || locmat[2][3] != T(0) )
		{
 			/* prhs is the right hand side of the equation. */
			vec4<T> prhs(locmat[0][3],locmat[1][3],locmat[2][3],locmat[3][3]);

 			/* Solve the equation by inverting pmat and multiplying
 			* prhs by the inverse.  (This is the easiest way, not
 			* necessarily the best.)
 			* inverse function (and det4x4, above) from the Matrix
 			* Inversion gem in the first volume.
 			*/
 			mat4<T> invpmat = pmat.inverse();
			mat4<T> tinvpmat = invpmat.transpose();
 			vec4<T> psol = prhs*tinvpmat;
		
 			/* Stuff the answer away. */
 			perspective = psol;
 			/* Clear the perspective partition. */
 			locmat[0][3] = locmat[1][3] = locmat[2][3] = 0;
 			locmat[3][3] = 1;
 		}
		else		/* No perspective. */
 			perspective = vec4<T>(0,0,0,1);

		// Take care of translation.
		for (i = 0; i < 3; i++)
		{
			translate[i] = locmat[3][i];
			locmat[3][i] = 0;
		}
		// Now get scale and shear.
		for (i = 0; i < 3; i++)
		{
			row[i][0] = locmat[i][0];
			row[i][1] = locmat[i][1];
			row[i][2] = locmat[i][2];
		}
		scale[0] = normalize(row[0]);
		// Compute XY shear factor and make 2nd row orthogonal to 1st
		shear[2] = row[0].dot(row[1]);
		row[1] -= row[0]*shear[2];
		// Now, compute Y scale and normalize 2nd row.
		scale[1] = normalize(row[1]);
		shear[2] /= scale[1];
		// Compute XY and YZ shears, orthogonalize 3rd row.
		shear[1] = row[0].dot(row[2]);
		row[2] -= row[0]*shear[1];
		shear[0] = row[1].dot(row[2]);
		row[2] -= row[1]*shear[0];

		// Next, get Z scale and normalize 3rd row.
		scale[2] = normalize(row[2]);
		shear[1] /= scale[2];
		shear[0] /= scale[2];
		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip. If the determinant
		// is -1, the negate the matrix and scaling factors.
		p = row[1].cross(row[2]);
		if (row[0].dot(p) < 0.0)
		{
			for (i = 0; i < 3; i++)
			{
				scale[i] *= T(-1);
				row[i][0] *= T(-1);
				row[i][1] *= T(-1);
				row[i][2] *= T(-1);
			}
		}
		quat<T> q(row);
		rotate = rotation<T>(q);
		return true;
	}

	template <class T>
	inline vec2<T> get_uv(const vec3<T>& p, const vec3<T>& n)
	{
		vec2<T> uv;
		T len = vec2<T>(n.x, n.y).length();
		if (len > CG::epsiron()) {
			uv = -p *
				mat4f(atan2(n.x, n.y), vec3f(0,0,1)) *
				mat4f(atan2(len, n.z), vec3f(1,0,0));
		}
		else {
			uv = -p;
		}
		//uv.y *= -uv.y;
		return uv;
	}
}
#endif// _OPT_CG_h

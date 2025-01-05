#pragma once
#include <iostream>
#include <numeric> // For std::gcd

namespace LAR
{
	class Rational
	{
	private:
		int mNumerator;
		int mDenominator;

		void Simplify();

	public:
		Rational(int numerator = 0, int denominator = 1);

		int GetNumerator() const { return mNumerator; }
		int GetDenominator() const { return mDenominator; }

		Rational operator+(const Rational& other) const;
		Rational operator-(const Rational& other) const;
		Rational operator*(const Rational& other) const;
		Rational operator/(const Rational& other) const;

		Rational& operator+=(const Rational& other);
		Rational& operator-=(const Rational& other);
		Rational& operator*=(const Rational& other);
		Rational& operator/=(const Rational& other);

		Rational operator-() const;
		bool operator==(const Rational& other) const;
		bool operator!=(const Rational& other) const;
		bool operator<(const Rational& other) const;
	};

	Rational operator+(const int value, const Rational& rational);
	Rational operator-(const int value, const Rational& rational);
	Rational operator*(const int value, const Rational& rational);
	Rational operator/(const int value, const Rational& rational);

	std::ostream& operator<<(std::ostream& os, const Rational& rational);
	Rational RandomValue(const Rational& min, const Rational& max);
}
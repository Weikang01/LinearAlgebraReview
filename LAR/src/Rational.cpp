#include "LAR/Rational.h"
#include "LAR/Algorithms.h"
namespace LAR
{
	void Rational::Simplify()
	{
		int gcd = std::gcd(mNumerator, mDenominator);
		mNumerator /= gcd;
		mDenominator /= gcd;

		if (mDenominator < 0)
		{
			mNumerator = -mNumerator;
			mDenominator = -mDenominator;
		}
	}
	void Rational::SimplifyForOperation(const Rational& r1, const Rational& r2, int& num1, int& den1, int& num2, int& den2)
	{
		int gcd1 = std::gcd(r1.mNumerator, r2.mDenominator);
		int gcd2 = std::gcd(r2.mNumerator, r1.mDenominator);
		num1 = r1.mNumerator / gcd1;
		den1 = r1.mDenominator / gcd2;
		num2 = r2.mNumerator / gcd2;
		den2 = r2.mDenominator / gcd1;
	}
	;

	Rational::Rational(int numerator, int denominator)
	{
		if (denominator == 0)
			throw std::invalid_argument("Denominator cannot be zero.");
		mNumerator = numerator;
		mDenominator = denominator;

		Simplify();
	}

	Rational Rational::operator+(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		int newNumerator = num1 * den2 + num2 * den1;
		int newDenominator = den1 * den2;

		return Rational(newNumerator, newDenominator);
	}

	Rational Rational::operator-(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);
		
		int newNumerator = num1 * den2 - num2 * den1;
		int newDenominator = den1 * den2;

		return Rational(newNumerator, newDenominator);
	}

	Rational Rational::operator*(const Rational& other) const
	{
		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		return Rational(num1 * num2, den1 * den2);
	}

	Rational Rational::operator/(const Rational& other) const
	{
		if (other.mNumerator == 0)
		{
			throw std::invalid_argument("Division by zero in Rational");
		}

		int num1, den1, num2, den2;
		SimplifyForOperation(*this, other, num1, den1, num2, den2);

		return Rational(num1 * den2, den1 * num2);
	}

	Rational& Rational::operator+=(const Rational& other)
	{
		*this = *this + other;
		return *this;
	}

	Rational& Rational::operator-=(const Rational& other)
	{
		*this = *this - other;
		return *this;
	}

	Rational& Rational::operator*=(const Rational& other)
	{
		*this = *this * other;
		return *this;
	}

	Rational& Rational::operator/=(const Rational& other)
	{
		*this = *this / other;
		return *this;
	}

	Rational Rational::operator-() const
	{
		return Rational(-mNumerator, mDenominator);
	}

	bool Rational::operator==(const Rational& other) const
	{
		return mNumerator == other.mNumerator && mDenominator == other.mDenominator;
	}

	bool Rational::operator!=(const Rational& other) const
	{
		return !(*this == other);
	}

	bool Rational::operator<(const Rational& other) const
	{
		return mNumerator * other.mDenominator < other.mNumerator * mDenominator;
	}

	Rational operator+(const int value, const Rational& rational)
	{
		return Rational(value) + rational;
	}

	Rational operator-(const int value, const Rational& rational)
	{
		return Rational(value) - rational;
	}

	Rational operator*(const int value, const Rational& rational)
	{
		return Rational(value) * rational;
	}

	Rational operator/(const int value, const Rational& rational)
	{
		return Rational(value) / rational;
	}

	std::ostream& operator<<(std::ostream& os, const Rational& rational)
	{
		os << rational.GetNumerator();
		if (rational.GetDenominator() != 1)
		{
			os << "/" << rational.GetDenominator();
		}
		return os;
	}

	Rational RandomValue(const Rational& min, const Rational& max)
	{
		// Compute the least common multiple of the denominators
		int maxDenominator = std::max(min.GetDenominator(), max.GetDenominator());

		// Compute decimal equivalents of min and max
		double minValue = static_cast<double>(min.GetNumerator()) / min.GetDenominator();
		double maxValue = static_cast<double>(max.GetNumerator()) / max.GetDenominator();

		// Generate a random floating-point value in [minValue, maxValue]
		double randomValue = RandomValue(minValue, maxValue);

		// Convert the random value back into a Rational number
		int num = static_cast<int>(round(randomValue * maxDenominator)); // Scaled numerator
		int den = maxDenominator; // Fixed denominator

		// Simplify the fraction using GCD
		int gcd = std::gcd(num, den); // C++17
		num /= gcd;
		den /= gcd;

		if (den < 0) // Ensure positive denominator
		{
			num = -num;
			den = -den;
		}

		return Rational(num, den);
	}

} // namespace LAR
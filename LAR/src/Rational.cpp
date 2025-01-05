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
	};

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
		return Rational(mNumerator * other.mDenominator + other.mNumerator * mDenominator, mDenominator * other.mDenominator);
	}

	Rational Rational::operator-(const Rational& other) const
	{
		return Rational(mNumerator * other.mDenominator - other.mNumerator * mDenominator, mDenominator * other.mDenominator);
	}

	Rational Rational::operator*(const Rational& other) const
	{
		return Rational(mNumerator * other.mNumerator, mDenominator * other.mDenominator);
	}

	Rational Rational::operator/(const Rational& other) const
	{
		return Rational(mNumerator * other.mDenominator, mDenominator * other.mNumerator);
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
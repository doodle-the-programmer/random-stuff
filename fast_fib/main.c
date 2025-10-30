/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpatrici <gpatrici@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:46:47 by gpatrici          #+#    #+#             */
/*   Updated: 2025/10/30 23:10:11 by gpatrici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/*
 * HOW THIS WORKS (NO MATRIX MATH NEEDED!):
 *

	* Think of it like this: instead of adding numbers one by one (fib[n] = fib[n-1]
	+ fib[n-2]),

	* we use a mathematical trick that lets us "jump ahead" using repeated squaring.
 *
 * ANALOGY: Computing 2^100
 * - Slow way: multiply 2 by itself 100 times
 * - Fast way: 2^100 = (2^50)^2 = ((2^25)^2)^2 = ...
 *   This takes only ~7 steps instead of 100!
 *
 * We do the same thing with Fibonacci, but instead of multiplying numbers,
 * we multiply a "package" of 4 numbers that represent the Fibonacci state.
 *
 * The 2x2 array is just a convenient way to store 4 related numbers:
 * F[0][0] will eventually hold our answer (the nth fibonacci number)
 * F[0][1], F[1][0], F[1][1] are helper values needed for the calculation
 */

// This combines two "states" of fibonacci numbers
// Think of it as: given two snapshots of where we are in the sequence,
// combine them to jump further ahead
void	multiply(long long F[2][2], long long M[2][2])
{
	long long	x;
	long long	y;
	long long	z;
	long long	w;

	// These formulas come from the math of how fibonacci numbers relate
	// You don't need to understand WHY these formulas work,
	// just know they correctly combine two fibonacci "states"
	x = F[0][0] * M[0][0] + F[0][1] * M[1][0];
	y = F[0][0] * M[0][1] + F[0][1] * M[1][1];
	z = F[1][0] * M[0][0] + F[1][1] * M[1][0];
	w = F[1][0] * M[0][1] + F[1][1] * M[1][1];
	F[0][0] = x;
	F[0][1] = y;
	F[1][0] = z;
	F[1][1] = w;
}

// This is where the magic happens: "fast exponentiation"
// Instead of doing n operations, we do log(n) operations
void	power(long long F[2][2], int n)
{
	long long	M[2][2];

	if (n == 0 || n == 1)
		return ;
	// M represents "one step forward" in the fibonacci sequence
	M[0][0] = 1;
	M[0][1] = 1;
	M[1][0] = 1;
	M[1][1] = 0;
	// KEY TRICK: To compute F^n, we:
	// 1. Compute F^(n/2) recursively (half the problem!)
	power(F, n / 2);
	// 2. Square it: F^(n/2) * F^(n/2) = F^n (if n is even)
	multiply(F, F);
	// 3. If n was odd, multiply by one more step
	if (n % 2 != 0)
		multiply(F, M);
	// Example: fib(100)
	// - Compute fib(50), square it
	// - To compute fib(50): compute fib(25), square it
	// - To compute fib(25): compute fib(12), square it, multiply by M
	// - etc... only ~7 levels deep instead of 100 steps!
}

// Fast fibonacci using the trick above - O(log n) instead of O(n)
long long	fast_fib(int n)
{
	long long	F[2][2];

	if (n == 0)
		return (0);
	F[0][0] = 1;
	F[0][1] = 1;
	F[1][0] = 1;
	F[1][1] = 0;
	power(F, n - 1);
	return (F[0][0]);
}

// The OLD SLOW way - recursive fibonacci O(2^n) - EXPONENTIALLY SLOW!
// Don't use this for n > 40 or you'll wait forever
long long	slow_fib(int n)
{
	if (n <= 1)
		return (n);
	return (slow_fib(n - 1) + slow_fib(n - 2));
}

// Get current time in microseconds
long long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000LL + tv.tv_usec);
}

int	main(void)
{
	int			n;
	long long	result;
	double		time_sec;
	int			test_values[] = {50, 75, 90};

	long long start, end;
	printf("=== FAST FIBONACCI: O(log n) Matrix Exponentiation ===\n\n");
	// Small values - compare both algorithms
	printf("SPEED COMPARISON:\n");
	for (n = 30; n <= 40; n += 5)
	{
		start = get_time_us();
		result = slow_fib(n);
		end = get_time_us();
		time_sec = (end - start) / 1000000.0;
		printf("SLOW fib(%d) = %lld  [%.6fs]\n", n, result, time_sec);
		start = get_time_us();
		result = fast_fib(n);
		end = get_time_us();
		time_sec = (end - start) / 1000000.0;
		printf("FAST fib(%d) = %lld  [%.6fs]\n\n", n, result, time_sec);
	}
	// Large values - fast only
	printf("LARGE VALUES (slow version would take years):\n");
	for (int i = 0; i < 3; i++)
	{
		n = test_values[i];
		start = get_time_us();
		result = fast_fib(n);
		end = get_time_us();
		time_sec = (end - start) / 1000000.0;
		printf("fib(%d) = %lld  [%.6fs]\n", n, result, time_sec);
	}
	printf("\n⚠️  Max value: fib(92) = %lld\n", fast_fib(92));
	printf("For larger: gcc -o fib_bigint main_bigint.c -lgmp -O3\n");
	return (0);
}

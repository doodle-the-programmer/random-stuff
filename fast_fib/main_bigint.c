/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bigint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpatrici <gpatrici@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:10:00 by gpatrici          #+#    #+#             */
/*   Updated: 2025/10/30 23:15:29 by gpatrici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*
 * FAST FIBONACCI WITH ARBITRARY PRECISION!
 *
 * Using GMP (GNU Multiple Precision Arithmetic Library)
 * Now we can compute fib(1000000) without overflow!
 *
 * Same O(log n) algorithm, but with big integers
 */

// Matrix multiplication for 2x2 matrices using GMP
void	multiply_gmp(mpz_t F[2][2], mpz_t M[2][2])
{
	mpz_t x, y, z, w;
	mpz_t temp1, temp2;
	// Initialize temporary variables
	mpz_init(x);
	mpz_init(y);
	mpz_init(z);
	mpz_init(w);
	mpz_init(temp1);
	mpz_init(temp2);
	// x = F[0][0] * M[0][0] + F[0][1] * M[1][0]
	mpz_mul(temp1, F[0][0], M[0][0]);
	mpz_mul(temp2, F[0][1], M[1][0]);
	mpz_add(x, temp1, temp2);
	// y = F[0][0] * M[0][1] + F[0][1] * M[1][1]
	mpz_mul(temp1, F[0][0], M[0][1]);
	mpz_mul(temp2, F[0][1], M[1][1]);
	mpz_add(y, temp1, temp2);
	// z = F[1][0] * M[0][0] + F[1][1] * M[1][0]
	mpz_mul(temp1, F[1][0], M[0][0]);
	mpz_mul(temp2, F[1][1], M[1][0]);
	mpz_add(z, temp1, temp2);
	// w = F[1][0] * M[0][1] + F[1][1] * M[1][1]
	mpz_mul(temp1, F[1][0], M[0][1]);
	mpz_mul(temp2, F[1][1], M[1][1]);
	mpz_add(w, temp1, temp2);
	// Copy results back
	mpz_set(F[0][0], x);
	mpz_set(F[0][1], y);
	mpz_set(F[1][0], z);
	mpz_set(F[1][1], w);
	// Clean up
	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(z);
	mpz_clear(w);
	mpz_clear(temp1);
	mpz_clear(temp2);
}

// Fast exponentiation with GMP
void	power_gmp(mpz_t F[2][2], int n)
{
	mpz_t	M[2][2];

	if (n == 0 || n == 1)
		return ;
	// Initialize M matrix
	mpz_init_set_ui(M[0][0], 1);
	mpz_init_set_ui(M[0][1], 1);
	mpz_init_set_ui(M[1][0], 1);
	mpz_init_set_ui(M[1][1], 0);
	// Recursive fast exponentiation
	power_gmp(F, n / 2);
	multiply_gmp(F, F);
	if (n % 2 != 0)
		multiply_gmp(F, M);
	// Clean up M
	mpz_clear(M[0][0]);
	mpz_clear(M[0][1]);
	mpz_clear(M[1][0]);
	mpz_clear(M[1][1]);
}

// Fast fibonacci with arbitrary precision - O(log n)
void	fast_fib_gmp(mpz_t result, int n)
{
	mpz_t	F[2][2];

	if (n == 0)
	{
		mpz_set_ui(result, 0);
		return ;
	}
	// Initialize F matrix
	mpz_init_set_ui(F[0][0], 1);
	mpz_init_set_ui(F[0][1], 1);
	mpz_init_set_ui(F[1][0], 1);
	mpz_init_set_ui(F[1][1], 0);
	power_gmp(F, n - 1);
	mpz_set(result, F[0][0]);
	// Clean up F
	mpz_clear(F[0][0]);
	mpz_clear(F[0][1]);
	mpz_clear(F[1][0]);
	mpz_clear(F[1][1]);
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
	mpz_t	result;
	double	time_sec;
	int		test_values[] = {50, 100, 1000, 10000, 100000, 1000000, 10000000,
				100000000};
	int		i;
	int		n;
	char	*str;
	size_t	len;

	long long start, end;
	mpz_init(result);
	printf("=== FAST FIBONACCI (ARBITRARY PRECISION) ===\n\n");
	// Test various sizes
	for (i = 0; i < 8; i++)
	{
		n = test_values[i];
		start = get_time_us();
		fast_fib_gmp(result, n);
		end = get_time_us();
		time_sec = (end - start) / 1000000.0;
		printf("fib(%d) = ", n);
		// For small numbers, print the full value
		if (n <= 100)
		{
			gmp_printf("%Zd", result);
			printf("  [%.6fs]\n", time_sec);
		}
		else
		{
			// For large numbers, show first/last digits and digit count
			str = mpz_get_str(NULL, 10, result);
			len = strlen(str);
			if (len <= 100)
			{
				printf("%s", str);
			}
			else
			{
				// Print first 50 and last 50 digits
				printf("%.50s...%.50s", str, str + len - 50);
			}
			printf("\n  [%zu digits, %.6fs]\n", len, time_sec);
			free(str);
		}
		printf("\n");
	}
	mpz_clear(result);
	return (0);
}

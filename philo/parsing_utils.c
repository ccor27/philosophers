#include "philo.h"

/**
 * Function to validate if a character is a space
 */
int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/**
 * Function to validate if a character is a digit
 */
int	ft_isdigit(char c)
{
	return (c >= 48 && c <= 57);
}

/**
 * Function to return a pointer to the first number
 *
 * so here we'll check for
 * 	- negative numbers
 * 	- if the number is legit
 * 			"   +78$%asd"  VALID
 * 			"   sdfferf87" NOT VALID
 *	- for MAX_INT, but just by the len
 *  	of the string
 */
const char	*ft_skip_no_digits(const char *s)
{
	int			len;
	const char	*number;

	len = 0;
	while (ft_isspace(*s))
		s++;
	if (*s == '+')
		s++;
	else if (*s == '-')
		ft_exit("Only positive number allowed", 1);
	number = s;
	while (ft_isdigit(*s++))
		len++;
	if (len > 10)
		ft_exit("Number is too big (INT_MAX is the limit)", 1);
	return (number);
}

/**
 * Function to parse numbers and check if there are
 * grather than the MAX_INT
 */
long	ft_atol(const char *s)
{
	long	num;

	num = 0;
	s = ft_skip_no_digits(s);
	while (ft_isdigit(*s))
		num = (num * 10) + (*s++ - 48);
	if (num > 2147483647)
		ft_exit("Number is too big (INT_MAX is the limit)", 1);
	return (num);
}

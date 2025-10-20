#include "philo.h"

/**
 * Function to validate if a character is a space
 */
int  ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/**
 * Function to validate if a character is a digit
 */
int  ft_isdigit(char c)
{
	return (c >= 48 && c <= 57);
}


/**
 * Function to return a pointer to the first number
 *
 * so here we'll check for
 * 1) negative numbers
 * 2) if the number is legit
 * 	"   +78$%asd"  VALID
 * 	"   sdfferf87" NOT VALID
 * 3)for limits (max_int)
 * 	3.1) filter for the len of th string, if len > 10 for sure is more than int_max
 */
const char *ft_skip_no_digits(char *s)
{
	int len;
	const char *number;
	len = 0;
	//skip spaces
	while(ft_isspace(*s))
		s++;
	if(*s=='+')
		s++;
	else if(*s=='-')
		error_exit("Only positive number allowed");
	number = s;//we now are in the position of the first number
	while(ft_isdigit(*s++))
		len++;
	if(len>10)
		error_exit("Number is too big (INT_MAX is the limit)");
	return (number);
}

/**
* Function to parse the string number from arguments
* 	1) check this case 2_147_483_647 -> 9_999_999_999
*/
long ft_atol(const char *s)
{
	long num;

	num = 0;
	s = ft_skip_no_digits(s);
	while(ft_isdigit(*s))
		num = (num * 10) + (*s++ - 48);
	if(num > INT_MAX)
		error_exit("Number is too big (INT_MAX is the limit)");
	return (num);
}

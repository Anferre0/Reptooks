
#include "ft_printf.h"

int	ft_strpercent(const char *str)
{
	int		i;
	int		ct;

	i = 0;
	ct = 0;
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1] && str[i + 1] != '%')
			ct++;
		i++;
	}
	return (ct);
}

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

int	ft_strlen(char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_putnbr(unsigned long nb, int *count)
{
	long nb2;

	nb2 = nb;
	if (nb2 < 0)
	{
		ft_putchar('-');
		nb2 *= -1;
		(*count)++;
	}
	if (nb2 > 9)
		ft_putnbr(nb2 / 10, count);
	ft_putchar((nb2 % 10) + '0');
	(*count)++;
	return (*count);
}

int ft_putnbr_base(unsigned long nb, char *base, int *count)
{
	long	nb2;
	int		baselen;
	
	nb2 = nb;
	baselen = 16;
	if (nb2 < 0)
	{
		ft_putchar('-');
		nb2 *= -1;
		(*count)++;
	}
	if (nb2 >= 16)
		ft_putnbr_base(nb2 / baselen, base, count);
	ft_putchar(base[nb2 % baselen]);
	(*count)++;
	return (*count);	
}

void	ft_simplechar(char c, int *count)
{
	ft_putchar(c);
	(*count)++;
}

void	ft_string(char *str, int *count)
{
	unsigned int	i;

	if (!str)
	{
		write(1,"(null)", 6);
		*count += 6;
		return ;
	}
	i = ft_strlen(str);
	write(1, str, i);
	*count += i;
}

void	ft_putptr(unsigned long address, int *count)
{

	if(address >= 16)
	{
		ft_putptr(address / 16, count);
		ft_putptr(address % 16, count);
	}
	else
	{
		if (address < 10)
			ft_putchar(address + '0');
		else
			ft_putchar(address - 10 + 'a');
		*count += 1;
	}
}
void	ft_ptr(void *ptr, int *count)
{
	
	unsigned long	address;
	unsigned int	i;

	i = 0;
	if (!ptr)
	{
		write(1,"(nil)", 5);
		*count += 5;
		return ;
	}
	address = (unsigned long)ptr;
	write(1, "0x", 2);
	*count += 2;
	ft_putptr(address, count);
}

void	ft_decimal(int j, int *count)
{
	*count = ft_putnbr(j, count);
}

void	ft_int(int j, int *count)
{
	*count = ft_putnbr(j, count);
}

void	ft_unsigneddeci(unsigned long u, int *count)
{
	*count = ft_putnbr(u, count);
	
}

void	ft_lowerhex(unsigned long h, int *count)
{
	char	*base;

	base = "0123456789abcdef";
	*count = ft_putnbr_base(h, base, count);
}

void	ft_upperhex(unsigned long h, int *count)
{
	char	*base;

	base = "0123456789ABCDEF";
	*count = ft_putnbr_base(h, base, count);
}

void	ft_percent(int *count)
{
	ft_putchar('%');
	(*count)++;
}
void	ft_check(char	c, va_list args, int *ct)
{
	if (c == 'c')
		ft_simplechar((char)va_arg(args, int), ct);
	else if (c == 's')
		ft_string(va_arg(args, char *), ct);
	else if (c == 'p')
		ft_ptr(va_arg(args, char *), ct);
	else if (c == 'd')
		ft_decimal(va_arg(args, int), ct);
	else if (c == 'i')
		ft_int(va_arg(args, int), ct);
	else if (c == 'u')
		ft_unsigneddeci(va_arg(args, unsigned long), ct);
	else if (c == 'x')
		ft_lowerhex(va_arg(args, unsigned long), ct);
	else if (c == 'X')
		ft_upperhex(va_arg(args, unsigned long), ct);
	else if (c == '%')
		ft_percent(ct);
}
int ft_printf(const char *s, ...)
{
	int		i;
	int 	count;
	int		*ct;
	va_list args;

	i = 0;
	count = 0;
	va_start(args, s);
	ct = &count;
	while(s[i])
	{
		if (s[i] == '%' && s[i + 1] != '\0')
		{
			ft_check(s[i + 1], args, ct);
			i += 2;
		}
		else
		{
			ft_putchar(s[i]);
			i++;
			count++;
		}
	}
	va_end(args);
	return (*ct);
}
#include <stdio.h>

int main()
{
	int ct;


	ct = ft_printf("%u", 4294967295);
	printf("%d\n", ct);
	ct = printf("%u", 4294967295);
	printf("%d\n", ct);
	
	return (0);
}
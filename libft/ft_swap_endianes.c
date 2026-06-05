#include<libft.h>

uint64_t	ft_swapLongEndian(uint64_t nbr)
{
	uint8_t b1 = (uint8_t)(nbr >> 56);
	uint8_t b2 = (uint8_t)(nbr >> 48);
	uint8_t b3 = (uint8_t)(nbr >> 40);
	uint8_t b4 = (uint8_t)(nbr >> 32);
	uint8_t b5 = (uint8_t)(nbr >> 24);
	uint8_t b6 = (uint8_t)(nbr >> 16);
	uint8_t b7 = (uint8_t)(nbr >> 8);
	uint8_t b8 = (uint8_t)(nbr);

	uint64_t newnbr = ((uint64_t)b8 << 56) | ((uint64_t)b7 << 48) | ((uint64_t)b6 << 40) | ((uint64_t)b5 << 32) | ((uint64_t)b4 << 24) | ((uint64_t)b3 << 16) | ((uint64_t)b2 << 8) | ((uint64_t)b1);

	return (newnbr);
}

uint32_t	ft_swapIntEndian(uint32_t nbr)
{
	uint8_t b1 = (uint8_t)(nbr >> 24);
	uint8_t b2 = (uint8_t)(nbr >> 16);
	uint8_t b3 = (uint8_t)(nbr >> 8);
	uint8_t b4 = (uint8_t)(nbr);

	uint32_t newnbr = ((uint32_t)b4 << 24) | ((uint32_t)b3 << 16) | ((uint32_t)b2 << 8) | ((uint32_t)b1);

	return (newnbr);
}

uint16_t	ft_swapShortEndian(uint16_t nbr)
{
	uint8_t b1 = (uint8_t)(nbr >> 8);
	uint8_t b2 = (uint8_t)(nbr);

	uint16_t newnbr = ((uint16_t)b2 << 8) | ((uint16_t)b1);

	return (newnbr);
}
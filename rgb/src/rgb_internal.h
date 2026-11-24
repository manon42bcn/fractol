#ifndef RGB_INTERNAL_H
# define RGB_INTERNAL_H

/** @brief Decode an sRGB byte to linear light in the range [0, 1]. */
double	ft_rgb_decode(int channel);

/** @brief Clip linear light to [0, 1] and encode it as an sRGB byte. */
int		ft_rgb_encode(double value);

#endif

# AVX / AVX-512 mathfunc

This package includes the AVX-optimized implementations of
mathematical functions (trigonometric, exponential, logarithmic and
cubic root functions) based on the original implementation
[avx_mathfun.h](https://github.com/gioGarbe/avx_mathfun) by Giovanni Garberoglio.

The major difference from the original version is that this package has additional 
implementation of cubic root function and implementations with the AVX-512 instructions.
The original implementations are also further optimized with FMA instructions. 

## Licences (zlib license)
Copyright (c) 2019 Kohji Yoshikawa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2.Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3.This notice may not be removed or altered from any source distribution.

/*
<https://github.com/rafagafe/base64>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include "base64.h"

/** Escape values. */
enum special_e {
    notabase64 = 64, /**< Value to return when a non base64 digit is found. */
    terminator = 65, /**< Value to return when the character '=' is found.  */
};

/** Lookup table that converts a integer to base64 digit. */
static char const bintodigit[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789"
                                 "+/";

/** Get the first base 64 digit of a block of 4.
  * @param a The first byte of the source block of 3.
  * @return A base 64 digit. */
static s32 get0( s32 a ) {
    s32 const index = a >> 2u;
    return bintodigit[ index ];
}

/** Get the second base 64 digit of a block of 4.
  * @param a The first byte of the source block of 3.
  * @param b The second byte of the source block of 3.
  * @return A base 64 digit. */
static s32 get1( s32 a, s32 b ) {
    s32 const indexA = ( a & 3 ) << 4u;
    s32 const indexB = b >> 4u;
    s32 const index  = indexA | indexB;
    return bintodigit[ index ];
}

/** Get the third base 64 digit of a block of 4.
  * @param b The second byte of the source block of 3.
  * @param c The third byte of the source block of 3.
  * @return A base 64 digit. */
static u32 get2( u32 b, u32 c ) {
    s32 const indexB = ( b & 15 ) << 2u;
    s32 const indexC = c >> 6u;
    s32 const index  = indexB | indexC;
    return bintodigit[ index ];
}

/** Get the fourth base 64 digit of a block of 4.
  * @param c The third byte of the source block of 3.
  * @return A base 64 digit. */
static s32 get3( s32 c ) {
    s32 const index = c & 0x3f;
    return bintodigit[ index ];
}

/* Convert a binary memory block in a base64 null-terminated string. */
char* bintob64( char* dest, void const* src, u32 size ) {

    typedef struct { u8 a; u8 b; u8 c; } block_t;
    block_t const* block = (block_t*)src;
    for( ; size >= sizeof( block_t ); size -= sizeof( block_t ), ++block ) {
        *dest++ = get0( block->a );
        *dest++ = get1( block->a, block->b );
        *dest++ = get2( block->b, block->c );
        *dest++ = get3( block->c );
    }

    if ( !size ) goto final;

    *dest++ = get0( block->a );
    if ( !--size ) {
        *dest++ = get1( block->a, 0 );
        *dest++ = '=';
        *dest++ = '=';
        goto final;
    }

    *dest++ = get1( block->a, block->b );
    *dest++ = get2( block->b, 0 );
    *dest++ = '=';

  final:
    *dest = '\0';
    return dest;
}

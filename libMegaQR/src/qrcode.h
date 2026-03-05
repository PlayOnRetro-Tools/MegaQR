/**
 * The MIT License (MIT)
 *
 * This library is written and maintained by Richard Moore.
 * Major parts were derived from Project Nayuki's library.
 *
 * Copyright (c) 2017 Richard Moore     (https://github.com/ricmoo/QRCode)
 * Copyright (c) 2017 Project Nayuki    (https://www.nayuki.io/page/qr-code-generator-library)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *  Special thanks to Nayuki (https://www.nayuki.io/) from which this library was
 *  heavily inspired and compared against.
 *
 *  See: https://github.com/nayuki/QR-Code-generator/tree/master/cpp
 */


#pragma once

#include <genesis.h>

// QR Code Format Encoding
#define MODE_NUMERIC        0
#define MODE_ALPHANUMERIC   1
#define MODE_BYTE           2

// Error Correction Code Levels
#define ECC_LOW            0
#define ECC_MEDIUM         1
#define ECC_QUARTILE       2
#define ECC_HIGH           3

// If set to non-zero, this library can ONLY produce QR codes at that version
// This saves a lot of dynamic memory, as the codeword tables are skipped
#ifndef LOCK_VERSION
#define LOCK_VERSION       0
#endif

typedef struct QRCode
{
    u8 version;
    u8 size;
    u8 ecc;
    u8 mode;
    u8 mask;
    u8 *modules;
} QRCode;

u16     qrcode_getBufferSize(u8 version);
s8      qrcode_initText(QRCode *qrcode, u8 *modules, u8 version, u8 ecc, const char *data);
s8      qrcode_initBytes(QRCode *qrcode, u8 *modules, u8 version, u8 ecc, u8 *data, u16 length);
bool    qrcode_getModule(QRCode *qrcode, s32 x, s32 y);

//
// Created by Botan on 11/11/18.
//


#include <float.h>
#include "decimal.h"

double read_double(char *buffer) {
    int i;
    double fnorm = 0.0;
    unsigned char temp;
    int sign;
    int exponent;
    double bitval;
    int maski, mask;
    int expbits = 11;
    int significandbits = 52;
    int shift;
    double answer;


    sign = buffer[0] & 0x80 ? -1 : 1;
    exponent = ((buffer[0] & 0x7F) << 4) | ((buffer[1] & 0xF0) >> 4);
    bitval = 0.5;
    maski = 1;
    mask = 0x08;
    for (i = 0; i < significandbits; i++) {
        if (buffer[maski] & mask)
            fnorm += bitval;

        bitval /= 2.0;
        mask >>= 1;
        if (mask == 0) {
            mask = 0x80;
            maski++;
        }
    }
    if (exponent == 0 && fnorm == 0)
        return 0.0;

    shift = exponent - ((1 << (expbits - 1)) - 1);
    if (shift == 1024 && fnorm != 0)
        return sqrt(-1.0);
    if (shift == 1024 && fnorm == 0) {

#ifdef INFINITY
        return sign == 1 ? INFINITY : -INFINITY;
#endif

        return (sign * 1.0) / 0.0;
    }
    if (shift > -1023) {
        answer = ldexp(fnorm + 1.0, shift);
        return answer * sign;
    } else {
        if (fnorm == 0.0)
            return 0.0;
        shift = -1022;
        while (fnorm < 1.0) {
            fnorm *= 2;
            shift--;
        }
        answer = ldexp(fnorm, shift);
        return answer * sign;
    }
}


float read_float(char *buffer) {
    unsigned long buff = 0;
    unsigned long buff2 = 0;
    unsigned long mask;
    int sign;
    int exponent;
    int shift;
    int i;
    int significandbits = 23;
    int expbits = 8;
    double fnorm = 0.0;
    double bitval;
    double answer;

    for (i = 0; i < 4; i++)
        buff = (buff << 8) | buffer[i];


    sign = (buff & 0x80000000) ? -1 : 1;
    mask = 0x00400000;
    exponent = (buff & 0x7F800000) >> 23;
    bitval = 0.5;
    for (i = 0; i < significandbits; i++) {
        if (buff & mask)
            fnorm += bitval;
        bitval /= 2;
        mask >>= 1;
    }
    if (exponent == 0 && fnorm == 0.0)
        return 0.0f;
    shift = exponent - ((1 << (expbits - 1)) - 1);

    if (shift == 128 && fnorm != 0.0)
        return (float) sqrt(-1.0);
    if (shift == 128 && fnorm == 0.0) {
#ifdef INFINITY
        return sign == 1 ? INFINITY : -INFINITY;
#endif
        return (sign * 1.0f) / 0.0f;
    }
    if (shift > -127) {
        answer = ldexp(fnorm + 1.0, shift);
        return (float) answer * sign;
    } else {
        if (fnorm == 0.0) {
            return 0.0f;
        }
        shift = -126;
        while (fnorm < 1.0) {
            fnorm *= 2;
            shift--;
        }
        answer = ldexp(fnorm, shift);
        return (float) answer * sign;
    }
}

void write_double(double x, char *buffer) {
    int shift;
    unsigned long sign, exp, hibits, hilong, lowlong;
    double fnorm, significand;
    int expbits = 11;
    int significandbits = 52;

    if (x == 0) {
        hilong = 0;
        lowlong = 0;
        goto writedata;
    }

    if (x > DBL_MAX) {
        hilong = 1024 + ((1 << (expbits - 1)) - 1);
        hilong <<= (31 - expbits);
        lowlong = 0;
        goto writedata;
    }

    if (x < -DBL_MAX) {
        hilong = 1024 + ((1 << (expbits - 1)) - 1);
        hilong <<= (31 - expbits);
        hilong |= (1 << 31);
        lowlong = 0;
        goto writedata;
    }

    if (x != x) {
        hilong = 1024 + ((1 << (expbits - 1)) - 1);
        hilong <<= (31 - expbits);
        lowlong = 1234;
        goto writedata;
    }

    if (x < 0) {
        sign = 1;
        fnorm = -x;
    } else {
        sign = 0;
        fnorm = x;
    }

    shift = 0;
    while (fnorm >= 2.0) {
        fnorm /= 2.0;
        shift++;
    }
    while (fnorm < 1.0) {
        fnorm *= 2.0;
        shift--;
    }

    if (shift < -1022) {
        while (shift < -1022) {
            fnorm /= 2.0;
            shift++;
        }
        shift = -1023;
    } else if (shift > 1023) {
        hilong = 1024 + ((1 << (expbits - 1)) - 1);
        hilong <<= (31 - expbits);
        hilong |= (sign << 31);
        lowlong = 0;
        goto writedata;
    } else
        fnorm = fnorm - 1.0;


    significand = fnorm * ((1LL << significandbits) + 0.5f);

    exp = shift + ((1 << (expbits - 1)) - 1);

    hibits = (long) (significand / 4294967296);
    hilong = (sign << 31) | (exp << (31 - expbits)) | hibits;
    x = significand - hibits * 4294967296;
    lowlong = (unsigned long) (significand - hibits * 4294967296);

    writedata:
    buffer[0] = (char) ((hilong >> 24) & 0xFF);
    buffer[1] = (char) ((hilong >> 16) & 0xFF);
    buffer[2] = (char) ((hilong >> 8) & 0xFF);
    buffer[3] = (char) (hilong & 0xFF);
    buffer[4] = (char) ((lowlong >> 24) & 0xFF);
    buffer[5] = (char) ((lowlong >> 16) & 0xFF);
    buffer[6] = (char) ((lowlong >> 8) & 0xFF);
    buffer[7] = (char) (lowlong & 0xFF);
}


void write_float(float x, char *buffer) {
    int shift;
    unsigned long sign, exp, hibits, buff;
    double fnorm, significand;
    int expbits = 8;
    int significandbits = 23;

    if (x == 0) {
        buff = 0;
        goto writedata;
    }

    if (x > FLT_MAX) {
        buff = (128 + ((1 << (expbits - 1)) - 1));
        buff <<= (31 - expbits);
        goto writedata;
    }

    if (x < -FLT_MAX) {
        buff = (128 + ((1 << (expbits - 1)) - 1));
        buff <<= (31 - expbits);
        buff |= (1 << 31);
        goto writedata;
    }

    if (x != x) {
        buff = 128 + ((1 << (expbits - 1)) - 1);
        buff <<= (31 - expbits);
        buff |= 1234;
        goto writedata;
    }

    if (x < 0) {
        sign = 1;
        fnorm = -x;
    } else {
        sign = 0;
        fnorm = x;
    }

    shift = 0;
    while (fnorm >= 2.0) {
        fnorm /= 2.0;
        shift++;
    }

    while (fnorm < 1.0) {
        fnorm *= 2.0;
        shift--;
    }

    if (shift < -126) {
        while (shift < -126) {
            fnorm /= 2.0;
            shift++;
        }
        shift = -1023;
    } else if (shift > 128) {
        buff = 128 + ((1 << (expbits - 1)) - 1);
        buff <<= (31 - expbits);
        buff |= (sign << 31);
        goto writedata;
    } else
        fnorm = fnorm - 1.0;

    significand = fnorm * ((1LL << significandbits) + 0.5f);


    exp = (shift + ((1 << (expbits - 1)) - 1));
    hibits = (long) (significand);
    buff = (sign << 31) | (exp << (31 - expbits)) | hibits;

    writedata:
    buffer[0] = (char) ((buff >> 24) & 0xFF);
    buffer[1] = (char) ((buff >> 16) & 0xFF);
    buffer[2] = (char) ((buff >> 8) & 0xFF);
    buffer[3] = (char) (buff & 0xF);
}
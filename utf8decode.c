/* https://nullprogram.com/blog/2017/10/06/ */

unsigned char *
utf8_decode(unsigned char *s, int *c)
{
    unsigned char *next;
    if (s[0] < 0x80) {
        *c = s[0];
        next = s + 1;
    } else if ((s[0] & 0xe0) == 0xc0) {
        *c = ((int)(s[0] & 0x1f) <<  6) |
             ((int)(s[1] & 0x3f) <<  0);
        next = s + 2;
    } else if ((s[0] & 0xf0) == 0xe0) {
        *c = ((int)(s[0] & 0x0f) << 12) |
             ((int)(s[1] & 0x3f) <<  6) |
             ((int)(s[2] & 0x3f) <<  0);
        next = s + 3;
    } else if ((s[0] & 0xf8) == 0xf0 && (s[0] <= 0xf4)) {
        *c = ((int)(s[0] & 0x07) << 18) |
             ((int)(s[1] & 0x3f) << 12) |
             ((int)(s[2] & 0x3f) <<  6) |
             ((int)(s[3] & 0x3f) <<  0);
        next = s + 4;
    } else {
        *c = -1; // invalid
        next = s + 1; // skip this byte
    }
    if (*c >= 0xd800 && *c <= 0xdfff)
        *c = -1; // surrogate half
    return next;
}

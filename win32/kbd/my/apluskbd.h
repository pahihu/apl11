/** @package

    apluskbd.c

    Copyright(c) Ergodesign Bt. 2000

    A+ keyboard map

    Author: ANDRAS PAHI
    Created: AP  12/3/2003 10:51:47 AM
	  Last change: AP 12/3/2003 12:26:58 PM
*/

static char* aplus_kbd[] = {
"`",C_DIAMOND,
"~",C_TILDE,
"1",C_DIAERESIS,
"!",C_IBEAM,
"2",C_OVERBAR,
"@",C_DEL_TILDE,
"3",C_LEFT_CARET,
"#",C_DEL_STILE,
"4",C_LT_EQUAL,
"$",C_DELTA_STILE,
"5",C_EQUAL,
"%",C_CIRCLE_STILE,
"6",C_GT_EQUAL,
"^",C_CIRCLE_BACKSLASH,
"7",C_RIGHT_CARET,
"&",C_CIRCLE_BAR,
"8",C_NOT_EQUAL,
"*",C_CIRCLE_STAR,
"9",C_DOWN_CARET,
"(",C_DOWN_CARET_TILDE,
"0",C_UP_CARET,
")",C_UP_CARET_TILDE,
"-",C_MULTIPLY,
"_",C_QUOTE_DOT,
"=",C_DIVIDE,
"+",C_QUAD_DIVIDE,
"q",C_QUERY,
#ifdef C_LOGAMMA
"Q",C_LOGAMMA,
#endif
#ifdef C_OMEGA
"w",C_OMEGA,
#endif
"W","W",
"e",C_EPSILON,
#ifdef C_EPSILON_UNDERBAR
"E",C_EPSILON_UNDERBAR,
#endif
"r",C_RHO,
"R","R",
"t",C_TILDE,
"T","T",
"y",C_UP_ARROW,
#ifdef C_YEN
"Y",C_YEN,
#endif
"u",C_DOWN_ARROW,
"U","U",
"i",C_IOTA,
"I","I",
"o",C_CIRCLE,
#ifdef C_CIRCLE_DIAERESIS
"O",C_CIRCLE_DIAERESIS,
#endif
"p",C_STAR,
#ifdef C_FONT
"P",C_FONT,
#endif
"[",C_LEFT_ARROW,
#ifdef C_LEFT_TACK
"{",C_LEFT_TACK,
#endif
"]",C_RIGHT_ARROW,
#ifdef C_RIGHT_TACK
"}",C_RIGHT_TACK,
#endif
"\\",C_BACKSLASH_BAR,
#ifdef C_DELTA_UNDERBAR
"|",C_DELTA_UNDERBAR,
#endif
#ifdef C_ALPHA
"a",C_ALPHA,
#endif
"A","A",
"s",C_UP_STILE,
#ifdef C_SQUAD
"S",C_SQUAD,
#endif
"d",C_DOWN_STILE,
"D","D",
"f",C_UNDERBAR,
"F",C_EQUAL_UNDERBAR,
"g",C_DEL,
"G",C_DEL_STILE,
"h",C_DELTA,
"H",C_DELTA_STILE,
"j",C_JOT,
#ifdef C_DIAERESIS_JOT
"J",C_DIAERESIS_JOT,
#endif
"k",C_QUOTE,
"K","K",
"l",C_QUAD,
"L",C_QUOTE_QUAD,
#ifdef C_RIGHT_TACK
";",C_RIGHT_TACK,
#endif
":",":",
#ifdef C_LEFT_TACK
"'",C_LEFT_TACK,
#endif
"\"","\"",
#ifdef C_LEFT_SHOE
"z",C_LEFT_SHOE,
#endif
#ifdef C_CENT
"Z",C_CENT,
#endif
"x",C_RIGHT_SHOE,
"X","X",
#ifdef C_UP_SHOE
"c",C_UP_SHOE,
#endif
"C",C_UP_SHOE_JOT,
#ifdef C_DOWN_SHOE
"v",C_DOWN_SHOE,
#endif
"V","V",
"b",C_UP_TACK,
"B",C_UP_TACK_JOT,
"n",C_DOWN_TACK,
"N",C_DOWN_TACK_JOT,
"m",C_STILE,
"M",C_STILE,
",",C_COMMA_BAR,
"<",C_LEFT_CARET,
".",C_BACKSLASH_BAR,
">",">",
"/",C_SLASH_BAR,
"?",C_QUERY
};

#define APLUSKBD_SIZE		(sizeof(aplus_kbd)/sizeof(char*))

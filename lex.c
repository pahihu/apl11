/* APL! ("APL-bang", an APLish ascii APL) embedded in APL\11
   William Chang (wchang@cshl.org 516-367-8866)

   <-  ->  <=  >=  ~=  are obvious
   I I-beam   @ quad   & delta   _ underline
   `1 is -1  `@ is quote-quad
   $ execute  # format  (were # $ respectively in original comp.lang.apl msg)

   ! iota   ? rho   ^ take   ~ drop   * times  are the only tricky ones

   Since these five are used much more frequently than most other APL functions,
   they must have good-looking symbols in order to keep programs readable,
   concise, and "APLish".  Other functions can be given keywords without
   significantly increasing program size.  Since an automatic translator
   cannot decide whether an APL symbol is used with one or two arguments,
   APL! keywords are uniformly the binary function names.  They are

   not and or nand nor max min exp log in up down deal solve circle choose
   encode decode rotate transpose outer

   which is fewer than the number of keywords in C for example.  Finally,
   / \ and rotate along the leading axis should be written as /[I29] etc.
   (I29 is index origin).

   I am considering unary keywords as well as keeping ~ for "not" when
   usage is clearly unary.
*/

#define lv yylval
#define v yyval

int xxpeek[2] = {0,0};

yylex()
{
    int c, rval;
    struct tab *tp;

    if(nlexsym != -1) {                /* first token is lexical context */
        c = nlexsym;
        nlexsym = -1;
        return(c);
    }
    while(litflag > 0) {            /* comment */
        c = *iline++;
        if(c == '\n') {
            nlexsym = 0;
            return(eol);
        }
    }
    if(xxpeek[0] != 0){
        lv.charval = xxpeek[0];        /* may want charptr here */
        xxpeek[0] = 0;
        return(xxpeek[1]);
    }
    do
        c = *iline++;
    while(c == ' ');
    if(c == '\n') {
        nlexsym = 0;
        return(eol);
    }

/* following modified or added by William Chang */

#if 0
    /* Mitloehner's system, .keyword */
    if ((c=='.' || c=='`') && alpha(*iline))
    {
      if (0==strncasecmp(iline,"in",2)) c='E';
      if (0==strncasecmp(iline,"or",2)) c='V';
      if (0==strncasecmp(iline,"tr",2)) c=0212;
      if (0==strncasecmp(iline,"up",2)) c=0215;
      if (0==strncasecmp(iline,"ce",2)) c='S';
      if (0==strncasecmp(iline,"hi",2)) c='S';
      if (0==strncasecmp(iline,"fl",2)) c='D';
      if (0==strncasecmp(iline,"lo",2)) c='D';
      if (0==strncasecmp(iline,"io",2)) c='!';
      if (0==strncasecmp(iline,"le",2)) c=0220;
      if (0==strncasecmp(iline,"ge",2)) c=0221;
      if (0==strncasecmp(iline,"ne",2)) c=0222;
      if (0==strncasecmp(iline,"is",2)) c='{';
      if (0==strncasecmp(iline,"go",2)) c='}';
      if (0==strncasecmp(iline,"qq",2)) c=0202;
      if (0==strncasecmp(iline,"ib",2)) c='I';
      if (0==strncasecmp(iline,"rho",3)) c='?';
      if (0==strncasecmp(iline,"max",3)) c='S';
      if (0==strncasecmp(iline,"min",3)) c='D';
      if (0==strncasecmp(iline,"exp",3)) c='P';
      if (0==strncasecmp(iline,"log",3)) c=0207;
      if (0==strncasecmp(iline,"rot",3)) c=0217;
      if (0==strncasecmp(iline,"not",3)) c='T';
      if (0==strncasecmp(iline,"and",3)) c='A';
      if (0==strncasecmp(iline,"nor",3)) c=0224;
      if (0==strncasecmp(iline,"cir",3)) c='O';
      if (0==strncasecmp(iline,"enc",3)) c='N';
      if (0==strncasecmp(iline,"dec",3)) c='B';
      if (0==strncasecmp(iline,"exec",4)) c=0241;
      if (0==strncasecmp(iline,"take",4)) c='^';
      if (0==strncasecmp(iline,"drop",4)) c='~';
      if (0==strncasecmp(iline,"down",4)) c=0216;
      if (0==strncasecmp(iline,"nand",4)) c=0223;
      if (0==strncasecmp(iline,"roll",4)) c='Q';
      if (0==strncasecmp(iline,"deal",4)) c='Q';
      if (0==strncasecmp(iline,"fact",4)) c=0210;
      if (0==strncasecmp(iline,"comb",4)) c=0210;
      if (0==strncasecmp(iline,"trig",4)) c='O';
      if (0==strncasecmp(iline,"quad",4)) c='@';
      if (0==strncasecmp(iline,"times",5)) c='*';
      if (0==strncasecmp(iline,"solve",5)) c=0214;
      if (0==strncasecmp(iline,"outer",5)) c='J';
      if (0==strncasecmp(iline,"member",6)) c='E';
      if (0==strncasecmp(iline,"domino",6)) c=0214;
      if (0==strncasecmp(iline,"choose",6)) c=0210;
      if (0==strncasecmp(iline,"gradeu",6)) c=0215;
      if (0==strncasecmp(iline,"graded",6)) c=0216;
      if (0==strncasecmp(iline,"format",6)) c=0242;

      while (alpha(*iline))
        ++iline;
    }
#endif

#ifdef APLFONT_NAME
#define XLT(a,x) if (c == a) c = x;

#ifdef _DYALOG_UTF8_H
    extern unsigned char* utf8_decode(unsigned char*, int*);
    {
        int old_c;
        unsigned char *new_iline;

        old_c = c;
        new_iline = utf8_decode(iline-1, &c);
        if (c != -1) {          /* decoded as UTF-8 */
#endif
	//XLT(C_OVERBAR,'`')
	XLT(C_UP_SHOE_JOT,0204)
	XLT(C_QUAD,'L')
	XLT(C_QUOTE_QUAD,0202)
	XLT(C_MULTIPLY,0225)
	XLT(C_DIVIDE,0226)
	XLT(C_STILE,'|')
	XLT(C_DOWN_STILE,'D')
	XLT(C_UP_STILE,'S')
	XLT(C_STAR,'P')
	XLT(C_CIRCLE_STAR,0207)
	XLT(C_CIRCLE,'O')
	XLT(C_QUOTE_DOT,0210)
	XLT(C_UP_CARET,'A')
	XLT(C_DOWN_CARET,'V')
	XLT(C_UP_CARET_TILDE,0205)
	XLT(C_DOWN_CARET_TILDE,0206)
	XLT(C_LT_EQUAL,0220)
	XLT(C_GT_EQUAL,0221)
	XLT(C_NOT_EQUAL,0222)
	XLT(C_TILDE,'T')
	XLT(C_QUERY,'Q')
	XLT(C_RHO,'R')
	XLT(C_IOTA,'!')
	XLT(C_EPSILON,'E')
	XLT(C_DOWN_TACK,'N')
	XLT(C_UP_TACK,'B')
	XLT(C_CIRCLE_BACKSLASH,0212)
	XLT(C_UP_ARROW,'Y')
	XLT(C_DOWN_ARROW,'U')
	XLT(C_LEFT_ARROW,'{')
	XLT(C_RIGHT_ARROW,'}')
	XLT(C_IBEAM,0213)
	XLT(C_QUAD_DIVIDE,0214)
	XLT(C_DELTA_STILE,0215)
	XLT(C_DEL_STILE,0216)
	XLT(C_UP_TACK_JOT,0241)
	XLT(C_DOWN_TACK_JOT,0242)
	XLT(C_SLASH_BAR,0200)
	XLT(C_BACKSLASH_BAR,0201)
	XLT(C_CIRCLE_STILE,0217)
	XLT(C_CIRCLE_BAR,0211)
	XLT(C_JOT,'J')
#ifdef _DYALOG_UTF8_H
        /* if char translated then accept new iline ptr */
        if (c != old_c)
          iline = new_iline;
        }
        else 
        { /* unknown UTF-8 sequence */
          c = old_c;
        }
    }
#endif
#endif

    /* my APL!, no delimiter, just keyword */
    if (alpha(c))
    { char *cp;
      cp = iline;
      while (alpha(*cp)) ++cp;
      if (!digit(*cp))
      {
		if (1 == cp-iline+1) if (0==strncasecmp(iline-1,"o}",2)) {c=0204;cp++;}
        if (2 == cp-iline+1) if (0==strncasecmp(iline-1,"in",2)) c='E';
        if (2 == cp-iline+1) if (0==strncasecmp(iline-1,"or",2)) c='V';
        if (2 == cp-iline+1) if (0==strncasecmp(iline-1,"up",2)) c=0215;
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"and",3)) c='A';
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"not",3)) c='T';
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"max",3)) c='S';
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"min",3)) c='D';
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"exp",3)) c='P';
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"log",3)) c=0207;
        if (3 == cp-iline+1) if (0==strncasecmp(iline-1,"nor",3)) c=0224;
        if (4 == cp-iline+1) if (0==strncasecmp(iline-1,"deal",4)) c='Q';
        if (4 == cp-iline+1) if (0==strncasecmp(iline-1,"nand",4)) c=0223;
        if (4 == cp-iline+1) if (0==strncasecmp(iline-1,"down",4)) c=0216;
        if (5 == cp-iline+1) if (0==strncasecmp(iline-1,"solve",5)) c=0214;
        if (5 == cp-iline+1) if (0==strncasecmp(iline-1,"outer",5)) c='J';
        if (6 == cp-iline+1) if (0==strncasecmp(iline-1,"circle",6)) c='O';
        if (6 == cp-iline+1) if (0==strncasecmp(iline-1,"choose",6)) c=0210;
        if (6 == cp-iline+1) if (0==strncasecmp(iline-1,"encode",6)) c='N';
        if (6 == cp-iline+1) if (0==strncasecmp(iline-1,"decode",6)) c='B';
        if (6 == cp-iline+1) if (0==strncasecmp(iline-1,"rotate",6)) c=0217;
        if (9 == cp-iline+1) if (0==strncasecmp(iline-1,"transpose",9)) c=0212;

        if (!alpha(c)) iline = cp;
      }
    }

    if(alpha(c)) return(getnam(c));

    if(digit(c) || (c=='.' && digit(*iline)) ||
       (c == C_OVERBAR && (digit(*iline) || *iline=='.'))) return(getnum(c));

    /* APL! symbols */
    if (c=='<' && *iline=='-') { c='{'; ++iline; };
    if (c=='-' && *iline=='>') { c='}'; ++iline; };
    if (c=='<' && *iline=='=') { c=0220; ++iline; };
    if (c=='>' && *iline=='=') { c=0221; ++iline; };
    if (c=='~' && *iline=='=') { c=0222; ++iline; };
    if (c=='`' && *iline=='@') { c=0202; ++iline; };
    if (c=='$') c=0241; /* execute */
    if (c=='#') c=0242; /* format */
    if (c=='I') c=0213; /* I-beam */
    if (c=='!') c='I';  /* iota */
    if (c=='?') c='R';  /* rho */
    if (c=='^') c='Y';  /* take */
    if (c=='~') c='U';  /* drop */
    if (c=='*') c='X';  /* times */
    if (c=='&') c='H';  /* delta */
    if (c=='@') c='L';  /* quad */

/* end of changes */

    c &= 0377;
    rval = unk;
    for(tp = tab; tp->input; tp++) {
        if(tp->input == c) {
            lv.charval = tp->lexval;
            rval = tp->retval;
            break;
        }
    }
                                    /*    If it's a comment, skip to the end
                                        of the line and return eol instead.  */
    if (lv.charval == COMNT) {
        while (1) {
            c = *iline++;
            if (c == '\n') {
                nlexsym = 0;
                return(eol);
            }
        }
    }

    if(lv.charval == QUAD) return(getquad());

    if(lv.charval == ISP){
        lv.charval = ISP2;
        xxpeek[0] = ISP1;
        xxpeek[1] = m;
        return(d);
    }

    if(lv.charval == PSI){
        lv.charval = PSI2;
        xxpeek[0] = PSI1;
        xxpeek[1] = m;
        return(d);
    }

    return(rval);
}

getquad()
{
    char c, *p1;
    struct qtab *p2;
    char qbuf[10];

    p1 = qbuf;
    while(alpha(*iline)) *p1++ = *iline++;
    *p1++ = 0;
    if(*qbuf == 0) return(quad);        /* ordinary quad */
    for(p2 = qtab; p2->qname; p2++){
        if(equal(p2->qname, qbuf)){
            lv.charval = p2->qtype;
            return(p2->rtype);
        }
    }
    return(unk);
}

getnam(ic)
{
    char name[NAMS], *cp;
    int c;
    struct nlist *np;

    c = ic;
    cp = name;
    do {
        if(cp >= &name[NAMS]) error("var name D");
        *cp++ = c;
        c = *iline++;
    } while(alpha(c) || digit(c));
    *cp++ = 0;
    iline--;
                                    /*    commands  */
    if(litflag == -1) {
        litflag = -2;
        for(c=0; comtab[c].ct_name; c++) {
            if(equal(name, comtab[c].ct_name)) break;
        }
        immedcmd = lv.charval = comtab[c].ct_ylval;
        return(comtab[c].ct_ytype);
    }
    for(np=nlist; np->namep; np++) {
        if(equal(np->namep, name)) {
            lv.charptr = (char *)np;
            switch(np->use) {
    
            case NF:
                if (context == lex2) sichk(np);
                return(nfun);

            case MF:
                if (context == lex2) sichk(np);
                return(mfun);

            case DF:
                if (context == lex2) sichk(np);
                return(dfun);
            }
            return(nam);
        }
    }
    np->namep = alloc(cp-name);
    copy(CH, name, np->namep, cp-name);
    np->type = LV;
    lv.charptr = (char *)np;
    return(nam);
}

getnum(ic)
{
    double d1, d2;
    int c, n, n1, s, s1;

    s = 0;
    n = 0;
    d1 = 0.;
    c = ic;
    if(c == C_OVERBAR) {                    /* '`' was '"' */
        s++;
        c = *iline++;
    }
    while(digit(c)) {
        d1 = d1*10. + c - '0';
        c = *iline++;
    }
    if(c == '.') {
        c = *iline++;
        while(digit(c)) {
            d1 = d1*10. + c - '0';
            c = *iline++;
            n--;
        }
    }
    if(c == 'e') {
        s1 = 0;
        n1 = 0;
        c = *iline++;
        if(c == C_OVERBAR) {                /* '`' was '"' */
            s1++;
            c = *iline++;
        }
        while(digit(c)) {
            n1 = n1*10 + c - '0';
            c = *iline++;
        }
        if(s1) n -= n1;
        else n += n1;
    }
    n1 = n;
    if(n1 < 0) n1 = -n1;
    d2 = 1.;
    while(n1--) d2 *= 10.;
    if(n < 0) d1 /= d2;
    else d1 *= d2;
    if(s) d1 = -d1;
    iline--;
    datum = d1;
    return(numb);
}

alpha(s)
{
    int c;

    c = s & 0377;
    return(
        (c >= 'a' && c <= 'z')
        || (c == 'F')
        || (c == '_')  /* added by William Chang */
        || (c == '&')  /* added by William Chang */
        // || (c >= 0243) /* 101004AP alternate character set Fa,... */
        || (litflag == -2 && (
               c == '/'
            || c == '.'
           ))
    );
}

digit(s)
{
    int c;

    c = s;
    if(c >='0' && c <= '9') return(1);
    return(0);
}

/*
 * s is statement
 * f is execution flag:
 *    0 compile immediate
 *    1 compile L
 *    2 function definition
 *    3 function prolog
 *    4 function epilog
 *    5 function body
 */
int     ilex[] =
{
    lex0, lex1, lex2, lex3, lex4, lex5, lex6
};


char *
compile(s, f)
char *s;
{
    char *p, *q;

    iline = s;
    ccharp = oline;
    litflag = 0;
    nlexsym = ilex[f];
    context = nlexsym;
    if(yyparse()) {
        pline(s, iline-s, lineNumber);
        if(iline-s > 1) {
			ttyerror();
			printf("syntax error\n");
		}
        return(0);
    }
    *ccharp++ = EOF;
    iline = p = alloc(ccharp-oline);
    for(q=oline; q<ccharp;) *p++ = *q++;
    return(iline);
}

yyerror()
{
}

                        /*    Byte-order dependency here?  */
                        /*    Potential alignment problem here?  */
char *
name(np, c)
{
    char *p, *npp;
    int temp;

    temp = np;
    npp = (char *)&temp;
    p = ccharp;
    *ccharp++ = c;
    *ccharp++ = *npp++;
    *ccharp++ = *npp++;
    *ccharp++ = *npp++;
    *ccharp++ = *npp++;
    return(p);
}

invert(char *a, char *b)
{
    flop(a, b);
    flop(b, ccharp);
    flop(a, ccharp);
}

flop(a, b)
char *a, *b;
{
    char *a1, *a2;
    int c;

    a1 = a;
    a2 = b;
    while(a1 < a2) {
        c = *a1;
        *a1++ = *--a2;
        *a2 = c;
    }
}

/*
 * genlab -- generates label code onto label stacks.
 *
 * prologue:    AUTO-lab, CONST-linenum, NAME-lab LABEL
 *
 * epilog:    REST-lab
 */
genlab(np)
struct nlist *np;
{
    /* label prologue */

    *labcpp++ = AUTO;
    labcpp += copy(IN, &np, labcpp, 1);
    *labcpp++ = CONST;
    *labcpp++ = 1;
    labcpp += copy(DA, &lnumb, labcpp, 1);
    *labcpp++ = NAME;
    labcpp += copy(IN, &np, labcpp, 1);
    *labcpp++ = LABEL;
    *labcpp = EOF;

    /* label epilog */

    *labcpe++ = REST;
    labcpe += copy(IN, &np, labcpe, 1);
    *labcpe = EOF;
}


int
isodigit(c)
int c;
{
    if (c < '0') return 0;
    if (c > '7') return 0;
    return 1;
}

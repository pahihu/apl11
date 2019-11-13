#include "apl.h"

ex_com0()
{
	fetch2();
	comk(0);
}

ex_comk()
{
	int k;

	k = topfix() - thread.iorg;
	fetch2();
	comk(k);
}

ex_com()
{
	struct item *q;

	fetch2();
	q = sp[-2];
	comk(q->rank-1);
}

#define EX_REPL	0

comk(k)
{
	struct item *p;
	data d;
	int i, dk, ndk, com1();
#if EX_REPL
	int n;
#endif

	p = sp[-1];
	bidx(sp[-2]);

	/* "getdat" returns the value of the data item which
	 * it is called to fetch.  If this is non-zero, just
	 * use the existing data on the stack (an example in
	 * APL would be "x/y" where x != 0.  If this is zero,
	 * the result is the null item, which is created by
	 * "newdat" and pushed on the stack.
	 */

	if(p->rank == 0 || (p->rank == 1 && p->size == 1)){
		if(getdat(p)) {
			pop();
			return;
		}
		p = newdat(idx.type, 1, 0);
		pop();
		pop();
		*sp++ = p;
		return;
	}

	if(idx.rank == 0 && p->rank == 1) {
		/* then scalar right arg ok */
		dk = p->dim[0];
		ndk = 0;
		for (i=0; i<dk; i++) {
			if(getdat(p)) ndk++;
		}
		p = newdat(idx.type, 1, ndk);
		d = getdat(sp[-2]);
		for(i =0; i<ndk; i++) putdat(p,d);
		pop();
		pop();
		*sp++ = p;
		return;
	}
	if(k < 0 || k >= idx.rank) error("compress X");
	dk = idx.dim[k];
	if(p->rank != 1 || p->size != dk) error("compress C");
	ndk = 0;
	for(i=0; i<dk; i++) {
#if EX_REPL
		if((n=getdat(p))<0) error("compress D");
		else ndk += n;
#else
		if(getdat(p)) ndk++;
#endif
	}
	p = newdat(idx.type, idx.rank, (dk==0) ? 0 : (idx.size/dk)*ndk);
	copy(IN, idx.dim, p->dim, idx.rank);
	p->dim[k] = ndk;
	*sp++ = p;
	forloop(com1, k);
	sp--;
	pop();
	pop();
	*sp++ = p;
}

com1(k)
{
#if EX_REPL
	struct item *p;
	int i, n;
	data d;

	p = sp[-2];
	p->index = idx.idx[k];
	if(n=getdat(p)) {
		p = sp[-3];
		p->index = access();
		d = getdat(p);
		for(i=0; i<n; i++) putdat(sp[-1], d);
	}
#else
	struct item *p;

	p = sp[-2];
	p->index = idx.idx[k];
	if(getdat(p)) {
		p = sp[-3];
		p->index = access();
		putdat(sp[-1], getdat(p));
	}
#endif
}

ex_exd0()
{
	fetch2();
	exdk(0);
}

ex_exdk()
{
	int k;

	k = topfix() - thread.iorg;
	fetch2();
	exdk(k);
}

ex_exd()
{
	struct item *q;

	fetch2();
	q = sp[-2];
	exdk(q->rank-1);
}

exdk(k)
{
	struct item *p;
	int i, dk;
	int exd1();

	p = sp[-1];
	bidx(sp[-2]);
	if(k < 0 || k >= idx.rank) error("expand X");
	dk = 0;
	for(i=0; i<p->size; i++) if(getdat(p)) dk++;
	if(p->rank != 1 || dk != idx.dim[k]) error("expand C");
	idx.dim[k] = p->size;
	size();
	p = newdat(idx.type, idx.rank, idx.size);
	copy(IN, idx.dim, p->dim, idx.rank);
	*sp++ = p;
	forloop(exd1, k);
	sp--;
	pop();
	pop();
	*sp++ = p;
}

exd1(k)
{
	struct item *p;

	p = sp[-2];
	p->index = idx.idx[k];
	if(getdat(p)) datum = getdat(sp[-3]);
	else if(idx.type == DA) datum = zero;
	else datum = ' ';
	putdat(sp[-1], datum);
}

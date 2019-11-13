#include "apl.h"

ex_take()
{
	int takezr();
	int i, k, o, fill[MRANK], fflg;

	/* While TANSTAAFL, in APL there is a close approximation.  It
	 * is possible to perform a "take" of more elements than an
	 * array actually contains (to be padded with zeros or blanks).
	 * If "td1()" detects that a dimension exceeds what the array
	 * actually contains it will return 1.  Special code is then
	 * required to force the extra elements in the new array to
	 * zero or blank.  This code is supposed to work for null items
	 * also, but it doesn't.
	 */

	o = 0;
	fflg = td1(0);
	for(i=0; i<idx.rank; i++) {
		fill[i] = 0;
		k = idx.idx[i];
		if(k < 0) {
			k = -k;
			if (k > idx.dim[i]) fill[i] = idx.dim[i] - k;
			o += idx.del[i] * (idx.dim[i] - k);
		}
		else {
			if (k > idx.dim[i]) fill[i] = idx.dim[i];
		}
		idx.dim[i] = k;
	}
	map(o);

	if (fflg){
		bidx(sp[-1]);
		forloop(takezr, fill);
	}
}

ex_drop()
{
	int i, k, o;

	o = 0;
	td1(1);
	for(i=0; i<idx.rank; i++) {
		k = idx.idx[i];
		if(k > 0) o += idx.del[i] * k;
		else k = -k;
		idx.dim[i] -= k;
	}
	map(o);
}

td1(tdmode)
{
	struct item *p, *q, *nq, *s2vect();
	int i, k;
	int r;					/* set to 1 if take > array dim */

	p = fetch2();
	q = sp[-2];
	r = !q->size;			/* Weird stuff for null items */
	if (q->rank == 0){		/* Extend scalars */
		nq = newdat(q->type, p->size, 1);
		*nq->datap = *q->datap;
		/* sp[-1] = p, sp[-2] = q ! */
#if 0
		pop();
		*sp++ = q = nq;
#else
		dealloc(sp[-2]); sp[-2] = q = nq;
#endif
		for(i=0; i<p->size; i++) q->dim[i] = 1;
	}
	if(p->rank > 1 || q->rank !=  p->size) error("take/drop C");
	bidx(q);
	for(i=0; i<p->size; i++) {
		k = fix(getdat(p));
		idx.idx[i] = k;
		if(k < 0) k = -k;

		/* If an attempt is made to drop more than what
		 * exists, modify the drop to drop exactly what
		 * exists.
		 */

		if(k > idx.dim[i]) {
			if (tdmode) idx.idx[i] = idx.dim[i];
			else r = 1;
		}
	}
	pop();
	return(r);
}

ex_dtrn()
{
	struct item *p, *q;
	int i;

	p = fetch2();
	q = sp[-2];
	if(p->rank > 1 || p->size != q->rank) error("tranpose C");
	for(i=0; i<p->size; i++) idx.idx[i] = fix(getdat(p)) - thread.iorg;
	pop();
	trn0();
}

ex_mtrn()
{
	struct item *p;
	int i;

	p = fetch1();
	if(p->rank <= 1) return;
	for(i=0; i<p->rank; i++) idx.idx[i] = p->rank-1-i;
	trn0();
}

trn0()
{
	int i, j;
	int d[MRANK], r[MRANK];

	bidx(sp[-1]);
	for(i=0; i<idx.rank; i++) d[i] = -1;
	for(i=0; i<idx.rank; i++) {
		j = idx.idx[i];
		if(j<0 || j>=idx.rank) error("tranpose X");
		if(d[j] != -1) {
			if(idx.dim[i] < d[j]) d[j] = idx.dim[i];
			r[j] += idx.del[i];
		}
		else {
			d[j] = idx.dim[i];
			r[j] = idx.del[i];
		}
	}
	j = idx.rank;
	for(i=0; i<idx.rank; i++) {
		if(d[i] != -1) {
			if(i > j) error("tranpose D");
			idx.dim[i] = d[i];
			idx.del[i] = r[i];
		}
		else if(i < j) j = i;
	}
	idx.rank = j;
	map(0);
}

ex_rev0()
{
	fetch1();
	revk(0);
}

ex_revk()
{
	int k;

	k = topfix() - thread.iorg;
	fetch1();
	revk(k);
}

ex_rev()
{
	struct item *p;

	p = fetch1();
	revk(p->rank-1);
}

revk(k)
{
	int o;

	bidx(sp[-1]);
	if(k < 0 || k >= idx.rank) error("reverse X");
	o = idx.del[k] * (idx.dim[k]-1);
	idx.del[k] = -idx.del[k];
	map(o);
}

map(o)
{
	struct item *p;
	int n, i;
	int map1();

	n = 1;
	for(i=0; i<idx.rank; i++) n *= idx.dim[i];
	if(n == 0) idx.rank == 0;
	p = newdat(idx.type, idx.rank, n);
	copy(IN, idx.dim, p->dim, idx.rank);
	*sp++ = p;
	if(n != 0) forloop(map1, o);
	sp--;
	pop();
	*sp++ = p;
}

map1(o)
{
	struct item *p;

	p = sp[-2];
	p->index = access() + o;
	putdat(sp[-1], getdat(p));
}

takezr(fill)
int *fill;
{
	struct item *p;
	int i;

	/* Zero appropriate elements of an array created by taking
	 * more than you originally had.  I apologize for the "dirty"
	 * argument passing (passing a pointer to an integer array
	 * through "forloop()" which treats it as an integer) and for
	 * the general dumbness of this code.
	 *					--John Bruner
	 */

	for(i=0; i<idx.rank; i++) {
		if (fill[i] > 0 && idx.idx[i] >= fill[i] || fill[i] < 0 && idx.idx[i] < -fill[i]){
			p = sp[-1];
			p->index = access();
			putdat(p, (p->type==DA) ? zero : (data)' ');
			return;
		}
	}
}

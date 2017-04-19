#include <math.h>
#include <stdio.h>

void dtob(int n)
{
    printf("%d\n", n);
    int c, k;
    for (c = 31; c >= 0; c--) {
	k = n >> c;
	if (k & 1)
	    printf("1");
	else
	    printf("0");
    }
    printf("\n");
}

int main()
{
    /*
	 * 10101
	 */
    int x = 64*64*64*64;
	dtob(x);
    x = (!!(x >> 16)) << 4; // if > 16?
	dtob(x);
    // based on previous x, if > (x + 8)
    x = x + ((!!(x >> (x + 8))) << 3);
	dtob(x);
    x = x + ((!!(x >> (x + 4))) << 2);
	dtob(x);
    x = x + ((!!(x >> (x + 2))) << 1);
	dtob(x);
    x = x + (!!(x >> (x + 1)));
	dtob(x);
}

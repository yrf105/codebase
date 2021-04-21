
#line 1 "test_foo_bar.rl"
#include <string.h>
#include <stdio.h>

#line 8 "test_foo_bar.rl"


#line 10 "test_foo_bar.rl.cc"
static const int foo_start = 1;
static const int foo_first_final = 6;
static const int foo_error = 0;

static const int foo_en_main = 1;


#line 10 "test_foo_bar.rl"


int main( int argc, char **argv )
{
	int cs, res = 0;
	if ( argc > 1 ) {
		char *p = argv[1];
		char *pe = p + strlen(p) + 1;
		
#line 28 "test_foo_bar.rl.cc"
	{
	cs = foo_start;
	}

#line 19 "test_foo_bar.rl"
		
#line 35 "test_foo_bar.rl.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 98: goto st2;
		case 102: goto st4;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 114 )
		goto tr4;
	goto st0;
tr4:
#line 7 "test_foo_bar.rl"
	{ res = 1; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 72 "test_foo_bar.rl.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 111 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 111 )
		goto tr4;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 20 "test_foo_bar.rl"
	}
	printf("result = %i\n", res );
	return 0;
}
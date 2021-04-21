
#line 1 "test_union.rl"
#include <iostream>
#include <string>


#line 14 "test_union.rl"



#line 12 "test_union.rl.cc"
static const int test_union_start = 1;
static const int test_union_first_final = 4;
static const int test_union_error = 0;

static const int test_union_en_main = 1;


#line 17 "test_union.rl"

int main(int argc, char** argv) {

    int cs, val = 0;

    std::string input;
    std::cin >> input;

    char* p = &input[0];
    char* pe = p + input.size() + 1;

    
#line 33 "test_union.rl.cc"
	{
	cs = test_union_start;
	}

#line 29 "test_union.rl"
    
#line 40 "test_union.rl.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 48 )
		goto tr0;
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr2;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st3;
	} else
		goto st3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 7 "test_union.rl"
	{ 
    val = val * 10 + ((*p) - '0');
}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 71 "test_union.rl.cc"
	switch( (*p) ) {
		case 88: goto st2;
		case 120: goto st2;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 7 "test_union.rl"
	{ 
    val = val * 10 + ((*p) - '0');
}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 89 "test_union.rl.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto st7;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st3;
	} else
		goto st3;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	goto st0;
	}
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 30 "test_union.rl"

    std::cout << val << std::endl;

    return 0;
}
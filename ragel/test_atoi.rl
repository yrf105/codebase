#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE_LINE_NORMAL 50

%%{
    machine atoi;
    write data;
}%%

long long ragel_atoi(char *str)
{
    char *p = str, *pe = str + strlen(str);
    // 当前状态
    int cs;
    long long val = 0;
    bool neg = false;

    %%{
        action see_neg {
            neg = true;
        }

        action add_digit { 
            val = val * 10 + (fc - '0');
        }
        # fc – The current character. This is equivalent to the expression (*p).
        main := ( '-'>see_neg | '+' )? ( digit @add_digit )+ '\n';

        # Initialize and execute.
        write init;
        write exec;
    }%%

    if ( neg )
        val = -1 * val;

    // name_first_final 变量存储第一个最终状态的ID。
    if ( cs < atoi_first_final )
        fprintf( stderr, "atoi: there was an error cs: %d\n", cs);

    return val;
};

int main(int argc, char *argv[])
{
    char buf[SIZE_LINE_NORMAL];

    while (fgets(buf, sizeof(buf), stdin) != 0) {
        long long value = ragel_atoi(buf);
        printf("%lld\n", value);
    }
    return 0;
}
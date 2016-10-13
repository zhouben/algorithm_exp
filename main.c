#include <stdio.h>

typedef union
{
    int value;
    struct
    {
        int en:1;
        int state:7;
        int length:8;
        int addr:16;
    };
} STATUS_REG;

int get_reg_value()
{
    STATUS_REG reg;
    
    reg.state = 0;
    reg.length = 120;
    reg.addr = 0x1234;
    return reg.value;
}

int main(int argc, char* argv[])
{
    int v;
    v = get_reg_value();
    printf("hello world! reg %08X", v.value);
    return 0;
}

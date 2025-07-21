
extern "C"
{

#include <stdio.h>
#include <string.h>
#include "LiteSHA1.h"
// implement __REV for x86
static uint32_t __REV(uint32_t value) {
    __asm {
        mov eax, value
        bswap eax
    }
}


// implement __ROR for x86
static uint32_t __ROR(uint32_t value, uint8_t shift) {
    __asm {
        mov eax, value
        mov cl, shift
        ror eax, cl
    }
}



int main()
{
    uint32_t a = 0x12345678;
    char key[] = "abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890";
    char out[32] = { 0 };
    //ws_handshake_response(key, out);
    //printf("%s\n%s\n%d\n", key, out, strlen(out));
    printf("%x\n%x\n%x\n", a, __ROR(a, 27), __REV(a));

    SHA1_CTX ctx;

    sha1_init(&ctx);

    // Compute hash and encode
    uint8_t sha1[32] = { 0 };
    sha1_encode(&ctx, (uint8_t*)key, strlen(key), sha1);
    for (int i = 0; i < 20; ++i)
        printf("%x", sha1[i]);
    printf("\n");

}

};
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

/*
 ============================================================================
 Name        : TTMS.c
 Author      : shu xinfeng
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include<math.h>
#include <windows.h>
#include <tchar.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"


//定义全局变量，用于存储登陆用户信息
account_t gl_CurUser={0, USR_ANOMY, "Anonymous",""};

float f(float x, float y, float z) {
    float a = x * x + 9.0f / 4.0f * y * y + z * z - 1;
    return a * a * a - x * x * z * z * z - 9.0f / 80.0f * y * y * z * z * z;
}

float h(float x, float z) {
	float y;
    for (y = 1.0f; y >= 0.0f; y -= 0.001f)
        if (f(x, y, z) <= 0.0f)
            return y;
    return 0.0f;
}
void jinru()
{

	printf("登");
	Sleep(200);
	printf("陆");
	Sleep(200);
	printf("成");
	Sleep(200);
	printf("功");
	Sleep(200);
	printf("，");
	Sleep(200);
	printf("欢");
	Sleep(200);
	printf("迎");
	Sleep(200);
	printf("您");
	Sleep(200);
	printf("...");
	Sleep(200);
	printf("...\n");
}
void denglu()
{
	system("cls");
	printf("正");
	Sleep(200);
	printf("在");
	Sleep(200);
	printf("登");
	Sleep(200);
	printf("录");
	Sleep(200);
	printf("，");
	Sleep(200);
	printf("请");
	Sleep(200);
	printf("稍");
	Sleep(200);
	printf("后");
	Sleep(200);
	printf("。。。\n");
	Sleep(200);
}

void love() {
	denglu();
    HANDLE o = GetStdHandle(STD_OUTPUT_HANDLE);
    _TCHAR buffer[25][80] = { _T(' ') };
    _TCHAR ramp[] = _T(".:-=+*#%@");
    float t,z,x;
    for (t = 0.0f;t<4; t += 0.1f) {
        int sy = 0;
        float s = sinf(t);
        float a = s * s * s * s * 0.2f;
        for (z = 1.3f; z > -1.2f; z -= 0.1f) {
            _TCHAR* p = &buffer[sy++][0];
            float tz = z * (1.2f - a);
            for (x = -1.5f; x < 1.5f; x += 0.05f) {
                float tx = x * (1.2f + a);
                float v = f(tx, 0.0f, tz);
                if (v <= 0.0f) {
                    float y0 = h(tx, tz);
                    float ny = 0.01f;
                    float nx = h(tx + ny, tz) - y0;
                    float nz = h(tx, tz + ny) - y0;
                    float nd = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
                    float d = (nx + ny - nz) * nd * 0.5f + 0.5f;
                    *p++ = ramp[(int)(d * 5.0f)];
                }
                else
                    *p++ = ' ';
            }
        }

        for (sy = 0; sy < 25; sy++) {
            COORD coord = { 0, sy };
            SetConsoleCursorPosition(o, coord);
            WriteConsole(o, buffer[sy], 79, NULL, 0);
        }
        Sleep(30);
    }
    printf("\n");
    jinru();
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	int i=2,k=0;
	//system("cls");
	do{
		system("cls");
		if(!SysLogin()) {
			printf("\n\t\t\t对不起,您的用户名或密码错误，您还有%d次机会......\n",i);
			getchar();
			//system("cls");
			k++;
			i--;
			if(k==3){
			printf("\t\t\t您无权登录本系统,请按任意键退出");
			getchar();
			exit(0);
			}
		}
		else
			break;
	}while(k!=3);

	love();
	Main_Menu();
	return EXIT_SUCCESS;
}

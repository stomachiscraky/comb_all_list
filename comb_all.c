#define _CRT_SECURE_NO_WARNINGS //Visual Studio only
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tbl[100][100] = { "A","B", "C", "D", "E" }; //初期値はサンプル用
int ary[100]; //添字0はフラグ。組み合わせは添字1から始まります。
char delim[10] = ","; //区切り文字列。
int para_n = 0;
int para_r_min = 0;
int para_r_max = 0;
int total_cnt = 0;
int count_option = 0;
int count_only_option = 0;
int suspend_option = 0;

//配列の値を出力します。
void pr(int size)
{
    total_cnt++;
    if (suspend_option) return;
    if (size == 0) {
        printf("#null!");
    }
    else {
        for (int i = 1; i <= size; i++) {
            if (i != 1) printf("%s", delim);
            printf("%s", tbl[ary[i] - 1]);
        }
    }
    printf("\n");
}

/*----
組み合わせを得ます。
最適化オプションを使用してください。
おおまかに2倍早くなります。
Visual Studio の場合は /O2 です。
スクリプトや Basic の場合は tmp は外した方が良いです。
for 文の中の処理が重くなりかえって遅くなります。
----*/
void comb(int n, int r)
{
    if (r == 0) {
        pr(0);
        return;
    }
    for (int i = 0; i <= r; i++) {
        ary[i] = i;
    }
    while (ary[0] == 0) {
        pr(r);
        for (int i = r; i >= 0; i--) {
            int tmp = ary[i] + 1;
            ary[i] = tmp;
            if (tmp <= n - r + i) {
                for (int j = i + 1; j <= r; j++) {
                    ary[j] = ++tmp;
                }
                break;
            }
        }
    }
}

int main(int argc, char **argv)
{
    int data_cnt = 0;
    int data_exist = 0;
    int n_option = 0;
    int r_min_option = 0;
    int r_max_option = 0;
    char *bufp;

    if (argc == 1) {
         delim[0] = ',';
        delim[1] = '\0';
        para_n = 5;
        para_r_min = 1;
        para_r_max = 5;
        printf("サンプルで実行します。\n");
    }
    else {
        argc--;
        while (argc--) {
            argv++;
            char buf[100];
            strcpy(buf, *argv);
            if (memcmp(buf, "-n:", 3) == 0) {
                para_n = strtol(buf + 3, &bufp, 10);
                if (bufp == buf + 3) {
                    printf("-n: 数値がありません。\n");
                    exit(1);
                }
                n_option = 1;
            }
            else if (memcmp(buf, "-r:", 3) == 0 || memcmp(buf, "-k:", 3) == 0) {
                para_r_max = strtol(buf + 3, &bufp, 10);
                if (bufp == buf + 3) {
                    printf("-r: -k: 数値がありません。\n");
                    exit(1);
                }
                para_r_min = para_r_max;
                r_min_option = 1;
                r_max_option = 1;
            }
            else if (memcmp(buf, "-rmin:", 6) == 0 || memcmp(buf, "-kmin:", 6) == 0) {
                para_r_min = strtol(buf + 6, &bufp, 10);
                if (bufp == buf + 6) {
                    printf("-rmin: -kmin: 数値がありません。\n");
                    exit(1);
                }
                r_min_option = 1;
            }
            else if (memcmp(buf, "-rmax:", 6) == 0 || memcmp(buf, "-kmax:", 6) == 0) {
                para_r_max = strtol(buf + 6, &bufp, 10);
                if (bufp == buf + 6) {
                    printf("-rmax: -kmax: 数値がありません。\n");
                    exit(1);
                }
                r_max_option = 1;
            }
            else if (memcmp(buf, "-d-", 4) == 0) {
                delim[0] = '\0';
            }
            else if (memcmp(buf, "-ds", 4) == 0) {
                delim[0] = 0x20;
                delim[1] = '\0';
            }
            else if (memcmp(buf, "-d:", 3) == 0) {
                if (buf[3] == '\0') {
                    printf("-d: 文字列がありません。\n");
                    exit(1);
                }
                else {
                    strcpy(delim, buf + 3);
                }
            }
            else if (memcmp(buf, "-c", 3) == 0) {
                count_option = 1;
            }
            else if (memcmp(buf, "-co", 4) == 0) {
                count_only_option = 1;
                suspend_option = 1;
            }
            else if (memcmp(buf, "-s", 3) == 0) {
                suspend_option = 1;
            }
            else if (*buf == '-') {
                printf("該当するオプションはありません。\n");
                exit(1);
            }
            else {
                strcpy(tbl[data_cnt], *argv);
                data_exist = 1;
                data_cnt++;
            }
        }
    }
    if (data_exist && n_option) {
        printf("データと -n: オプションは同時に指定できません。\n");
        exit(1);
    }
    if (data_exist) {
        para_n = data_cnt;
    }
    if (n_option) {
        for (int i = 1; i <= para_n; i++) {
            sprintf(tbl[i - 1], "%d", i);
        }
    }
    if (!r_min_option) {
        para_r_min = 1;
    }
    if (!r_max_option) {
        para_r_max = para_n;
    }
    if (para_n < 0 || para_r_min < 0 || para_r_max < 0
        || para_r_min > para_n || para_r_max > para_n) {
        printf("nは0以上でなければなりません。\n");
        printf("rは0以上かつrはn以下でなければなりません。\n");
        exit(1);
    }
    if (para_r_min > para_r_max) {
        printf("rmin が rmax より大きいです。\n");
        exit(1);
    }
    for (int i = para_r_min; i <= para_r_max; i++) {
        comb(para_n, i);
    }
    if (count_only_option) {
        printf("%d", total_cnt);
    }
    else if (count_option) {
        printf("パターンの総計 %d\n", total_cnt);
    }
    return 0;
}

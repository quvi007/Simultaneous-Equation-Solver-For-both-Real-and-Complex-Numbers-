/* Coded by Mehbubul Hasan Al Quvi
BUET CSE
Section A
Batch 18
Student ID: 1805007
Program: Dynamic Equation Solver
Supports equations of up to 10 variables
and Complex Numbers as co-efficients and constants
Parent: String to Complex / Real Number Parser by the same developer
Version: 4.0
Date: 21st November, 2019

Feel free to provide feedback or report bugs at
facebook.com/quvi.ninad
quvi5052@gmail.com
github.com/quvi007
*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>
typedef long long ll;
#define MAX_C 300

typedef struct
{
    double re, im;
} CMPLX_NUM;

//Function Prototypes Start

CMPLX_NUM mulC(CMPLX_NUM, CMPLX_NUM);
CMPLX_NUM addC(CMPLX_NUM, CMPLX_NUM);
CMPLX_NUM divC(CMPLX_NUM, CMPLX_NUM);
CMPLX_NUM det(CMPLX_NUM **, int);
CMPLX_NUM cmplx_conj(CMPLX_NUM);
double mod_sq(CMPLX_NUM);
int isNum(char);
int contains_i(char *, char);
ll str_len(char *);
int digitCount(ll);
ll EXP_10(int);
ll strToInt(char *);
double d_abs(double);
double StrToDouble(char *);
char* sub_str(char *, int);
void str_cat(char *, char *);
void str_catc(char *, char);
double parseOneTerm(char *);
double parse(char *);
char* eval(char *);
char** str_tok(char *, char);
CMPLX_NUM parseOneComplexTerm(char *);
CMPLX_NUM parseComplex(char *);
char* ComplexXYtoRTheta(CMPLX_NUM);

//Function Prototypes End

char* ComplexXYtoRTheta(CMPLX_NUM c){
    double arg, mod;
    mod = sqrt(c.re * c.re + c.im * c.im);
    if(c.re > 0 && c.im >= 0) arg = atan(c.im / c.re) * 180 / acos(-1);
    else if(c.re < 0 && c.im >= 0) arg = 180 - atan(d_abs(c.im / c.re)) * 180 / acos(-1);
    else if(c.re < 0 && c.im < 0) arg = -180 + atan(d_abs(c.im / c.re)) * 180 / acos(-1);
    else if(c.re > 0 && c.im < 0) arg = -atan(d_abs(c.im / c.re)) * 180 / acos(-1);
    else if(c.im == 0) arg = 0;
    else if(c.re == 0 && c.im > 0) arg = 90;
    else if(c.re == 0 && c.im < 0) arg = 90;
    char *str = malloc(MAX_C);
    sprintf(str, "%lf<%lf", mod, arg);
    return str;
}

double d_abs(double d)
{
    if(d > 0)
        return d;
    return (-1.0 * d);
}

char** str_tok(char *str, char ch)
{
    int n = contains_i(str, ch) + 1;
    char **str_tokd = malloc(n * sizeof(char*));
    for(int i = 0; i < n; ++i)
    {
        str_tokd[i] = malloc(MAX_C);
    }
    int j = 0, k = 0;
    while(*str)
    {
        if(*str == ch)
        {
            str_tokd[j][k] = 0;
            j++;
            k = 0;
        }
        else
        {
            str_tokd[j][k++] = *str;
        }
        str++;
    }
    str_tokd[j][k] = 0;
    return str_tokd;
}

int isNum(char ch)
{
    return (ch >= '0' && ch <= '9');
}
int contains_i(char *str, char t)
{
    int c = 0;
    while(*str)
    {
        char ch = *str++;
        if(ch == t)
            c++;
    }
    return c;
}
ll str_len(char *str)
{
    ll c = 0LL;
    while(*str)
        c++, str++;
    return c;
}
int digitCount(ll n)
{
    if(!n)
        return 1;
    int x = 0;
    while(n)
    {
        x++;
        n /= 10LL;
    }
    return x;
}
ll EXP_10(int n)
{
    ll ans = 1LL;
    for(int i = 0; i < n; ++i)
        ans *= 10LL;
    return ans;
}
ll strToInt(char *str)
{
    int Sign = 1;
    if(*str == '+')
        Sign = 1, str++;
    else if(*str == '-')
        Sign = -1, str++;
    ll ans = 0LL;
    while(*str)
        ans = ans * 10LL + *str++ - '0';
    return Sign * ans;
}
double strToDouble(char *str)
{
    int Sign = 1;
    if(*str == '+')
        Sign = 1, str++;
    else if(*str == '-')
        Sign = -1, str++;
    char flag = 0;
    double ans = 0;
    ll div = 10LL;
    while(*str)
    {
        if(*str == '.')
        {
            flag = 1;
            str++;
            continue;
        }
        if(flag == 1)
        {
            ans += ((*str++ - '0') * 1.0) / div;
            div *= 10LL;
        }
        else if(flag == 0)
            ans = ans * 10 + *str++ - '0';
    }
    return (Sign * ans);
}
CMPLX_NUM strToComplex(char *str)
{
    if(contains_i(str, '<'))
    {
        CMPLX_NUM C;
        char **vals = str_tok(str, '<');
        double mod = strToComplex(vals[0]).re;
        double arg = strToComplex(vals[1]).re * acos(-1) / 180.0;
        C.re = mod * cos(arg);
        C.im = mod * sin(arg);
        free(*(vals));
        free(*(vals + 1));
        free(vals);
        return C;
    }

    if(contains_i(str, '{'))
    {
        char new_str[MAX_C] = "";
        if(str[0] != '{')
            str_cat(new_str, str + 2);
        else
            str_cat(new_str, str + 1);
        new_str[str_len(new_str) - 1] = 0;
        CMPLX_NUM C;
        char **vals = str_tok(new_str, ',');
        C.re = strToDouble(vals[0]);
        C.im = strToDouble(vals[1]);
        free(*(vals));
        free(*(vals + 1));
        free(vals);
        CMPLX_NUM Sign = (CMPLX_NUM)
        {
            1, 0
        };
        if(str[0] == '-')
            Sign = (CMPLX_NUM)
        {
            -1, 0
        };
        return mulC(C, Sign);
    }
    if(!contains_i(str, 'i') && !contains_i(str, 'j'))
        return (CMPLX_NUM)
    {
        strToDouble(str), 0
    };
    if(str[0] == 'i' || str[0] == 'j')
    {
        if(str_len(str) == 1)
            return (CMPLX_NUM)
        {
            0, 1
        };
        return (CMPLX_NUM)
        {
            0, strToDouble(str + 1)
        };
    }
    else if(str[str_len(str) - 1] == 'i' || str[str_len(str) - 1] == 'j')
    {
        if(str_len(str) > 2)
            return (CMPLX_NUM)
        {
            0, strToDouble(sub_str(str, str_len(str) - 1))
        };
        if(str[0] == '-')
            return (CMPLX_NUM)
        {
            0, -1
        };
        return (CMPLX_NUM)
        {
            0, strToDouble(sub_str(str, str_len(str) - 1))
        };
    }
    else if((str[1] == 'i' || str[1] == 'j') && str_len(str) > 2)
    {
        char new_str[MAX_C] = "";
        str_cat(new_str, str);
        new_str[1] = '0';
        return (CMPLX_NUM)
        {
            0, strToDouble(new_str)
        };
    }


}
char* sub_str(char *str, int len)
{
    char *rStr = malloc(len + 1);
    char *p = rStr;
    int i = 0;
    while(*str && i < len)
    {
        *p++ = *str++;
        i++;
    }
    *p = 0;
    return rStr;
}
void str_cat(char *str1, char *str2)
{
    char *p = str1;
    while(*p)
        p++;
    while(*str2)
        *p++ = *str2++;
    *p = 0;
}
void str_catc(char *str, char ch)
{
    int l = str_len(str);
    *(str + l) = ch;
    *(str + l + 1) = 0;
}
char* eval(char *str)
{
    static int call_no = 0;
    if(!call_no)
    {
        if(str[0] == '(' && str[str_len(str) - 1] == ')' && contains_i(str, '(') == 1 && contains_i(str, ')') == 1)
        {
            str++;
            str = sub_str(str, str_len(str) - 1);
        }
        call_no = 1;
    }
    double ans = 0;
    if((contains_i(str, '(') == 1 && contains_i(str, ')') == 1 && str[0] == '(' && str[str_len(str) - 1] == ')') || (!contains_i(str, '(') && !contains_i(str, ')')))
    {
        char *str_ans = malloc(MAX_C);
        CMPLX_NUM parsedCMPLX = parseComplex(str);
        sprintf(str_ans, "{%lf,%lf}", parsedCMPLX.re, parsedCMPLX.im);
        return str_ans;
    }
    char *str_build = malloc(MAX_C);
    *str_build = 0;
    while(*str)
    {
        char ch = *str;
        if(ch != '(' && ch != ')')
        {
            str_catc(str_build, ch);
        }
        else if(ch == '(')
        {
            int idx = 0;
            int lc = 0, rc = 0;
            char *cp = str;
            while(*cp)
            {
                char ch2 = *cp;
                if(ch2 == '(')
                    lc++;
                else if(ch2 == ')')
                {
                    rc++;
                    if(rc == lc)
                        break;
                }
                cp++;
                idx++;
            }
            char new_str[MAX_C] = "";
            str_cat(new_str, sub_str(str + 1, idx - 1));
            int t_flag = 0;
            if(isNum(*(str - 1)) || *(str - 1) == 'i' || *(str - 1) == 'j' || *(str - 1) == ')')
                str_catc(str_build, '*');
            if(contains_i(new_str, '(') > 1)
            {
                str_catc(str_build, ch);
                t_flag = 1;
            }
            str += idx;
            char *returned_str = eval(new_str);
            str_cat(str_build, returned_str);
            if(t_flag)
            {
                str_catc(str_build, ')');
            }
            if(isNum(*(str + 1)) || *(str + 1) == 'i' || *(str + 1) == 'j' || *(str + 1) == '(')
                str_catc(str_build, '*');
        }
        str++;
    }
    return eval(str_build);
}

CMPLX_NUM parseOneComplexTerm(char *str)
{
    char *tp = str;
    char str_build[MAX_C] = "";
    while(*tp)
    {
        if(*tp == '/')
        {
            int idx = 0;
            str_catc(str_build, '*');
            char *tp2 = tp + 1;
            while(*tp2)
            {
                if(!isNum(*tp2) && *tp2 != '+' && *tp2 != '-' && *tp2 != 'i' && *tp2 != 'j' && *tp2 != '{' && *tp2 != ',' && *tp2 != '}' && *tp2 != '.' && *tp2 != '<')
                    break;
                idx++;
                tp2++;
            }
            char tmp_str[MAX_C] = "";
            CMPLX_NUM tCmplxNum = divC((CMPLX_NUM)
            {
                1, 0
            }, strToComplex(sub_str(tp + 1, idx)));
            sprintf(tmp_str, "{%lf,%lf}", tCmplxNum.re, tCmplxNum.im);
            str_cat(str_build, tmp_str);
            tp += idx;
        }
        else
        {
            str_catc(str_build, *tp);
        }
        tp++;
    }
    str = str_build;
    CMPLX_NUM ans = (CMPLX_NUM)
    {
        0, 0
    };
    if(contains_i(str, '*'))
    {
        char **vals;
        CMPLX_NUM valr;
        CMPLX_NUM ans_n = (CMPLX_NUM)
        {
            1, 0
        };
        vals = str_tok(str, '*');
        int n = contains_i(str, '*') + 1;
        for(int i = 0; i < n; ++i)
        {
            valr = strToComplex(*(vals + i));
            ans_n = mulC(ans_n, valr);
        }
        for(int i = 0; i < n; ++i)
            free(*(vals + i));
        free(vals);
        ans = addC(ans, ans_n);
    }
    else
        ans = addC(ans, strToComplex(str));
    return ans;
}

CMPLX_NUM parseComplex(char *str)
{
    CMPLX_NUM ans = (CMPLX_NUM)
    {
        0, 0
    };
    char str_tokd[MAX_C][MAX_C];
    int j = 0, k = 0, to_add = 1;
    for(int i = 0; i < str_len(str); ++i)
    {
        if(str[i] == '(' || str[i] == ')')
            continue;
        if(str[i] == '+' && (isNum(str[i - 1]) || str[i - 1] == 'i' || str[i - 1] == 'j' || str[i - 1] == '}'))
        {
            str_tokd[j][k] = 0;
            CMPLX_NUM parsed_cmplx = parseOneComplexTerm(str_tokd[j++]);
            ans.re += to_add * parsed_cmplx.re;
            ans.im += to_add * parsed_cmplx.im;
            to_add = 1;
            k = 0;
        }
        else if(str[i] == '-' && (isNum(str[i - 1]) || str[i - 1] == 'i' || str[i - 1] == 'j' || str[i - 1] == '}'))
        {
            str_tokd[j][k] = 0;
            CMPLX_NUM parsed_cmplx = parseOneComplexTerm(str_tokd[j++]);
            ans.re += to_add * parsed_cmplx.re;
            ans.im += to_add * parsed_cmplx.im;
            to_add = -1;
            k = 0;
        }
        else
            str_tokd[j][k++] = str[i];
    }
    str_tokd[j][k] = 0;
    CMPLX_NUM parsed_cmplx = parseOneComplexTerm(str_tokd[j++]);
    ans.re += to_add * parsed_cmplx.re;
    ans.im += to_add * parsed_cmplx.im;
    return ans;
}
CMPLX_NUM cmplx_conj(CMPLX_NUM c)
{
    return (CMPLX_NUM)
    {
        c.re, -c.im
    };
}
double mod_sq(CMPLX_NUM c)
{
    return (c.re * c.re + c.im * c.im);
}
CMPLX_NUM mulC(CMPLX_NUM c1, CMPLX_NUM c2)
{
    return (CMPLX_NUM)
    {
        c1.re * c2.re - c1.im * c2.im, c1.re * c2.im + c2.re * c1.im
    };
}
CMPLX_NUM addC(CMPLX_NUM c1, CMPLX_NUM c2)
{
    return (CMPLX_NUM)
    {
        c1.re + c2.re, c1.im + c2.im
    };
}
CMPLX_NUM divC(CMPLX_NUM c1, CMPLX_NUM c2)
{
    CMPLX_NUM q = mulC(c1, cmplx_conj(c2));
    double divisor = mod_sq(c2);
    q.re /= divisor;
    q.im /= divisor;
    return q;
}

CMPLX_NUM det(CMPLX_NUM **mat, int m)
{
    if(m == 1)
        return **mat;
    CMPLX_NUM ans = {0, 0};
    for(int k = 0; k < m; ++k)
    {
        CMPLX_NUM **new_mat = malloc((m-1) * sizeof(CMPLX_NUM*));
        int r = 0;
        for(int x = 1; x < m; ++x)
        {
            int c = 0;
            *(new_mat + r) = malloc((m-1) * sizeof(CMPLX_NUM));
            for(int y = 0; y < m; ++y)
            {
                if(y == k)
                    continue;
                *(*(new_mat + r) + c++) = *(*(mat + x) + y);
            }
            r++;
        }
        ans = addC(ans, mulC(mulC(*(*mat + k), (CMPLX_NUM)
        {
            ((k % 2) ? -1 : 1), 0
        }), det(new_mat, m - 1)));
    }
    return ans;
}
int main()
{
    system("cls");
    int m;
    printf("Number of Variables: ");
    scanf("%d", &m);
    char var_names[m][MAX_C];
    printf("Enter the names of these %d variables (each separated using a space):\n", m);
    for(int i = 0; i < m; ++i)
        scanf("%s", var_names[i]);
    printf("\nEnter the co-efficients of the variables sequentially and the constant (each line for one equation):\n");

    for(int i = 0; i < m; ++i)
        printf("%s\t", var_names[i]);
    printf("Constant\n");
    CMPLX_NUM main_mat[m][m+1];
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < m + 1; ++j)
        {
            char str[MAX_C * 7];
            scanf("%s", str);
            main_mat[i][j] = strToComplex(eval(str));
        }
    }
    CMPLX_NUM Dn[m];
    CMPLX_NUM D;
    CMPLX_NUM **mat;
    mat = malloc(m * sizeof(CMPLX_NUM*));
    for(int i = 0; i < m; ++i)
    {
        *(mat + i) = malloc(m * sizeof(CMPLX_NUM));
        for(int j = 0; j < m; ++j)
        {
            mat[i][j] = main_mat[i][j];
        }
    }
    D = det(mat, m);
    int inf = 0;
    if(D.re == 0 && D.im == 0) inf = 1;
    free(mat);
    printf("\n\n");
    for(int k = 0; k < m; ++k)
    {
        mat = malloc(m * sizeof(CMPLX_NUM*));
        for(int i = 0; i < m; ++i)
        {
            *(mat + i) = malloc(m * sizeof(CMPLX_NUM));
            for(int j = 0; j < m; ++j)
            {
                if(j == k)
                    *(*(mat + i) + j) = *(*(main_mat + i) + m);
                else *(*(mat + i) + j) = (CMPLX_NUM)*(*(main_mat + i) + j);
            }
        }
        if(inf == 1){
            printf("Infinite Solutions!\n");
            goto jump;
        }
        Dn[k] = det(mat, m);
        Dn[k] = mulC(Dn[k], cmplx_conj(D));
        Dn[k].re /= mod_sq(D);
        Dn[k].im /= mod_sq(D);
        for(int i = 0; i < m; ++i)
            free(*(mat + i));
        free(mat);
        char *rt = ComplexXYtoRTheta(Dn[k]);
        if(Dn[k].re != 0 && Dn[k].im != 0)
        {
            if(Dn[k].im > 0)
                printf("%s : %.3lf + %.3lfi ;\tr-theta: %s\n", var_names[k], Dn[k].re, Dn[k].im, rt);
            else if(Dn[k].im < 0)
                printf("%s : %.3lf - %.3lfi ;\tr-theta: %s\n", var_names[k], Dn[k].re, d_abs(Dn[k].im), rt);
        }
        else if(Dn[k].re == 0 && Dn[k].im == 0)
            printf("%s : 0 ;\t\tr-theta: %s\n", var_names[k], rt);
        else if(Dn[k].re == 0)
            printf("%s : %.3lfi ;\tr-theta: %s\n", var_names[k], Dn[k].im, rt);
        else
            printf("%s : %.3lf ;\tr-theta: %s\n", var_names[k], Dn[k].re, rt);
        free(rt);
    }
    jump:
    printf("\n\nPress any key to solve again\n");
    getch();
    return main();
}

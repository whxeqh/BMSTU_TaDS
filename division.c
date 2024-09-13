#include "division.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#define MAX(A, B) (A > B ? A : B)

static void delete_first_zeros(bdouble_t *num, const size_t len)
{
    for (size_t i = len; i < num->man_length; i++)
        num->mantissa[i - len] = num->mantissa[i];
    for (size_t i = num->man_length - len; i < num->man_length + len; i++)
        num->mantissa[i] = 0;
}

/** 
 * @brief Умножение длинного числа на короткое
 * 
 * Алгоритм с emaxx
 * 
 * @param big мантисса длинного числа
 * @param big_len длина мантиссы
 * @param digit короткое число
 * @param base основание системы счисления
*/
static void mul_big_small(bdouble_t *big, const int digit, const size_t base)
{
    reverse_mantissa(big->mantissa, big->man_length);
    int carry = 0;
    for (size_t i = 0; i < big->man_length || carry; ++i)
    {
        if (i == big->man_length)
            (big->man_length)++;
        int cur = carry + big->mantissa[i] * digit;
        big->mantissa[i] = cur % base;
        carry = cur / base;
    }

    int i = big->man_length- 1;
    while(big->mantissa[i] == 0)
        i--;

    reverse_mantissa(big->mantissa, big->man_length);
}

static int cmp_mantissa(int *man1, const size_t len1, int *man2, const size_t len2)
{
    int ans = 0;
    reverse_mantissa(man1, len1);
    reverse_mantissa(man2, len2);

    for (int i = MAX(len1, len2) - 1; i >= 0; --i)
        if (man1[i] != man2[i])
        {
            ans = man1[i] - man2[i];
            break;
        }

    reverse_mantissa(man1, len1);
    reverse_mantissa(man2, len2);

    return ans;
}

/** 
 * @brief Нахождение частного неполного делимого и делителя
 * 
 * Алгоритм с методички
 * 
 * @param len Длина неполного делимого
 * @param divident Делитель
 * @param divisor Делимое
 * @return Неполное делимое / (10 ^ (k - 1)), где k - длина делителя (мантиссы)
*/
static size_t incomplete_dividend(size_t *len, bdouble_t *divident, const bdouble_t *divisor, exp_t *exp)
{
    static short cnt = 0;
    cnt++;

    size_t digit = 0, i = 0;
    size_t divisor_len = divisor->man_length;
    size_t divident_len = divisor_len;
    size_t old_divident_len = divident->man_length;

    if (divident->man_length < divisor_len)
    {
        //divident->exponent -= (divisor_len - divident_len);
        divident->man_length += (divisor_len - divident->man_length);
    }


    while(i < divisor_len)
    {
        if (divident->mantissa[i] < divisor->mantissa[i])
        {
            divident_len++;
            if (divident->man_length == divisor->man_length)
                divident->man_length++;
            break;
        }
        else if (divident->mantissa[i] > divisor->mantissa[i])
            break;

        ++i;
    }

    *len = divident_len;

    if (exp->flag == false && cnt == 1 && *len > old_divident_len)
    {
        exp->exp -=(*len - (old_divident_len + 1));
        exp->flag = true;
    }
    else if (exp->flag == false)
    {
        if (cnt > 0 && *len == old_divident_len)
        {
            exp->exp = cnt;
            exp->flag = true;
        }
        else if (cnt > 1)
            exp->exp = (cnt - 1);
    }

    if (divident_len > divisor_len)
        return digit = (divident->mantissa[0] * 10 + divident->mantissa[1]) / divisor->mantissa[0];
    else 
        return digit = divident->mantissa[0] / divisor->mantissa[0];
}

/**
 * @brief Нахождение вычитаемого и очередную цифру частного
 * @param subtrahend Вычитаемое
 * @param sub_len Длина вычитаемого
 * @param divident Делимое
 * @param divisor Делитель
 * @return очередная цифра частного
*/
static int get_sub(size_t *len, bdouble_t *subtrahend, bdouble_t *divident,  const bdouble_t *divisor, exp_t *exp)
{
    //size_t divident_len = divident->man_length;
    size_t divisor_len = divisor->man_length;

    bdouble_t tmp_divisor;   //Для умножения на цифру
    memset(subtrahend->mantissa, 0, sizeof(int) * MANTISA_LEN);
    memset(&tmp_divisor, 0, sizeof(int) * MANTISA_LEN);

    tmp_divisor.man_length = divisor_len;

    memcpy(tmp_divisor.mantissa, divisor->mantissa, divisor_len * sizeof(int));

    size_t digit = incomplete_dividend(len, divident, divisor, exp);

    mul_big_small(&tmp_divisor, digit, BASE);

    while (cmp_mantissa(divident->mantissa, *len, tmp_divisor.mantissa, tmp_divisor.man_length) < 0)
    {
        digit--;
        memcpy(&tmp_divisor, divisor, sizeof(bdouble_t));
        mul_big_small(&tmp_divisor, digit, BASE);
    }

    subtrahend->man_length = tmp_divisor.man_length;
    memcpy(subtrahend->mantissa, &tmp_divisor.mantissa, tmp_divisor.man_length * sizeof(int));

    return digit; // Для ответа каждая цифра
}

// Первое число не меньше второго (a >= b)
/** 
 * @brief Функция вычитания
 * @param tmp_divident весь делитель
 * @param len длина неполного делителя
 * @param subtrahend вычитаемое
 * @param sub_len длина вычитаемого
 * @param base основание системы счисления
*/
static void subtraction(bdouble_t *tmp_divident, int len, bdouble_t *subtrahend, size_t *ostatok, const size_t base)
{
    int sub_len = subtrahend->man_length;
    reverse_mantissa(subtrahend->mantissa, sub_len);
    reverse_mantissa(tmp_divident->mantissa, len);

    int carry = 0;
    for (int i = 0; i < sub_len || carry; ++i)
    {
        tmp_divident->mantissa[i] -= carry + (i < len ? subtrahend->mantissa[i] : 0);
      carry = tmp_divident->mantissa[i] < 0;
      if (carry)
            tmp_divident->mantissa[i] += base;
    }

    reverse_mantissa(tmp_divident->mantissa, len);

    int i = 0;
    while(i < len && tmp_divident->mantissa[i] == 0)
        i++;
    delete_first_zeros(tmp_divident, i);

    tmp_divident->man_length -= i;

    *ostatok = len - i;
}


bdouble_t div_big_numbers(const bdouble_t *divident, bdouble_t *divisor)
{
    exp_t exp, fake_exp;
    size_t ostatok = 0, len;
    bdouble_t ostatok_t;
    bdouble_t ans, subtrahend, tmp_divident = *divident, all_zeros;
    memset(&all_zeros, 0, sizeof(bdouble_t));
    memset(&ans, 0, sizeof(bdouble_t));
    memset(&exp, 0, sizeof(exp_t));

    ans.exponent = -1 * divisor->exponent + divisor->man_length;

    do
    {
        //incomplete_dividend(&tmp_divident, divisor);
        memset(&subtrahend, 0, sizeof(bdouble_t));
        memset(&ostatok_t, 0, sizeof(bdouble_t));

        ans.mantissa[ans.man_length++] = get_sub(&len, &subtrahend, &tmp_divident, divisor, &exp);

        subtraction(&tmp_divident, len, &subtrahend, &ostatok, BASE);
    

        ostatok++;

        while(ostatok < divisor->man_length)
        {
            if (exp.flag == false && tmp_divident.man_length < divisor->man_length)
                ans.exponent++;
            ostatok++;
            ans.mantissa[ans.man_length++] = 0;
        }

        memcpy(&ostatok_t.mantissa, &tmp_divident.mantissa, ostatok * sizeof(int));

        if (ostatok == divisor->man_length && cmp_mantissa(ostatok_t.mantissa, ostatok, divisor->mantissa, ostatok) < 0 && cmp_mantissa(tmp_divident.mantissa, MANTISA_LEN, all_zeros.mantissa, MANTISA_LEN) != 0)
        {
            if (exp.flag == false)
                ans.exponent++;
            ans.mantissa[ans.man_length++] = 0;
        }

    } while (cmp_mantissa(tmp_divident.mantissa, MANTISA_LEN, all_zeros.mantissa, MANTISA_LEN) != 0 && ans.man_length < MANTISA_LEN);

    size_t i = ans.man_length - 1;
    while(ans.mantissa[i] == 0)
    {
        ans.man_length--;
        ans.exponent--;
        i--;
    }

    if (ans.man_length == MANTISA_LEN)
    {
        memset(&subtrahend, 0, sizeof(bdouble_t));
        memset(&ostatok_t, 0, sizeof(bdouble_t));
        memset(&fake_exp, 0, sizeof(exp_t));

        int digit = get_sub(&len, &subtrahend, &tmp_divident, divisor, &fake_exp);
        if (digit >= 5)
            ans.mantissa[MANTISA_LEN - 1]++;
    }

    ans.exponent += exp.exp;

    if (ans.man_length > MANTISA_LEN)
    {
        if (ans.mantissa[MANTISA_LEN] >= 5)
            ans.mantissa[MANTISA_LEN - 1]++;
        ans.man_length--;
    }

    if ((divident->sign == '+' && divisor->sign == '-') || (divident->sign == '-' && divisor->sign == '+'))
        ans.sign = '-';
    else
        ans.sign = '+';

    return ans;
}
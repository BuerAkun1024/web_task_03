#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <cstring>
using namespace std;
#define  NCARDS 52
#define  NSUITS 4
char values[] = "23456789TJQKA";
char types[]  = "CDHS";
char white[] = "White wins.";
char black[] = "Black wins.";
char tie  [] = "Tie.";
//输入扑克当前值和类型
//返回扑克牌加权大小
int rank_card(char value , char type)
{
    int i,j;
    for (i = 0 ; i < NCARDS; ++i )
    {
        if (value == values[i])
        {
            for (j = 0 ; j < NSUITS ; ++j)
            {
                if (type == types[j])
                {
                    return (i*NSUITS+j);// i*4+j
                }
            }
        }
    }
    fprintf(stderr,"bad card\n");
    return -1;
}
//获取牌面的类型
char card_type(int card)
{
    return types[card%4];
}
//获取牌面的数值
char card_value(int card)
{
    return values[card/4];
}
bool is_order(int arr[])
{
    char str[6] = "";
    int i = 0;
    for( ; i < 5 ; i++)
        str[i] = card_value(arr[i]);
    char* rc = search(values,values+13,str,str+5);//若找到相应的位置，则返回开始出现的位置
    return (rc!=values+13);
}
bool is_same_type(int arr[])
{
    int i = 1;
    for(; i < 5 ; i ++)
    {
        if(card_type(arr[i]) != card_type(arr[i-1]))
            return false;
    }
    return true;
}
int  get_level_type(int arr[])
{
    char ch = card_value(arr[0]);//获取牌面值
    int level_type = 0;
    int i ;
    int nsame= 1;
    int ndouble = 0 , nthree = 0 , nfour = 0;
    for(i = 1; i < 5 ; ++i)  //判断循环中相同牌面值的排数 知道有几个对子 几个三条 几个铁支
    {
        if(ch == card_value(arr[i]))
        {
            nsame++;//相同牌面值的个数
        }
        else
        {
            ch = card_value(arr[i]);
            if(nsame == 2)
            {
                ndouble++;
            }
            else if(nsame == 3)
            {
                nthree++;
            }
            else if (nsame == 4)
            {
                nfour++;
            }
            nsame = 1;
        }
    }
    if(nsame == 2)
    {
        ndouble++;
    }
    else if(nsame == 3)
    {
        nthree++;
    }
    else if (nsame == 4)
    {
        nfour++;
    }
    assert(!(nthree&&nfour));//条件为真则停止运行
    assert(ndouble < 3);
    if(nfour)
    {
        level_type = (1<<6);          //铁支：64
        return level_type;
    }
    if(ndouble == 2)//双对
    {
        level_type  = ndouble;        //双对：2
        return level_type;
    }
    else if(ndouble ==1)//一对
    {
        //printf("yi dui \n");
        level_type = ndouble;        //一对：1
    }
    if(nthree)//三张
    {
        level_type |= (1<<2);        //三条：4
        if(ndouble ==1)
        {
            level_type |= (1<<5);    //葫芦：32
        }
        return level_type;
    }
    if(ndouble)		//如果 有一对 则不用检查 顺子和同花；
        return level_type;


    if(is_same_type(arr))           //同花：16
    {
        level_type = (1<<4);
    }
    if (is_order(arr))              //顺子：8
    {
        level_type |= (1<<3);
    }
    if( (level_type & (1<<4))  && (level_type & (1<<3)) )//进行按位与，判断同花顺
    {
        level_type |= (1<<7);       //同花顺：128
    }
    return level_type;
}
int same_level_cmp(int arr1[],int level_type1 , int arr2[],int level_type2)
{
    int i = 0;
    int arr1_1,arr1_2,arr2_1,arr2_2;//检测双对
    if( !level_type1  || (level_type1&(1 << 4)) || (level_type1&(1<<3)))  //同花 顺子 最大牌 归为一类 ， 选择两幅牌的最大牌
    {
        for(i = 4 ; i >=0 ; i--)
        {
            if (card_value(arr1[i]) != card_value(arr2[i]))
            {
                return arr2[i]-arr1[i];
            }
        }
    }
    if( (level_type1&(1<<6)) || (level_type1&(1<<2)))  //四张 三张（包括葫芦）  归为一类  只要检测五张牌的中间那张
    {
        printf("duo zhang");
        return arr2[2] - arr1[2];
    }
    if( (level_type1&(1<<1)) ) //双对
    {
        printf("shuang dui\n");
        arr1_1 = arr1[1];
        arr1_2 = arr1[3];
        if(arr1_1 > arr1_2) swap(arr1_1,arr1_2);

        arr2_1 = arr2[1];
        arr2_2 = arr2[3];
        if (arr2_1 > arr2_2) swap(arr2_1,arr2_2);

        if(arr1_2 != arr2_2) return arr2_2 - arr1_2;
        else				 return arr2_1 - arr1_1;

    }
    if( (level_type1&(1)) )//一对
    {
        printf("yi dui\n");
        for (i = 1 ; i < 5 ; i ++)
        {
            if (card_value(arr1[i]) == card_value(arr1[i-1]))
            {
                arr1_1 = arr1[i];
                break;
            }
        }
        for (i = 1; i < 5 ; i ++)
        {
            if(card_value(arr2[i]) == card_value(arr2[i-1]))
            {
                arr2_1 = arr2[i];
                break;
            }
        }
        if(arr1_1 != arr2_1) return arr1_1 - arr2_1;
        for (i = 4 ; i >= 0 ; i--)
        {
            if(card_value(arr1[i]) != card_value(arr2[i]))
                return arr2[i] - arr1[i];
        }
    }



    return 0;
}
int get_result(int arr1[] , int arr2[])
{
    int level_type1 = 0 , level_type2 = 0;
    sort(arr1,arr1+5);
    sort(arr2,arr2+5);
    level_type1 = get_level_type(arr1);
    level_type2 = get_level_type(arr2);
    if (level_type1 > level_type2)
    {
        return -1;
    }
    else if(level_type1 < level_type2)
    {
        return 1;
    }
    return  same_level_cmp( arr1,level_type1 ,  arr2, level_type2);
}
int main()
{
    char str[1000];
    char *p = NULL;
    int i = 0;
    int result = 0;
    memset(str,0,1000);  //初始化str数组
    int array[10] = {0};
    while(gets(str))
    {
        i = 0;
        p = strtok(str," ");
        while (p)
        {
            array[i++] = rank_card(p[0],p[1]);//将牌面的加权值存到array数组中
            p = strtok(NULL," ");
        }
        //	result =get_result(array,array+5);
        if ( ( result=get_result(array,array+5)) < 0 )
        {
            printf("%s\n",black);
        }
        else if(result > 0)
        {
            printf("%s\n",white);
        }
        else
        {
            printf("%s\n",tie);
        }
        memset(str,0,1000);

    }
    return 0;
}
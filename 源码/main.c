#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attack_phy_armor.h"
#include "attack_element_armor.h"
#include "affinity_armor.h"
#include "damage_armor_phy.h"
#include "damage_armor_element.h"

//only calculate weapon final phy attack
//存储不同种类技能
int armor_phy_skill_low[14]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int armor_phy_skill_high[14]={7,5,5,3,3,5,5,3,3,3,3,3,3,3};    
int armor_elem_skill_low[4]={-1,-1,-1,-1};
int armor_elem_skill_high[4]={5,4,4,4};
int armor_affi_skill_low[3]={-1,-1,-1};
int armor_affi_skill_high[3]={7,3,3};
int armor_dmg_skill_low[3]={-1,-1,-1};
int armor_dmg_skill_high[3]={3,3,3};
//存储武器的面板数据
double weapon_attack;
double sharpness[3]={0,0,0};    //紫，白，蓝斩的消耗比例:0.5,0.5,0; 1,0,0; 0,0.8,0.2; ...
double weapon_affinity;
double weapon_element[2]={0,0};   //第0位表示武器属性 0：无， 1：火， 2：水， 3：冰， 4：雷， 5：龙；第1位是具体属性值
double action_element=1;   //初始化的武器攻击的平均属性补正
double action=1;            //初始化计算所需动作值
//初始化受击的平均肉质和平均属性吸收
double hit_phy=0,hit_elem=0;
//初始化计算所需各数值
double add_atk_phy=0;       //纯物理攻击的加算乘算
double multi_atk_phy=1;     // 
double add_atk_elem=0;      //纯属性攻击的加算乘算
double multi_atk_elem=1;    //
double add_atk_p_e_phy=0;   //物理+属性攻击的物，属加算乘算
double multi_atk_p_e_phy=1; //
double add_atk_p_e_elem=0;   //
double multi_atk_p_e_elem=1; //
double add_affi=0;          //纯会心的加算
double add_atk_p_a=0;       //物理+会心的攻击的加算乘算和会心加算
double multi_atk_p_a=1;     //
double add_affi_p_a=0;      //
double crit_phy=1.25;       //物理伤害的会心初始倍率
double crit_elem=1;         //会心击属性伤害的会心初始倍率
double crit_elem_weak=1;    //属性弱特的会心初始倍率
//存储对应各个技能的覆盖率
double armor_phy_skill_k[14];
double armor_elem_skill_k[4];
double armor_affi_skill_k[3];
double armor_dmg_skill_k[3];

//上限480000组物理攻击力相关技能的技能组合0-7，8，9为计算得到物理攻击力乘算，加算
//攻击，怨恨，死里逃生，攻击守势，火场怪力，龙气活性，巧击，偷袭
double result_phy_atk[480000][10];
int count_phy_atk=0;
int count_phy_atk_max=480000;
int overflow_phy_atk=0;   //设定超过时置1并警告

//上限1000组属性攻击力相关技能组合0-3，4，5为计算得到属性攻击力乘算，加算
//相应属性强化，钢壳的恩惠，炎鳞的恩惠，风雷合一
double result_elem_atk[1000][6];
int count_elem_atk=0;
int count_elem_atk_max=1000;
int overflow_elem_atk=0;   //设定超过时置1并警告

//上限2000组物理+属性攻击力相关技能组合0-4，5，6，7，8为计算得到物理，属性攻击力乘算，加算
//伏魔耗命蓝（3虫），业铠修罗红，因祸得福，狂龙病，连击
double result_phy_elem_atk[2000][9];
int count_phy_elem_atk=0;
int count_phy_elem_atk_max=2000;
int overflow_phy_elem_atk=0;   //设定超过时置1并警告

//上限200组会心相关技能组合0-2，3为计算得到会心率
//看破，弱点特效，精神抖擞
double result_affi[200][4];
int count_affi=0;
int count_affi_max=200;
int overflow_affi=0;   //设定超过时置1并警告

//上限10组攻击力+会心相关技能组合0，1，2，3为计算得到攻击力乘算，加算，会心率加算
//挑战者
double result_atk_affi[10][4];
int count_atk_affi=0;
int count_atk_affi_max=10;
int overflow_atk_affi=0;   //设定超过时置1并警告

//上限80组伤害补正相关技能组合0-2，3,4,5为计算得到物理，属性伤害1,属性伤害2乘算
//超会心，会心击属性 弱特属性
double result_dmg[80][6];
int count_dmg=0;
int count_dmg_max=80;
int overflow_dmg=0;   //设定超过时置1并警告


//上限为10000的最终伤害结果存储
double result_final[10000][25];     //14+4+3+3=24技能0-23，24计算伤害
int count_final=0;
int count_final_max=10000;
int overflow_final=0;   //设定超过时置1并警告

//函数声明
void cal_phy_atk();             //物理
void restore_phy_atk_result();
void cal_elem_atk();            //属性
void restore_elem_atk_result();
void cal_phy_elem_atk();        //物理+属性
void restore_phy_elem_atk_result();
void cal_affi();                //会心
void restore_affi_result();
void cal_atk_affi();            //攻击+会心
void restore_atk_affi_result();
void cal_dmg();               //伤害补正
void restore_dmg_result();
void cal_final();               //最终处理
void restore_final_result();

void read();                    //读入数据
void fliter();             //筛选处于伤害区间的数据
void sort();                    //数据排序
void putout();                  //向putout.csv输出结果

void read()
{
    int line_count=0;
    int k=0;
    const char* delim = ",";
    FILE *fp = NULL;
    fp = fopen("putin.csv", "r");
    if (fp != NULL)
	printf("csv file open\n");
    else printf("csv file open failed\n");
    
    char *s = (char*)malloc(1024);
    char *p = (char*)malloc(1024);

    while(!feof(fp))
    {
        fgets(s,1024,fp);
        line_count++;
        //printf("i :%d\n",line_count);

        if(line_count == 2)             //取武器面板攻击力
        {
            weapon_attack = atof(s);    //字符串转浮点
            //printf("weapon atk:%lf\n",weapon_attack);
        }

        if(line_count == 4)             //取武器面板会心
        {
            weapon_affinity = atof(s);    //字符串转浮点
            //printf("weapon affi:%lf\n",weapon_affinity);
        }

        if(line_count == 6)             //取斩味消耗比例
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 3) 
            {
                sharpness[k] = atof(p);//字符串转浮点
                //printf("sharp[%d]=%lf\n",k, sharpness[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 8)             //取动作值
        {
            action = atof(s);    //记录计算式使用的动作值
            //printf("action: %lf\n",s,action);
        }

        if(line_count == 10)             //面板属性数据
        {
            k=0;
            int elem_check=0,elem_data=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 5) 
            {
                elem_data = atoi(p);//字符串转整型
                //printf("armor_phy_skill_low[%d]=%d\n",k, armor_phy_skill_low[k]);
                if (elem_data != 0)
                {
                    weapon_element[0]=k+1;
                    weapon_element[1]=elem_data;
                    elem_check = 1;
                }
                p = strtok(NULL, delim);
                k++;
            }
            if(elem_check == 0)     //无属性情况
            {
                weapon_element[0]=0;
                weapon_element[1]=0;
            }
            //printf("element: %lf %lf\n",weapon_element[0],weapon_element[1]);
        }

        if(line_count == 12)             //平均武器招式属性补正
        {
            action_element = atof(s);    //记录攻击动作的平均属性补正
            //printf("action element: %lf\n",action_element);
        }

        if(line_count == 16)             //平均受击肉质，平均受击属性吸收
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            hit_phy = atof(p);
            p = strtok(NULL, delim);
            hit_elem = atof(p);
            //printf("hit: %lf %lf\n",hit_phy,hit_elem);
        }

        if(line_count == 20)             //取物理攻击力相关技能
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 14) 
            {
                armor_phy_skill_low[k] = atoi(p);//字符串转整型
                //printf("armor_phy_skill_low[%d]=%d ",k, armor_phy_skill_low[k]);
                p = strtok(NULL, delim);
                armor_phy_skill_high[k] = atoi(p);//字符串转整型
                //printf("armor_phy_skill_high[%d]=%d\n",k, armor_phy_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_phy_skill_low[k]<0)
                armor_phy_skill_high[k]=0,armor_phy_skill_low[k]=0;
                k++;
            }
        }

        if(line_count == 22)            //取物理攻击力相关技能的覆盖率
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 14) 
            {
                armor_phy_skill_k[k] = atof(p);//字符串转浮点
                //printf("armor_phy_skill_k[%d]=%lf\n",k, armor_phy_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 24)            //取属性攻击力相关技能，注意部分技能的控制由物理攻击力部分负责
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 4) 
            {
                armor_elem_skill_low[k] = atoi(p);//字符串转整型
                //printf("armor_elem_skill_low[%d]=%d ",k, armor_elem_skill_low[k]);
                p = strtok(NULL, delim);
                armor_elem_skill_high[k] = atoi(p);//字符串转整型
                //printf("armor_elem_skill_high[%d]=%d\n",k, armor_elem_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_elem_skill_low[k]<0)
                armor_elem_skill_low[k]=0,armor_elem_skill_high[k]=0;
                k++;
            }
        }
    
        if(line_count == 26)            //记录属性攻击力相关技能的覆盖率
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            while (p != NULL && k < 4) 
            {
                armor_elem_skill_k[k] = atof(p);//字符串转浮点
                //printf("armor_elem_skill_k[%d]=%d\n",k, armor_elem_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 28)            //记录会心率相关技能
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 3) 
            {
                armor_affi_skill_low[k] = atoi(p);//字符串转整型
                //printf("armor_affi_skill_low[%d]=%d\n",k, armor_affi_skill_low[k]);
                p = strtok(NULL, delim);
                armor_affi_skill_high[k] = atoi(p);//字符串转整型
                //printf("armor_affi_skill_high[%d]=%d\n",k, armor_affi_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_affi_skill_low[k] < 0)
                armor_affi_skill_low[k] = 0 , armor_affi_skill_high[k] =0;
                k++;
            }
        }

        if(line_count == 30)            //记录会心率相关技能的覆盖率
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_affi_skill_k[k] = atof(p);//字符串转浮点
                //printf("armor_affi_skill_k[%d]=%d\n",k, armor_affi_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 32) //记录伤害补正技能
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_dmg_skill_low[k] = atoi(p);//字符串转整型
                //printf("armor_dmg_skill_low[%d]=%d\n",k, armor_dmg_skill_low[k]);
                p = strtok(NULL, delim);
                armor_dmg_skill_high[k] = atoi(p);//字符串转整型
                //printf("armor_dmg_skill_high[%d]=%d\n",k, armor_dmg_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_dmg_skill_low[k] < 0)
                armor_dmg_skill_low[k] = 0, armor_dmg_skill_high[k] = 0;
                k++;
            }
        }

        if(line_count == 34) //记录伤害补正技能覆盖率
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_dmg_skill_k[k] = atof(p);//字符串转整型
                //printf("armor_dmg_skill_k[%d]=%d\n",k, armor_dmg_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

    }
}
void restore_phy_atk_result(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7)
{
    if (i0<0)i0=0; 
    if (i1<0)i1=0; 
    if (i2<0)i2=0; 
    if (i3<0)i3=0; 
    if (i4<0)i4=0; 
    if (i5<0)i5=0; 
    if (i6<0)i6=0; 
    if (i7<0)i7=0;
    result_phy_atk[count_phy_atk][0]=i0;
    result_phy_atk[count_phy_atk][1]=i1;
    result_phy_atk[count_phy_atk][2]=i2;
    result_phy_atk[count_phy_atk][3]=i3;
    result_phy_atk[count_phy_atk][4]=i4;
    result_phy_atk[count_phy_atk][5]=i5;
    result_phy_atk[count_phy_atk][6]=i6;
    result_phy_atk[count_phy_atk][7]=i7;
    result_phy_atk[count_phy_atk][8]=multi_atk_phy;
    result_phy_atk[count_phy_atk][9]=add_atk_phy;
    count_phy_atk++;
    if(count_phy_atk>=count_phy_atk_max) overflow_phy_atk = 1;
}
void cal_phy_atk()
{
    /*
    攻击,怨恨,死里逃生,攻击守势,火场怪力,龙气活性,巧击,偷袭
    */
    //printf("%d",armor_phy_skill_low[16]);
    
    int i0,i1,i2,i3,i4,i5,i6,i7;
    for(i0=armor_phy_skill_high[0];i0>=armor_phy_skill_low[0]&&i0>=0;i0--)   //攻击
    {
        //printf("011\n");
        if(armor_phy_skill_low[0]>=0)
        {
            add_atk_phy = add_atk_phy + Attack_Boost_add[i0];
            multi_atk_phy = multi_atk_phy * Attack_Boost_multi[i0];
        }
        else
        {
            add_atk_phy = add_atk_phy;
            multi_atk_phy = multi_atk_phy;
            i0 = 0;
        }
        for(i1=armor_phy_skill_high[2];i1>=armor_phy_skill_low[2]&&i1>=0;i1--)   //怨恨
        {
            //printf("012\n");
            if(armor_phy_skill_low[2]>=0)
            add_atk_phy = add_atk_phy + Resentment_add[i1] * armor_phy_skill_k[2];
            else add_atk_phy = add_atk_phy,i1=0;
            for(i2=armor_phy_skill_high[3];i2>=armor_phy_skill_low[3]&&i2>=0;i2--)   //死里逃生
            {
                //printf("013\n");
                if(armor_phy_skill_low[3]>=0)
                add_atk_phy = add_atk_phy + Resuscitate_add[i2] * armor_phy_skill_k[3];
                else add_atk_phy = add_atk_phy,i2=0;
                for(i3=armor_phy_skill_high[4];i3>=armor_phy_skill_low[4]&&i3>=0;i3--)   //攻击守势
                {
                    //printf("014\n");
                    if(armor_phy_skill_low[4]>=0)
                    multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[4]+Offensive_Guard_multi[i3] * armor_phy_skill_k[4]);
                    else multi_atk_phy = multi_atk_phy,i3=0;
                    for(i4=armor_phy_skill_high[5];i4>=armor_phy_skill_low[5]&&i4>=0;i4--)   //火场怪力
                    {
                        //printf("015\n");
                        if(armor_phy_skill_low[5]>=0)
                        multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[5]+Heroics_multi[i4] * armor_phy_skill_k[5]);
                        else multi_atk_phy = multi_atk_phy,i4=0;
                        for(i5=armor_phy_skill_high[6];i5>=armor_phy_skill_low[6]&&i5>=0;i5--)   //龙气活性
                        {
                            //printf("016\n");
                            if(armor_phy_skill_low[6]>=0)
                            multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[6]+Dragonheart_multi[i5] * armor_phy_skill_k[6]);
                            else multi_atk_phy = multi_atk_phy,i5=0;
                            for(i6=armor_phy_skill_high[12];i6>=armor_phy_skill_low[12]&&i6>=0;i6--)    //巧击
                            {
                                if(armor_phy_skill_low[12]>=0)
                                add_atk_phy = add_atk_phy + Adrenaline_Rush[i6] * armor_phy_skill_k[12];
                                else add_atk_phy = add_atk_phy,i6=0;
                                for(i7=armor_phy_skill_high[13];i7>=armor_phy_skill_low[13]&&i7>=0;i7--)    //偷袭
                                {
                                    if(armor_phy_skill_low[13]>=0)
                                    multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[13] + Sneak_Attack[i7] * armor_phy_skill_k[13]);
                                    else multi_atk_phy = multi_atk_phy,i7=0;
                                    restore_phy_atk_result(i0,i1,i2,i3,i4,i5,i6,i7);
                                    if(overflow_phy_atk == 1)
                                    {
                                        printf("01 Too many requseted skills, please limit your requirement! \n");
                                        add_atk_phy=0;
                                        multi_atk_phy=1;
                                        return;
                                    }
                                    else
                                    {
                                        //printf("armor physical attack calculating:...%d... multi:%lf add:%lf\n",count_phy_atk,multi_atk_phy,add_atk_phy);
                                    }
                                    if(armor_phy_skill_low[13]>=0)
                                    multi_atk_phy = multi_atk_phy / (1-armor_phy_skill_k[13] + Sneak_Attack[i7] * armor_phy_skill_k[13]);
                                    else multi_atk_phy = multi_atk_phy;
                                }
                                if(armor_phy_skill_low[12]>=0)
                                add_atk_phy = add_atk_phy - Adrenaline_Rush[i6] * armor_phy_skill_k[12];
                                else add_atk_phy = add_atk_phy;
                            }
                            if(armor_phy_skill_low[6]>=0)
                            multi_atk_phy = multi_atk_phy / (1-armor_phy_skill_k[6]+Dragonheart_multi[i5] * armor_phy_skill_k[6]);
                            else multi_atk_phy = multi_atk_phy;
                        }
                        if(armor_phy_skill_low[5]>=0)
                        multi_atk_phy = multi_atk_phy / (1-armor_phy_skill_k[5]+Heroics_multi[i4] * armor_phy_skill_k[5]);
                        else multi_atk_phy = multi_atk_phy;
                    }
                    if(armor_phy_skill_low[4]>=0)
                    multi_atk_phy = multi_atk_phy / (1-armor_phy_skill_k[4]+Offensive_Guard_multi[i3] * armor_phy_skill_k[4]);
                    else multi_atk_phy = multi_atk_phy;
                }
                if(armor_phy_skill_low[3]>=0)
                add_atk_phy = add_atk_phy - Resuscitate_add[i2] * armor_phy_skill_k[3];
                else add_atk_phy = add_atk_phy;
            }        
            if(armor_phy_skill_low[2]>=0)
            add_atk_phy = add_atk_phy - Resentment_add[i1] * armor_phy_skill_k[2];
            else add_atk_phy = add_atk_phy;
        }     
        if(armor_phy_skill_low[0]>=0)
        {
            add_atk_phy = add_atk_phy - Attack_Boost_add[i0];
            multi_atk_phy = multi_atk_phy / Attack_Boost_multi[i0];
        }
        else
        {
            add_atk_phy = add_atk_phy;
            multi_atk_phy = multi_atk_phy;
        }
    }
    //printf("%d %d %d",i0,armor_phy_skill_low[0],i0>=armor_phy_skill_low[0]);
    return;
}
void restore_elem_atk_result(int check,int i0,int i1,int i2,int i3)
{
    if (i0<0)i0=0; 
    if (i1<0)i1=0; 
    if (i2<0)i2=0; 
    if (i3<0)i3=0;  
    int i,j;
    if(check == 0)      //无属性
    {
        count_elem_atk = 0;
    }
    else                //有属性
    {
        result_elem_atk[count_elem_atk][0]=i0;
        result_elem_atk[count_elem_atk][1]=i1;
        result_elem_atk[count_elem_atk][2]=i2;
        result_elem_atk[count_elem_atk][3]=i3;
        result_elem_atk[count_elem_atk][4]=multi_atk_elem;
        result_elem_atk[count_elem_atk][5]=add_atk_elem;
        
        //printf("%lf %lf\n",result_elem_atk[count_elem_atk][4],result_elem_atk[count_elem_atk][5]);

        if(count_elem_atk>=count_elem_atk_max) overflow_elem_atk = 1;
        count_elem_atk++;
    }
}
void cal_elem_atk()
{
    /*
    对应属性攻击强化，钢龙的恩惠，炎鳞的恩惠，风雷合一
    */
    
    int i0,i1,i2,i3;
    if( weapon_element[0] == 0) 
    {
        restore_elem_atk_result(0,i0,i1,i2,i3);
        return;
    }
    for(i0=armor_elem_skill_high[0];i0>=armor_elem_skill_low[0]&&i0>=0;i0--)   //对应属性攻击强化
    {
        if(armor_elem_skill_low[0]>=0)
        {
            add_atk_elem = add_atk_elem + Element_Attack_add[i0];
            multi_atk_elem = multi_atk_elem * Element_Attack_multi[i0];
        }
        else
        {
            add_atk_elem = add_atk_elem;
            multi_atk_elem = multi_atk_elem;
            i0 = 0;
        }
        //printf("%lf,%lf  \n",add_atk_elem,multi_atk_elem);
        for(i1=armor_elem_skill_high[1];i1>=armor_elem_skill_low[1]&&i1>=0;i1--)   //钢壳的恩惠
        {
            if(armor_elem_skill_low[1]>=0) 
            multi_atk_elem = multi_atk_elem * (1-armor_elem_skill_k[1]+Kushala_Blessing_multi[i1] * armor_elem_skill_k[1]);
            else multi_atk_elem = multi_atk_elem,i1=0;
            for(i2=armor_elem_skill_high[2];i2>=armor_elem_skill_low[2]&&i2>=0;i2--)   //炎鳞的恩惠
            {
                if(armor_elem_skill_low[2]>=0)
                multi_atk_elem = multi_atk_elem * (1-armor_elem_skill_k[2]+Teostra_Blessing_multi[i2] * armor_elem_skill_k[2]);
                else multi_atk_elem = multi_atk_elem,i2=0;
                for(i3=armor_elem_skill_high[3];i3>=armor_elem_skill_low[3]&&i3>=0;i3--)   //风雷合一
                {
                    if(armor_elem_skill_low[3]>=0)
                    multi_atk_elem = multi_atk_elem * (1-armor_elem_skill_k[3]+Stormsou_multi[i3] * armor_elem_skill_k[3]);
                    
                    restore_elem_atk_result(1,i0,i1,i2,i3);
                    if(overflow_elem_atk == 1)
                    {
                        printf("02 Too many requseted element skills, please limit your requirement! \n");
                        add_atk_elem=0;
                        multi_atk_elem=1;
                        return;
                    }
                    else
                    {
                        //printf("armor element attack calculating:...%d... multi:%lf add:%lf \n",count_elem_atk,result_elem_atk[count_elem_atk-1][4],result_elem_atk[count_elem_atk-1][5]);
                    }    
                    if(armor_elem_skill_low[3]>=0)
                    multi_atk_elem = multi_atk_elem / (1-armor_elem_skill_k[3]+Stormsou_multi[i3] * armor_elem_skill_k[3]);
                    else multi_atk_elem = multi_atk_elem;
                }
                if(armor_elem_skill_low[2]>=0)
                multi_atk_elem = multi_atk_elem / (1-armor_elem_skill_k[2]+Teostra_Blessing_multi[i2] * armor_elem_skill_k[2]);
                else multi_atk_elem = multi_atk_elem;
            }
            if(armor_elem_skill_low[1]>=0) 
            multi_atk_elem = multi_atk_elem / (1-armor_elem_skill_k[1]+Kushala_Blessing_multi[i1] * armor_elem_skill_k[1]);
            else multi_atk_elem = multi_atk_elem;
        }
        if(armor_elem_skill_low[0]>=0)
        {
            add_atk_elem = add_atk_elem - Element_Attack_add[i0];
            multi_atk_elem = multi_atk_elem / Element_Attack_multi[i0];
        }
        else
        {
            add_atk_elem = add_atk_elem;
            multi_atk_elem = multi_atk_elem;
        }
        //printf("%lf,%lf  \n",add_atk_elem,multi_atk_elem);
    }
}
void restore_phy_elem_atk_result(int i0,int i1,int i2,int i3,int i4)
{
    if (i0<0)i0=0; 
    if (i1<0)i1=0; 
    if (i2<0)i2=0; 
    if (i3<0)i3=0; 
    if (i4<0)i4=0; 
    int i,j;
    {
        result_phy_elem_atk[count_phy_elem_atk][0]=i0;
        result_phy_elem_atk[count_phy_elem_atk][1]=i1;
        result_phy_elem_atk[count_phy_elem_atk][2]=i2;
        result_phy_elem_atk[count_phy_elem_atk][3]=i3;
        result_phy_elem_atk[count_phy_elem_atk][4]=i4;
        result_phy_elem_atk[count_phy_elem_atk][5]=multi_atk_p_e_phy;
        result_phy_elem_atk[count_phy_elem_atk][6]=add_atk_p_e_phy;
        result_phy_elem_atk[count_phy_elem_atk][7]=multi_atk_p_e_elem;
        result_phy_elem_atk[count_phy_elem_atk][8]=add_atk_p_e_elem;
        
        //printf("%lf %lf %lf %lf\n",result_elem_atk[count_elem_atk][4],result_elem_atk[count_elem_atk][5]);

        if(count_elem_atk>=count_elem_atk_max) overflow_phy_elem_atk = 1;
        count_phy_elem_atk++;
    }
}
void cal_phy_elem_atk()
{
    /*
    伏魔耗命蓝（3虫），业铠修罗红，因祸得福，狂龙病，连击
    */
    
    int i0,i1,i2,i3,i4;
    for(i0=armor_phy_skill_high[7];i0>=armor_phy_skill_low[7]&&i0>=0;i0--)   //伏魔耗命蓝（3虫）
    {
        if(armor_elem_skill_low[7]>=0)
        {
            add_atk_p_e_phy = add_atk_p_e_phy + Dereliction_phy_add[i0] * armor_phy_skill_k[7];
            add_atk_p_e_elem = add_atk_p_e_elem + Dereliction_elem_add[i0] * (1 - armor_phy_skill_k[7]);
        }
        else
        {
            add_atk_p_e_phy = add_atk_p_e_phy;
            add_atk_p_e_elem = add_atk_p_e_elem;
            i0 = 0;
        }
        for(i1=armor_phy_skill_high[8];i1>=armor_phy_skill_low[8]&&i1>=0;i1--)   //业铠修罗红
        {
            if(armor_phy_skill_low[8]>=0) 
            {
                add_atk_p_e_phy = add_atk_p_e_phy + Mail_of_Hellfire_phy_add[i1] * armor_phy_skill_k[8];
                multi_atk_p_e_elem = multi_atk_p_e_elem * (armor_phy_skill_k[8] + Mail_of_Hellfire_elem_multi[i1] * (1 - armor_phy_skill_k[8]));
            }
            else
            {
                add_atk_p_e_phy = add_atk_p_e_phy;
                multi_atk_p_e_elem = multi_atk_p_e_elem;
                i1 = 0;
            }
            for(i3=armor_phy_skill_high[10];i3>=armor_phy_skill_low[10]&&i3>=0;i3--)   //狂龙病
            {
                if(armor_phy_skill_low[10]>=0)
                {
                    if(i3 <= 2 && i3 >= 0)
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy + Bloodlust_phy_add[i3] * 0.3;
                        add_atk_p_e_elem = add_atk_p_e_elem + Bloodlust_elem_add[i3] * 0.7;
                    }
                    else 
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy + Bloodlust_phy_add[i3] * 0.22;
                        add_atk_p_e_elem = add_atk_p_e_elem + Bloodlust_elem_add[i3] * 0.78;
                    }   
                }
                else
                {
                    add_atk_p_e_phy = add_atk_p_e_phy;
                    add_atk_p_e_elem = add_atk_p_e_elem;
                    i3 = 0;
                }
                for(i4=armor_phy_skill_high[11];i4>=armor_phy_skill_low[11]&&i4>=0;i4--)   //连击
                {
                    if(armor_phy_skill_low[11]>=0)
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy + Chain_Crit_phy_add[i4] * armor_phy_skill_k[11];
                        add_atk_p_e_elem = add_atk_p_e_elem + Chain_Crit_elem_add[i4] * armor_phy_skill_k[11];
                    }
                    else
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy;
                        add_atk_p_e_elem = add_atk_p_e_elem;
                        i4 = 0;
                    }
                    for(i2=armor_phy_skill_high[9];i2>=armor_phy_skill_low[9]&&i2>=0;i2--)   //因祸得福
                    {
                        if(armor_phy_skill_low[9]>=0)
                        {
                            if(i3 <= 2 && i3 >= 0)     //关联狂龙病所以i3
                            {
                                add_atk_p_e_phy = add_atk_p_e_phy + Coalescence_phy_add[i2] * 0.35;
                                add_atk_p_e_elem = add_atk_p_e_elem + Coalescence_elem_add[i2] * 0.35;
                            }
                            else 
                            {
                                add_atk_p_e_phy = add_atk_p_e_phy + Coalescence_phy_add[i2] * 0.25;
                                add_atk_p_e_elem = add_atk_p_e_elem + Coalescence_elem_add[i2] * 0.25;
                            }  
                        }
                        else
                        {
                            add_atk_p_e_phy = add_atk_p_e_phy;
                            add_atk_p_e_elem = add_atk_p_e_elem;
                            i2 = 0;
                        }
                        restore_phy_elem_atk_result(i0,i1,i2,i3,i4);
                        if(overflow_phy_elem_atk == 1)
                        {
                            printf("03 Too many requseted phy skills, please limit your requirement! \n");
                            add_atk_elem=0;
                            multi_atk_elem=1;
                            return;
                        }
                        //printf("armor phy_element attack calculating:...%d... phy multi:%lf phy add:%lf elem multi:%lf elem add:%lf\n",count_phy_elem_atk,result_phy_elem_atk[count_phy_elem_atk-1][5],result_phy_elem_atk[count_phy_elem_atk-1][6],result_phy_elem_atk[count_phy_elem_atk-1][7],result_phy_elem_atk[count_phy_elem_atk-1][8]);
                        if(armor_phy_skill_low[9]>=0)
                        {
                            if(i3 <= 2 && i3 >= 0)
                            {
                                add_atk_p_e_phy = add_atk_p_e_phy - Coalescence_phy_add[i2] * 0.35;
                                add_atk_p_e_elem = add_atk_p_e_elem - Coalescence_elem_add[i2] * 0.35;
                            }
                            else 
                            {
                                add_atk_p_e_phy = add_atk_p_e_phy - Coalescence_phy_add[i2] * 0.25;
                                add_atk_p_e_elem = add_atk_p_e_elem - Coalescence_elem_add[i2] * 0.25;
                            }   
                        }
                        else
                        {
                            add_atk_p_e_phy = add_atk_p_e_phy;
                            add_atk_p_e_elem = add_atk_p_e_elem;
                        }
                    }
                    
                
                    if(armor_phy_skill_low[11]>=0)
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy - Chain_Crit_phy_add[i4] * armor_phy_skill_k[11];
                        add_atk_p_e_elem = add_atk_p_e_elem - Chain_Crit_elem_add[i4] * armor_phy_skill_k[11];
                    }
                    else
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy;
                        add_atk_p_e_elem = add_atk_p_e_elem;
                    }
                }
                if(armor_phy_skill_low[10]>=0)
                {
                    if(i3 <= 2 && i3 >= 0)
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy - Bloodlust_phy_add[i3] * 0.3;
                        add_atk_p_e_elem = add_atk_p_e_elem - Bloodlust_elem_add[i3] * 0.7;
                    }
                    else 
                    {
                        add_atk_p_e_phy = add_atk_p_e_phy - Bloodlust_phy_add[i3] * 0.2;
                        add_atk_p_e_elem = add_atk_p_e_elem - Bloodlust_elem_add[i3] * 0.8;
                    }   
                }
                else
                {
                    add_atk_p_e_phy = add_atk_p_e_phy;
                    add_atk_p_e_elem = add_atk_p_e_elem;
                }
            }
            if(armor_phy_skill_low[8]>=0) 
            {
                add_atk_p_e_phy = add_atk_p_e_phy - Mail_of_Hellfire_phy_add[i1] * armor_phy_skill_k[8];
                multi_atk_p_e_elem = multi_atk_p_e_elem / (armor_phy_skill_k[8] + Mail_of_Hellfire_elem_multi[i1] * (1 - armor_phy_skill_k[8]));
            }
            else
            {
                add_atk_p_e_phy = add_atk_p_e_phy;
                multi_atk_p_e_elem = multi_atk_p_e_elem;
            }
        }
        if(armor_phy_skill_high[7]>=0)
        {
            add_atk_p_e_phy = add_atk_p_e_phy - Dereliction_phy_add[i0] * armor_phy_skill_k[7];
            add_atk_p_e_elem = add_atk_p_e_elem - Dereliction_elem_add[i0] * (1 - armor_phy_skill_k[7]);
        }
        else
        {
            add_atk_p_e_phy = add_atk_p_e_phy;
            add_atk_p_e_elem = add_atk_p_e_elem;
        }
    }
}
void restore_affi_result(int i0,int i1,int i2)
{
    if (i0<0)i0=0; 
    if (i1<0)i1=0; 
    if (i2<0)i2=0; 
    result_affi[count_affi][0]=i0;
    result_affi[count_affi][1]=i1;
    result_affi[count_affi][2]=i2;
    result_affi[count_affi][3]=add_affi;
    if(count_affi>=count_affi_max) overflow_affi = 1 ;
    count_affi++;
}
void cal_affi()
{
    /*
        看破，弱点特效，精神抖擞； 
        狂龙病也会加会心，但是不在这里计算
    */
    //printf("   00:%d %d ",armor_affi_skill_high[0],armor_affi_skill_low[0]);
    int i0,i1,i2;
    for(i0=armor_affi_skill_high[0];i0>=armor_affi_skill_low[0]&&i0>=0;i0--)   //看破
    {
        if(armor_affi_skill_low[0]>=0) 
        add_affi = add_affi + Critial_Eye[i0] * armor_affi_skill_k[0];
        else add_affi = add_affi,i0=0;
        //printf("01:%lf ",add_affi);
        for(i1=armor_affi_skill_high[1];i1>=armor_affi_skill_low[1]&&i1>=0;i1--)   //弱点特效
        {
            if(armor_affi_skill_low[1]>=0)
            add_affi = add_affi + Weakness_Exploit[i1] * armor_affi_skill_k[1];
            else add_affi = add_affi,i1=0;
            //printf("02:%lf ",add_affi);
            for(i2=armor_affi_skill_high[2];i2>=armor_affi_skill_low[2]&&i2>=0;i2--)   //精神抖擞
            {
                if(armor_affi_skill_low[2]>=0)
                add_affi = add_affi + Maximum_Might[i2] * armor_affi_skill_k[2];
                else add_affi = add_affi,i2=0;
                //printf("03:%lf ",add_affi);
                restore_affi_result(i0,i1,i2);
                if(overflow_affi == 1)
                {
                    printf("04 Too many requseted affinity skills, please limit your requirement! \n");
                    //printf("count_affi :%d\n",count_atk_affi);
                    add_affi = 0;
                    return;
                }
                //printf("armor affinity calculating:...%d... affinity:%lf\n",count_affi,result_affi[count_affi-1][3]);
                if(armor_affi_skill_low[2]>=0)
                add_affi = add_affi - Maximum_Might[i2] * armor_affi_skill_k[2];
                else add_affi = add_affi,i2=0;
            }
            if(armor_affi_skill_low[1]>=0)
            add_affi = add_affi - Weakness_Exploit[i1] * armor_affi_skill_k[1];
            else add_affi = add_affi,i1=0;
        }
        if(armor_affi_skill_low[0]>=0) 
        add_affi = add_affi - Critial_Eye[i0] * armor_affi_skill_k[0];
        else add_affi = add_affi;
    }
}           
void restore_atk_affi_result(int i0)
{
    if (i0<0)i0=0; 
    result_atk_affi[count_atk_affi][0]=i0;
    result_atk_affi[count_atk_affi][1]=multi_atk_p_a;
    result_atk_affi[count_atk_affi][2]=add_atk_p_a;
    result_atk_affi[count_atk_affi][3]=add_affi_p_a;
    if(count_atk_affi>=count_atk_affi_max) overflow_atk_affi = 1 ;
    count_atk_affi++;
}
void cal_atk_affi()
{
    /*
    挑战者
    */
    int i0;
    for(i0=armor_phy_skill_high[1];i0>=armor_phy_skill_low[1]&&i0>=0;i0--)   //挑战者
    {
        if(armor_phy_skill_low[1] >= 0) 
        {
            add_atk_p_a = add_atk_p_a + Agitator_add[i0] * armor_phy_skill_k[1];
            add_affi_p_a = add_affi_p_a + Agitator_affi[i0] * armor_phy_skill_k[1];
        }
        else
        {
            add_atk_p_a = add_atk_p_a;
            add_affi_p_a = add_affi_p_a;
            i0 = 0;
        }
        restore_atk_affi_result(i0);
        if(overflow_atk_affi == 1)
        {
            printf("05 Too many requseted skills, please limit your requirement! \n");
            //printf("count_affi :%d\n",count_atk_affi);
            add_affi = 0;
            return;
        }
        //printf("armor attack affinity calculating:...%d... attack:%lf affinity:%lf\n",count_atk_affi,result_atk_affi[count_atk_affi-1][2],result_atk_affi[count_atk_affi-1][3]);
        if(armor_phy_skill_low[1] >= 0) 
        {
            add_atk_p_a = add_atk_p_a - Agitator_add[i0] * armor_phy_skill_k[1];
            add_affi_p_a = add_affi_p_a - Agitator_affi[i0] * armor_phy_skill_k[1];
        }
        else
        {
            add_atk_p_a = add_atk_p_a;
            add_affi_p_a = add_affi_p_a;
            i0 = 0;
        }
    }
}
void restore_dmg_result(int i0,int i1,int i2)
{
    if (i0<0)i0=0; 
    if (i1<0)i1=0; 
    if (i2<0)i2=0; 
    result_dmg[count_dmg][0] = i0;
    result_dmg[count_dmg][1] = i1;
    result_dmg[count_dmg][2] = i2;
    result_dmg[count_dmg][3] = crit_phy;
    result_dmg[count_dmg][4] = crit_elem;
    result_dmg[count_dmg][5] = crit_elem_weak;
    count_dmg++;
}
void cal_dmg()
{
    /*
    超会心，属性会心，弱特属性
    */
    int i0,i1,i2;
    for(i0=armor_dmg_skill_high[0];i0>=armor_dmg_skill_low[0]&&i0>=0;i0--)
    {
        if(armor_dmg_skill_low[0]>=0)
        crit_phy = Critical_Boost[i0];
        else crit_phy = Critical_Boost[0],i0=0;
        for(i1=armor_dmg_skill_high[1];i1>=armor_dmg_skill_low[1]&&i1>=0;i1--)
        {
            if(armor_dmg_skill_low[1]>=0)
            crit_elem = Critical_Element[i1];
            else crit_elem = Critical_Element[0],i1=0;
            for(i2=armor_dmg_skill_high[2];i2>=armor_dmg_skill_low[2]&&i2>=0;i2--)
            {
                if(armor_dmg_skill_low[2]>=0 && hit_elem >= 20)
                crit_elem_weak = Element_Exploit[i2];
                else crit_elem_weak = Element_Exploit[0],i2=0;
                restore_dmg_result(i0,i1,i2);
                if(overflow_dmg == 1)
                {
                    printf("06 Too many requseted skills, please limit your requirement! \n");
                    add_affi = 0;
                    return;
                }
                crit_elem_weak = Element_Exploit[0];
            }
            crit_elem = Critical_Element[0];
        }
        crit_phy = Critical_Boost[0];
    }
}
void restore_final_result(int i1,int i2,int i3,int i4,int i5,int i6,int dmg)
{
    int k;  //8 4 5 3 1 3
    for(k = 0;k < 8;k++)
    {
        if(k>0&&k<6)
        result_final[count_final][k+1] = result_phy_atk[i1][k];
        else if(k==0)
        result_final[count_final][k] = result_phy_atk[i1][k];
        else if(k>5)
        result_final[count_final][k+6] = result_phy_atk[i1][k];
    }
    //printf("1\n");
    for(k=0;k<4;k++)
    {
        result_final[count_final][14+k] = result_elem_atk[i2][k];
    }
    //printf("1\n");
    for(k=0;k<5;k++)
    {
        result_final[count_final][7+k] = result_phy_elem_atk[i3][k];
    }
    //printf("1\n");
    for(k=0;k<3;k++)
    {
        result_final[count_final][18+k] = result_affi[i4][k];
    }
    //printf("1\n");
    k=0 , result_final[count_final][1] = result_atk_affi[i5][k];
    //printf("1\n");
    for(k=0;k<3;k++)
    {
        result_final[count_final][21+k] = result_dmg[i6][k];
    }
    //printf("1\n");
    result_final[count_final][24] = dmg;
    //int kk;
    //for(kk=0;kk<25;kk++) printf("%d ",result_final[count_final][kk]);
    count_final++;
    if(count_final >= count_final_max) overflow_final = 1;
}
void sort()
{
    int i,j,k,count;
    int temp[33];
    for (i = 0; i < count_final - 1; i++)
    {
        for (j = 0; j < count_final - 1 - i; j++)
        {
            if (result_final[j][24] < result_final[j+1][24])
            {
                for(k=0;k<25;k++)
                {
                    temp[k] = result_final[j][k];
                    result_final[j][k] = result_final[j+1][k];
                    result_final[j+1][k] = temp[k];
                }
            }
        }
    }
}
void fliter()
{   
    //锋利度补正
    double sharp_phy=0,sharp_elem=0;
    sharp_phy = 1.39*sharpness[0]+1.32*sharpness[1]+1.20*sharpness[2]+1.05*(1-sharpness[0]-sharpness[1]-sharpness[2]);
    sharp_elem = 1.25*sharpness[0]+1.15*sharpness[1]+1.06*sharpness[2]+1*(1-sharpness[0]-sharpness[1]-sharpness[2]);

    //计算最大等级的伤害期望
    double maxatk_phy = weapon_attack * (result_phy_atk[0][8] * result_phy_elem_atk[0][5] * result_atk_affi[0][1]) 
                + (result_phy_atk[0][9] + result_phy_elem_atk[0][6] + result_atk_affi[0][2]);
    double maxatk_elem = weapon_element[1] * (result_elem_atk[0][4] * result_phy_elem_atk[0][7]) 
                + (result_elem_atk[0][5] + result_phy_elem_atk[0][8]) ;
    double maxaffi = result_atk_affi[0][3] + result_affi[0][3] + Bloodlust_affi_add[3] * 0.78 * armor_phy_skill_high[10];
    double maxdmg_phy = maxatk_phy /100 * (1 - maxaffi / 100 + maxaffi / 100 * result_dmg[0][3]) * sharp_phy * hit_phy * action /100;
    double maxdmg_elem = maxatk_elem * (1 - maxaffi / 100 + maxaffi / 100 * result_dmg[0][4]) * sharp_elem * hit_elem * action_element / 100 * result_dmg[0][5];
    
    int maxdmg ;
    if(weapon_element[0]==0)
    {
        maxdmg = (int)(maxdmg_phy + 0.5);
    }
    else
    {
        maxdmg = (int)(maxdmg_phy + 0.5) + (int)(maxdmg_elem + 0.5);
    }
    //printf("%d %d\n",(int)result_dmg[0][0],(int)result_dmg[0][1]);

    printf("将所选技能等级全部最大化的最大伤害期望为：%d\n",maxdmg);
    printf("有 %d 种技能组合\n",count_phy_atk*count_elem_atk*count_phy_elem_atk*count_affi*count_atk_affi*count_dmg);

    //处理可能配装成立的区间的技能组合呈现
    int check=0, gap=0;
    double atk_phy=0, atk_elem=0, affi=0, dmg_phy=0, dmg_elem=0;
    int dmg;
    int i1,i2,i3,i4,i5,i6;
    while( check == 0)
    {
        printf("请输入想要查看与最大伤害期望相差绝对值为 x 整数的技能搭配，为您提供在伤害区间为+-5的范围内最多10000条的结果:\n");
        scanf("%d",&gap);
        atk_phy=0, atk_elem=0, affi=0, dmg_phy=0, dmg_elem=0, dmg=0;
        printf("在提示输出完毕前请勿打开 putout.csv 文件，否则写入文件过程会失败\n");
        for(i1=0;i1<count_phy_atk;i1++)
        {
            for(i2=0;i2<count_elem_atk;i2++)
            {
                for(i3=0;i3<count_phy_elem_atk;i3++)
                {
                    for(i4=0;i4<count_affi;i4++)
                    {
                        for(i5=0;i5<count_atk_affi;i5++)
                        {
                            for(i6=0;i6<count_dmg;i6++)
                            {
                                atk_phy = weapon_attack * (result_phy_atk[i1][8] * result_phy_elem_atk[i3][5] * result_atk_affi[i5][1]) 
                                            + (result_phy_atk[i1][9] + result_phy_elem_atk[i3][6] + result_atk_affi[i5][2]);
                                atk_elem = weapon_element[1] * (result_elem_atk[i2][4] * result_phy_elem_atk[i3][7]) 
                                            + (result_elem_atk[i2][5] + result_phy_elem_atk[i3][8]) ;
                                if((int)result_phy_elem_atk[i3][3]==3)
                                {affi = result_atk_affi[i5][3] + result_affi[i4][3] + Bloodlust_affi_add[3] * 0.78 * armor_phy_skill_high[10];}
                                else 
                                {affi = result_atk_affi[i5][3] + result_affi[i4][3] + Bloodlust_affi_add[(int)result_phy_elem_atk[i3][3]] * 0.7* armor_phy_skill_high[10];}
                                dmg_phy = atk_phy /100 * (1 - affi / 100 + affi / 100 * result_dmg[i6][3]) * sharp_phy * hit_phy * action /100;
                                dmg_elem = atk_elem * (1 - affi / 100 + affi / 100 * result_dmg[i6][4]) * sharp_elem * hit_elem * action_element/100 * result_dmg[i6][5];
                                if(weapon_element[0]==0)
                                {
                                    dmg = (int)(dmg_phy + 0.5);
                                }
                                else
                                {
                                    dmg = (int)(dmg_phy + 0.5) + (int)(dmg_elem + 0.5);
                                }
                                //printf("%d\n",dmg);
                                //对比gap进行筛选
                                if(dmg >= maxdmg - gap - 5 && dmg <= maxdmg - gap + 5)
                                {
                                    //printf("%d %d %d\n",(int)result_dmg[i6][0],(int)result_dmg[i6][1],dmg);
                                    
                                    restore_final_result(i1,i2,i3,i4,i5,i6,dmg);
                                    if (overflow_final == 1) break; 
                                }
                            }
                            if (overflow_final == 1) break;
                        }
                        if (overflow_final == 1) break;
                    }
                    if (overflow_final == 1) break;
                }
                if (overflow_final == 1) break;
            }
            if (overflow_final == 1) break;
        }
        printf("count:%d\n",count_final);
        sort();
        putout();
        printf("输出完毕，可以打开putout.csv文件查看是否可以实现配装\n");
        printf("是否需要重新输入绝对值x 以重新筛选伤害区间？\n[Y/N]\n");
        char rein;
        scanf("%s",&rein);
        if(rein == 'Y')
        {
            count_final = 0; 
            overflow_final = 0;
            continue;
        }
        else break;
    }  
}
void putout()
{
    
    FILE *fp = fopen("putout.csv", "w");
    if (fp == NULL) {
        printf("fopen() failed.\n");
        return;
    }
    char *x="攻击,挑战者,怨恨,死里逃生,攻击守势,火场怪力,龙气活性,伏魔耗命,业铠修罗红,因祸得福,狂龙病,连击,巧击,偷袭,对应属性攻击强化,钢龙的恩惠,炎鳞的恩惠,风雷合一,看破,弱点特效,精神抖擞,超会心,会心击属性,弱特属性";
    //fprintf(fp,"%s\n",x);
    int i,j;
    for(i=0;i<count_final;i++)
    {
        if(i%10==0) fprintf(fp,"%s\n",x);
        for(j=0;j<25;j++)
        {
            fprintf(fp,"%d,",(int)result_final[i][j]);
        }
 
        fprintf(fp,"\n");
    }
    fprintf(fp,"%s\n",x);
    fclose(fp);
}


int main()
{
    read();

    cal_phy_atk();

    cal_elem_atk();
    
    cal_phy_elem_atk(); 
    
    
    cal_affi();
    
    cal_atk_affi(); 
    
    cal_dmg();
    
    fliter();

    return 0;
}
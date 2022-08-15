#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attack_phy_armor.h"
#include "attack_element_armor.h"
#include "affinity_armor.h"
#include "damage_armor_phy.h"
#include "damage_armor_element.h"

//only calculate weapon final phy attack
//�洢��ͬ���༼��
int armor_phy_skill_low[14]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int armor_phy_skill_high[14]={7,5,5,3,3,5,5,3,3,3,3,3,3,3};    
int armor_elem_skill_low[4]={-1,-1,-1,-1};
int armor_elem_skill_high[4]={5,4,4,4};
int armor_affi_skill_low[3]={-1,-1,-1};
int armor_affi_skill_high[3]={7,3,3};
int armor_dmg_skill_low[3]={-1,-1,-1};
int armor_dmg_skill_high[3]={3,3,3};
//�洢�������������
double weapon_attack;
double sharpness[3]={0,0,0};    //�ϣ��ף���ն�����ı���:0.5,0.5,0; 1,0,0; 0,0.8,0.2; ...
double weapon_affinity;
double weapon_element[2]={0,0};   //��0λ��ʾ�������� 0���ޣ� 1���� 2��ˮ�� 3������ 4���ף� 5��������1λ�Ǿ�������ֵ
double action_element=1;   //��ʼ��������������ƽ�����Բ���
double action=1;            //��ʼ���������趯��ֵ
//��ʼ���ܻ���ƽ�����ʺ�ƽ����������
double hit_phy=0,hit_elem=0;
//��ʼ�������������ֵ
double add_atk_phy=0;       //���������ļ������
double multi_atk_phy=1;     // 
double add_atk_elem=0;      //�����Թ����ļ������
double multi_atk_elem=1;    //
double add_atk_p_e_phy=0;   //����+���Թ���������������
double multi_atk_p_e_phy=1; //
double add_atk_p_e_elem=0;   //
double multi_atk_p_e_elem=1; //
double add_affi=0;          //�����ĵļ���
double add_atk_p_a=0;       //����+���ĵĹ����ļ������ͻ��ļ���
double multi_atk_p_a=1;     //
double add_affi_p_a=0;      //
double crit_phy=1.25;       //�����˺��Ļ��ĳ�ʼ����
double crit_elem=1;         //���Ļ������˺��Ļ��ĳ�ʼ����
double crit_elem_weak=1;    //�������صĻ��ĳ�ʼ����
//�洢��Ӧ�������ܵĸ�����
double armor_phy_skill_k[14];
double armor_elem_skill_k[4];
double armor_affi_skill_k[3];
double armor_dmg_skill_k[3];

//����480000������������ؼ��ܵļ������0-7��8��9Ϊ����õ������������㣬����
//������Թ�ޣ������������������ƣ��𳡹������������ԣ��ɻ���͵Ϯ
double result_phy_atk[480000][10];
int count_phy_atk=0;
int count_phy_atk_max=480000;
int overflow_phy_atk=0;   //�趨����ʱ��1������

//����1000�����Թ�������ؼ������0-3��4��5Ϊ����õ����Թ��������㣬����
//��Ӧ����ǿ�����ֿǵĶ��ݣ����۵Ķ��ݣ����׺�һ
double result_elem_atk[1000][6];
int count_elem_atk=0;
int count_elem_atk_max=1000;
int overflow_elem_atk=0;   //�趨����ʱ��1������

//����2000������+���Թ�������ؼ������0-4��5��6��7��8Ϊ����õ��������Թ��������㣬����
//��ħ��������3�棩��ҵ�����޺죬����ø���������������
double result_phy_elem_atk[2000][9];
int count_phy_elem_atk=0;
int count_phy_elem_atk_max=2000;
int overflow_phy_elem_atk=0;   //�趨����ʱ��1������

//����200�������ؼ������0-2��3Ϊ����õ�������
//���ƣ�������Ч��������
double result_affi[200][4];
int count_affi=0;
int count_affi_max=200;
int overflow_affi=0;   //�趨����ʱ��1������

//����10�鹥����+������ؼ������0��1��2��3Ϊ����õ����������㣬���㣬�����ʼ���
//��ս��
double result_atk_affi[10][4];
int count_atk_affi=0;
int count_atk_affi_max=10;
int overflow_atk_affi=0;   //�趨����ʱ��1������

//����80���˺�������ؼ������0-2��3,4,5Ϊ����õ����������˺�1,�����˺�2����
//�����ģ����Ļ����� ��������
double result_dmg[80][6];
int count_dmg=0;
int count_dmg_max=80;
int overflow_dmg=0;   //�趨����ʱ��1������


//����Ϊ10000�������˺�����洢
double result_final[10000][25];     //14+4+3+3=24����0-23��24�����˺�
int count_final=0;
int count_final_max=10000;
int overflow_final=0;   //�趨����ʱ��1������

//��������
void cal_phy_atk();             //����
void restore_phy_atk_result();
void cal_elem_atk();            //����
void restore_elem_atk_result();
void cal_phy_elem_atk();        //����+����
void restore_phy_elem_atk_result();
void cal_affi();                //����
void restore_affi_result();
void cal_atk_affi();            //����+����
void restore_atk_affi_result();
void cal_dmg();               //�˺�����
void restore_dmg_result();
void cal_final();               //���մ���
void restore_final_result();

void read();                    //��������
void fliter();             //ɸѡ�����˺����������
void sort();                    //��������
void putout();                  //��putout.csv������

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

        if(line_count == 2)             //ȡ������幥����
        {
            weapon_attack = atof(s);    //�ַ���ת����
            //printf("weapon atk:%lf\n",weapon_attack);
        }

        if(line_count == 4)             //ȡ����������
        {
            weapon_affinity = atof(s);    //�ַ���ת����
            //printf("weapon affi:%lf\n",weapon_affinity);
        }

        if(line_count == 6)             //ȡնζ���ı���
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 3) 
            {
                sharpness[k] = atof(p);//�ַ���ת����
                //printf("sharp[%d]=%lf\n",k, sharpness[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 8)             //ȡ����ֵ
        {
            action = atof(s);    //��¼����ʽʹ�õĶ���ֵ
            //printf("action: %lf\n",s,action);
        }

        if(line_count == 10)             //�����������
        {
            k=0;
            int elem_check=0,elem_data=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 5) 
            {
                elem_data = atoi(p);//�ַ���ת����
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
            if(elem_check == 0)     //���������
            {
                weapon_element[0]=0;
                weapon_element[1]=0;
            }
            //printf("element: %lf %lf\n",weapon_element[0],weapon_element[1]);
        }

        if(line_count == 12)             //ƽ��������ʽ���Բ���
        {
            action_element = atof(s);    //��¼����������ƽ�����Բ���
            //printf("action element: %lf\n",action_element);
        }

        if(line_count == 16)             //ƽ���ܻ����ʣ�ƽ���ܻ���������
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            hit_phy = atof(p);
            p = strtok(NULL, delim);
            hit_elem = atof(p);
            //printf("hit: %lf %lf\n",hit_phy,hit_elem);
        }

        if(line_count == 20)             //ȡ����������ؼ���
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 14) 
            {
                armor_phy_skill_low[k] = atoi(p);//�ַ���ת����
                //printf("armor_phy_skill_low[%d]=%d ",k, armor_phy_skill_low[k]);
                p = strtok(NULL, delim);
                armor_phy_skill_high[k] = atoi(p);//�ַ���ת����
                //printf("armor_phy_skill_high[%d]=%d\n",k, armor_phy_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_phy_skill_low[k]<0)
                armor_phy_skill_high[k]=0,armor_phy_skill_low[k]=0;
                k++;
            }
        }

        if(line_count == 22)            //ȡ����������ؼ��ܵĸ�����
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 14) 
            {
                armor_phy_skill_k[k] = atof(p);//�ַ���ת����
                //printf("armor_phy_skill_k[%d]=%lf\n",k, armor_phy_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 24)            //ȡ���Թ�������ؼ��ܣ�ע�ⲿ�ּ��ܵĿ����������������ָ���
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 4) 
            {
                armor_elem_skill_low[k] = atoi(p);//�ַ���ת����
                //printf("armor_elem_skill_low[%d]=%d ",k, armor_elem_skill_low[k]);
                p = strtok(NULL, delim);
                armor_elem_skill_high[k] = atoi(p);//�ַ���ת����
                //printf("armor_elem_skill_high[%d]=%d\n",k, armor_elem_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_elem_skill_low[k]<0)
                armor_elem_skill_low[k]=0,armor_elem_skill_high[k]=0;
                k++;
            }
        }
    
        if(line_count == 26)            //��¼���Թ�������ؼ��ܵĸ�����
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            while (p != NULL && k < 4) 
            {
                armor_elem_skill_k[k] = atof(p);//�ַ���ת����
                //printf("armor_elem_skill_k[%d]=%d\n",k, armor_elem_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 28)            //��¼��������ؼ���
        {
            k=0;
            p = strtok(s, delim);
            //printf("%s\n%s\n",s,p);
            while (p != NULL && k < 3) 
            {
                armor_affi_skill_low[k] = atoi(p);//�ַ���ת����
                //printf("armor_affi_skill_low[%d]=%d\n",k, armor_affi_skill_low[k]);
                p = strtok(NULL, delim);
                armor_affi_skill_high[k] = atoi(p);//�ַ���ת����
                //printf("armor_affi_skill_high[%d]=%d\n",k, armor_affi_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_affi_skill_low[k] < 0)
                armor_affi_skill_low[k] = 0 , armor_affi_skill_high[k] =0;
                k++;
            }
        }

        if(line_count == 30)            //��¼��������ؼ��ܵĸ�����
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_affi_skill_k[k] = atof(p);//�ַ���ת����
                //printf("armor_affi_skill_k[%d]=%d\n",k, armor_affi_skill_k[k]);
                p = strtok(NULL, delim);
                k++;
            }
        }

        if(line_count == 32) //��¼�˺���������
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_dmg_skill_low[k] = atoi(p);//�ַ���ת����
                //printf("armor_dmg_skill_low[%d]=%d\n",k, armor_dmg_skill_low[k]);
                p = strtok(NULL, delim);
                armor_dmg_skill_high[k] = atoi(p);//�ַ���ת����
                //printf("armor_dmg_skill_high[%d]=%d\n",k, armor_dmg_skill_high[k]);
                p = strtok(NULL, delim);
                if(armor_dmg_skill_low[k] < 0)
                armor_dmg_skill_low[k] = 0, armor_dmg_skill_high[k] = 0;
                k++;
            }
        }

        if(line_count == 34) //��¼�˺��������ܸ�����
        {
            k=0;
            //printf("%s\n",s);
            p = strtok(s, delim);
            
            while (p != NULL && k < 3) 
            {
                armor_dmg_skill_k[k] = atof(p);//�ַ���ת����
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
    ����,Թ��,��������,��������,�𳡹���,��������,�ɻ�,͵Ϯ
    */
    //printf("%d",armor_phy_skill_low[16]);
    
    int i0,i1,i2,i3,i4,i5,i6,i7;
    for(i0=armor_phy_skill_high[0];i0>=armor_phy_skill_low[0]&&i0>=0;i0--)   //����
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
        for(i1=armor_phy_skill_high[2];i1>=armor_phy_skill_low[2]&&i1>=0;i1--)   //Թ��
        {
            //printf("012\n");
            if(armor_phy_skill_low[2]>=0)
            add_atk_phy = add_atk_phy + Resentment_add[i1] * armor_phy_skill_k[2];
            else add_atk_phy = add_atk_phy,i1=0;
            for(i2=armor_phy_skill_high[3];i2>=armor_phy_skill_low[3]&&i2>=0;i2--)   //��������
            {
                //printf("013\n");
                if(armor_phy_skill_low[3]>=0)
                add_atk_phy = add_atk_phy + Resuscitate_add[i2] * armor_phy_skill_k[3];
                else add_atk_phy = add_atk_phy,i2=0;
                for(i3=armor_phy_skill_high[4];i3>=armor_phy_skill_low[4]&&i3>=0;i3--)   //��������
                {
                    //printf("014\n");
                    if(armor_phy_skill_low[4]>=0)
                    multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[4]+Offensive_Guard_multi[i3] * armor_phy_skill_k[4]);
                    else multi_atk_phy = multi_atk_phy,i3=0;
                    for(i4=armor_phy_skill_high[5];i4>=armor_phy_skill_low[5]&&i4>=0;i4--)   //�𳡹���
                    {
                        //printf("015\n");
                        if(armor_phy_skill_low[5]>=0)
                        multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[5]+Heroics_multi[i4] * armor_phy_skill_k[5]);
                        else multi_atk_phy = multi_atk_phy,i4=0;
                        for(i5=armor_phy_skill_high[6];i5>=armor_phy_skill_low[6]&&i5>=0;i5--)   //��������
                        {
                            //printf("016\n");
                            if(armor_phy_skill_low[6]>=0)
                            multi_atk_phy = multi_atk_phy * (1-armor_phy_skill_k[6]+Dragonheart_multi[i5] * armor_phy_skill_k[6]);
                            else multi_atk_phy = multi_atk_phy,i5=0;
                            for(i6=armor_phy_skill_high[12];i6>=armor_phy_skill_low[12]&&i6>=0;i6--)    //�ɻ�
                            {
                                if(armor_phy_skill_low[12]>=0)
                                add_atk_phy = add_atk_phy + Adrenaline_Rush[i6] * armor_phy_skill_k[12];
                                else add_atk_phy = add_atk_phy,i6=0;
                                for(i7=armor_phy_skill_high[13];i7>=armor_phy_skill_low[13]&&i7>=0;i7--)    //͵Ϯ
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
    if(check == 0)      //������
    {
        count_elem_atk = 0;
    }
    else                //������
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
    ��Ӧ���Թ���ǿ���������Ķ��ݣ����۵Ķ��ݣ����׺�һ
    */
    
    int i0,i1,i2,i3;
    if( weapon_element[0] == 0) 
    {
        restore_elem_atk_result(0,i0,i1,i2,i3);
        return;
    }
    for(i0=armor_elem_skill_high[0];i0>=armor_elem_skill_low[0]&&i0>=0;i0--)   //��Ӧ���Թ���ǿ��
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
        for(i1=armor_elem_skill_high[1];i1>=armor_elem_skill_low[1]&&i1>=0;i1--)   //�ֿǵĶ���
        {
            if(armor_elem_skill_low[1]>=0) 
            multi_atk_elem = multi_atk_elem * (1-armor_elem_skill_k[1]+Kushala_Blessing_multi[i1] * armor_elem_skill_k[1]);
            else multi_atk_elem = multi_atk_elem,i1=0;
            for(i2=armor_elem_skill_high[2];i2>=armor_elem_skill_low[2]&&i2>=0;i2--)   //���۵Ķ���
            {
                if(armor_elem_skill_low[2]>=0)
                multi_atk_elem = multi_atk_elem * (1-armor_elem_skill_k[2]+Teostra_Blessing_multi[i2] * armor_elem_skill_k[2]);
                else multi_atk_elem = multi_atk_elem,i2=0;
                for(i3=armor_elem_skill_high[3];i3>=armor_elem_skill_low[3]&&i3>=0;i3--)   //���׺�һ
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
    ��ħ��������3�棩��ҵ�����޺죬����ø���������������
    */
    
    int i0,i1,i2,i3,i4;
    for(i0=armor_phy_skill_high[7];i0>=armor_phy_skill_low[7]&&i0>=0;i0--)   //��ħ��������3�棩
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
        for(i1=armor_phy_skill_high[8];i1>=armor_phy_skill_low[8]&&i1>=0;i1--)   //ҵ�����޺�
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
            for(i3=armor_phy_skill_high[10];i3>=armor_phy_skill_low[10]&&i3>=0;i3--)   //������
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
                for(i4=armor_phy_skill_high[11];i4>=armor_phy_skill_low[11]&&i4>=0;i4--)   //����
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
                    for(i2=armor_phy_skill_high[9];i2>=armor_phy_skill_low[9]&&i2>=0;i2--)   //����ø�
                    {
                        if(armor_phy_skill_low[9]>=0)
                        {
                            if(i3 <= 2 && i3 >= 0)     //��������������i3
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
        ���ƣ�������Ч�������ӣ� 
        ������Ҳ��ӻ��ģ����ǲ����������
    */
    //printf("   00:%d %d ",armor_affi_skill_high[0],armor_affi_skill_low[0]);
    int i0,i1,i2;
    for(i0=armor_affi_skill_high[0];i0>=armor_affi_skill_low[0]&&i0>=0;i0--)   //����
    {
        if(armor_affi_skill_low[0]>=0) 
        add_affi = add_affi + Critial_Eye[i0] * armor_affi_skill_k[0];
        else add_affi = add_affi,i0=0;
        //printf("01:%lf ",add_affi);
        for(i1=armor_affi_skill_high[1];i1>=armor_affi_skill_low[1]&&i1>=0;i1--)   //������Ч
        {
            if(armor_affi_skill_low[1]>=0)
            add_affi = add_affi + Weakness_Exploit[i1] * armor_affi_skill_k[1];
            else add_affi = add_affi,i1=0;
            //printf("02:%lf ",add_affi);
            for(i2=armor_affi_skill_high[2];i2>=armor_affi_skill_low[2]&&i2>=0;i2--)   //������
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
    ��ս��
    */
    int i0;
    for(i0=armor_phy_skill_high[1];i0>=armor_phy_skill_low[1]&&i0>=0;i0--)   //��ս��
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
    �����ģ����Ի��ģ���������
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
    //�����Ȳ���
    double sharp_phy=0,sharp_elem=0;
    sharp_phy = 1.39*sharpness[0]+1.32*sharpness[1]+1.20*sharpness[2]+1.05*(1-sharpness[0]-sharpness[1]-sharpness[2]);
    sharp_elem = 1.25*sharpness[0]+1.15*sharpness[1]+1.06*sharpness[2]+1*(1-sharpness[0]-sharpness[1]-sharpness[2]);

    //�������ȼ����˺�����
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

    printf("����ѡ���ܵȼ�ȫ����󻯵�����˺�����Ϊ��%d\n",maxdmg);
    printf("�� %d �ּ������\n",count_phy_atk*count_elem_atk*count_phy_elem_atk*count_affi*count_atk_affi*count_dmg);

    //���������װ����������ļ�����ϳ���
    int check=0, gap=0;
    double atk_phy=0, atk_elem=0, affi=0, dmg_phy=0, dmg_elem=0;
    int dmg;
    int i1,i2,i3,i4,i5,i6;
    while( check == 0)
    {
        printf("��������Ҫ�鿴������˺�����������ֵΪ x �����ļ��ܴ��䣬Ϊ���ṩ���˺�����Ϊ+-5�ķ�Χ�����10000���Ľ��:\n");
        scanf("%d",&gap);
        atk_phy=0, atk_elem=0, affi=0, dmg_phy=0, dmg_elem=0, dmg=0;
        printf("����ʾ������ǰ����� putout.csv �ļ�������д���ļ����̻�ʧ��\n");
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
                                //�Ա�gap����ɸѡ
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
        printf("�����ϣ����Դ�putout.csv�ļ��鿴�Ƿ����ʵ����װ\n");
        printf("�Ƿ���Ҫ�����������ֵx ������ɸѡ�˺����䣿\n[Y/N]\n");
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
    char *x="����,��ս��,Թ��,��������,��������,�𳡹���,��������,��ħ����,ҵ�����޺�,����ø�,������,����,�ɻ�,͵Ϯ,��Ӧ���Թ���ǿ��,�����Ķ���,���۵Ķ���,���׺�һ,����,������Ч,������,������,���Ļ�����,��������";
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
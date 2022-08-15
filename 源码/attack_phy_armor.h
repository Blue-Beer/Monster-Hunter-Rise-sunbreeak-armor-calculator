#ifndef __attack_phy_armor__

#define __attack_phy_armor__

//记录对物理攻击力有影响的防具技能。
//数组来储存技能的效果 等级为n,则数组长度为n+1，0储存无等级状态
/*10.03防具技能列表
攻击
挑战者
无伤
怨恨
死里逃生
看破
超会心
弱点特效
力量解放
精神抖擞
会心击【属性】
达人艺
火属性攻击强化
水属性攻击强化
冰属性攻击强化
雷属性攻击强化
龙属性攻击强化
毒属性强化
麻痹属性强化
睡眠属性强化
爆破属性强化
匠
利刃
弹丸节约
刚刃打磨
心眼
弹道强化
钝器能手
解放弓的蓄力阶段
集中
强化持续
跑者
体术
耐力急速回复
防御性能
防御强化
攻击守势
拔刀术【技】
拔刀术【力】
纳刀术
击晕术
夺取耐力
滑走强化
吹笛名人
炮术
炮弹装填
特殊射击强化
通常弹・连射箭强化
贯穿弹・贯穿箭强化
散弹・扩散箭强化
装填扩充
装填速度
减轻后坐力
抑制偏移
速射强化
防御
精灵加护
体力回复量提升
回复速度
快吃
耳塞
风压耐性
耐震
泡沫之舞
回避性能
回避距离提升
火耐性
水耐性
冰耐性
雷耐性
龙耐性
属性异常状态的耐性
毒耐性
麻痹耐性
睡眠耐性
昏厥耐性
泥雪耐性
爆破异常状态的耐性
植生学
地质学
破坏王
捕获名人
剥取名人
幸运
砥石使用高速化
炸弹客
最爱蘑菇
道具使用强化
广域化
满足感
火场怪力
不屈
减轻胆怯
跳跃铁人
剥取铁人
饥饿耐性
飞身跃入
佯动
骑乘名人
霞皮的恩惠
钢壳的恩惠
炎鳞的恩惠
龙气活性
翔虫使
墙面移动
逆袭
高速变形
鬼火缠
风纹一致
雷纹一致
风雷合一
转祸为福
业铠【修罗】
伏魔耗命
*/
//乘算技能数据
double Attack_Boost_multi[8]={1,1,1,1,1.05,1.06,1.08,1.1};        //攻击
double Bludgeoner_multi[4]={1,1.05,1.1,1.1};          //钝器能手
double Offensive_Guard_multi[4]={1,1.05,1.1,1.15};     //攻击守势,10s
double Heroics_multi[6]={1,1,1.05,1.05,1.1,1.3};    //火场怪力
                                                //额外注释：猫火作第6级人火加入计算
double Fortify_multi[3]={1,1.1,1.2};             //不屈
double Dragonheart_multi[6]={1,1,1,1,1.05,1.1};     //龙气活性
double Sneak_Attack[4]={1.05,1.1,1.12};    //偷袭
//加算技能数据
int Attack_Boost_add[8]={0,3,6,9,7,8,9,10};        //攻击
int Agitator_add[6]={0,4,8,12,16,20};            //挑战者
int Peak_Performance_add[4]={0,5,10,20};    //无伤
int Resentment_add[6]={0,5,10,15,20,25};          //怨恨
int Resuscitate_add[4]={0,5,10,20};         //死里逃生
int Wall_Runner_add[4]={0,0,0,20};         //墙面移动 攻击力加成的时间未知，不推荐
int Counterstrike_add[4]={0,10,15,25};       //逆袭
int Coalescence_phy_add[4]={0,12,15,18};      //转祸为福物理加成
int Punishing_Draw_add[4]={0,3,5,7};    //拔刀术力
int Mail_of_Hellfire_phy_add[4]={0,15,25,35};   //业铠【修罗】红书时生效
int Chain_Crit_phy_add[4]={0,11,13,16};     //连击，默认为长枪，默认将第一刀的5攻击力加成分配给连续攻击，因为基本连段：横扫+飞身跃入刚好满足该循环。
int Bloodlust_phy_add[4]={0,10,15,20};  //狂龙病，感染时（60s发生一次）增加 假设25S感染时间，所以攻击覆盖0.3，会心覆盖0.7(此为1，2级的情况)
int Dereliction_phy_add[4]={0,25,30,35};      //伏魔耗命，均为3只虫子的加成，每少一只-5攻击力，只发生在蓝书时。
int Adrenaline_Rush[4]={0,10,15,30};     //巧击
#endif
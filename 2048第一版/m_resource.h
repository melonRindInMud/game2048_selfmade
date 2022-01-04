
#pragma once
#include "resource.h"
#include "stdafx.h"
//方向选择
typedef  int  Orient;
#define UP  1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//绘图数据
#define LOS  106
#define ILOS  100
#define  S_X  100
#define  S_Y  60

//是否使用AI：
//宏定义
//#define USE_AI   1
#define NO_USE_AI  0

//决定


//参数系数  -- 用于AI 选择
#define P1    1    //最大值位置参数
#define P2    2    //连续读参数
#define P3    4    //相邻比例参数
#define P4    8    //是否合并参数
#define P5    2    //最大值是否合并参数
#define P6    1

//智能运行速度
#define  INTERVAL 400


//重绘皮肤
//宏定义 -- 请注释掉 不适用的皮肤
#define  NO_REMOD     //不使用皮肤
//#define  REMOD_1      //蔡徐坤皮肤




//游戏设定  --- 请注释掉  不适用的模式的宏定义
//#define  OLD_RULE 
#define  NEW_RULE 


//动画设定
#define ANIMATE
//#define NO_ANIMATE


class AI;




class Game
{
public:
	int data [4][4];//先纵再横 从左上角开始
	int ID [4][4];//方块 ID
	int last_ID[4][4];

	int last_data [4][4];//上一步的格局
	int is_change[4][4];//当前格局中的每一格相对上一步是否发生了改变是否发生了改变

	int has_combine;

	int elem; //现存方块数
	Orient last_orient;
	int combine[4];//四行上一次合并次数
private:
	int n_x; //新出现的方块的横坐标
	int n_y; //新出现的方块的纵坐标
	int score;//当前分数
public:
	int gg;//游戏是否结束

public://皮肤重绘使用的数据
	CBitmap bmp[11];//使用1-10号

public:
	Game();
	~Game();
//动作
public:
	//产生一个新的数字
	void New_Turn(CDC* pDC, CFont* f);
	//移动及合并操作
	//全局

	//这个函数执行了所有的移动操作
	void Move(Orient ori);
	//单独处理某一行
	void Move_a_string(Orient ori, int ord);
	//递归函数 -- 直到不再产生新的合并
	//返回值 有合并返回 1 没有合并返回 0 
	int  process_a_string(int* n, int* num, int* tID);

	int fake_process_a_string(int* n, int* num);


	BOOL pro_Move(Orient ori, int data[][4]);//执行假设移动 -- 如果移动后和移动前相同 则返回FALSE
	//低内聚度函数 需要集合两个函数的功能


	void show_game(CDC* pDC, CFont* f);

	BOOL Check_GG(void);//任何相邻的都不相等
	BOOL Check_Similar(int pro_move[][4]);//检测模拟操作后是否和原状态同
	void  Record_change(void);//记录棋盘上移动的情况（相对上一步）

	void Copy_Pro_State(int pro_move[][4]);//将棋盘当前状态复制到检测假设移动的数据中
	void Copy_Pro_ID(int pro_ID[][4]);


	friend class AI;//AI类可以调用2048游戏控制类


};


//开始制作AI
class AI
{
public:
	void One_Turn(Game& g);//进行一次4个方向的试探的过程函数

private:
	//int move_or_not (Game& g, Orient ori);//是否能够移动 直接传递移植游戏中的函数 

	int where_the_max(Game& g, int pattren[][4]); 
	int evenness(Game& g, int pattren[][4]);
	int neighborhood(Game& g, int pattren[][4]);
	int check_combine(Game& g, int pattren[][4]);
	int max_move_or_not (Game& g,int pattren[][4]);
	//第六个函数待编写


	//检查当前元素个数
	int check_element(Game& g, int pattren[][4]);
	
};

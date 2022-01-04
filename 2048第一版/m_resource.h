
#pragma once
#include "resource.h"
#include "stdafx.h"
//����ѡ��
typedef  int  Orient;
#define UP  1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//��ͼ����
#define LOS  106
#define ILOS  100
#define  S_X  100
#define  S_Y  60

//�Ƿ�ʹ��AI��
//�궨��
//#define USE_AI   1
#define NO_USE_AI  0

//����


//����ϵ��  -- ����AI ѡ��
#define P1    1    //���ֵλ�ò���
#define P2    2    //����������
#define P3    4    //���ڱ�������
#define P4    8    //�Ƿ�ϲ�����
#define P5    2    //���ֵ�Ƿ�ϲ�����
#define P6    1

//���������ٶ�
#define  INTERVAL 400


//�ػ�Ƥ��
//�궨�� -- ��ע�͵� �����õ�Ƥ��
#define  NO_REMOD     //��ʹ��Ƥ��
//#define  REMOD_1      //������Ƥ��




//��Ϸ�趨  --- ��ע�͵�  �����õ�ģʽ�ĺ궨��
//#define  OLD_RULE 
#define  NEW_RULE 


//�����趨
#define ANIMATE
//#define NO_ANIMATE


class AI;




class Game
{
public:
	int data [4][4];//�����ٺ� �����Ͻǿ�ʼ
	int ID [4][4];//���� ID
	int last_ID[4][4];

	int last_data [4][4];//��һ���ĸ��
	int is_change[4][4];//��ǰ����е�ÿһ�������һ���Ƿ����˸ı��Ƿ����˸ı�

	int has_combine;

	int elem; //�ִ淽����
	Orient last_orient;
	int combine[4];//������һ�κϲ�����
private:
	int n_x; //�³��ֵķ���ĺ�����
	int n_y; //�³��ֵķ����������
	int score;//��ǰ����
public:
	int gg;//��Ϸ�Ƿ����

public://Ƥ���ػ�ʹ�õ�����
	CBitmap bmp[11];//ʹ��1-10��

public:
	Game();
	~Game();
//����
public:
	//����һ���µ�����
	void New_Turn(CDC* pDC, CFont* f);
	//�ƶ����ϲ�����
	//ȫ��

	//�������ִ�������е��ƶ�����
	void Move(Orient ori);
	//��������ĳһ��
	void Move_a_string(Orient ori, int ord);
	//�ݹ麯�� -- ֱ�����ٲ����µĺϲ�
	//����ֵ �кϲ����� 1 û�кϲ����� 0 
	int  process_a_string(int* n, int* num, int* tID);

	int fake_process_a_string(int* n, int* num);


	BOOL pro_Move(Orient ori, int data[][4]);//ִ�м����ƶ� -- ����ƶ�����ƶ�ǰ��ͬ �򷵻�FALSE
	//���ھ۶Ⱥ��� ��Ҫ�������������Ĺ���


	void show_game(CDC* pDC, CFont* f);

	BOOL Check_GG(void);//�κ����ڵĶ������
	BOOL Check_Similar(int pro_move[][4]);//���ģ��������Ƿ��ԭ״̬ͬ
	void  Record_change(void);//��¼�������ƶ�������������һ����

	void Copy_Pro_State(int pro_move[][4]);//�����̵�ǰ״̬���Ƶ��������ƶ���������
	void Copy_Pro_ID(int pro_ID[][4]);


	friend class AI;//AI����Ե���2048��Ϸ������


};


//��ʼ����AI
class AI
{
public:
	void One_Turn(Game& g);//����һ��4���������̽�Ĺ��̺���

private:
	//int move_or_not (Game& g, Orient ori);//�Ƿ��ܹ��ƶ� ֱ�Ӵ�����ֲ��Ϸ�еĺ��� 

	int where_the_max(Game& g, int pattren[][4]); 
	int evenness(Game& g, int pattren[][4]);
	int neighborhood(Game& g, int pattren[][4]);
	int check_combine(Game& g, int pattren[][4]);
	int max_move_or_not (Game& g,int pattren[][4]);
	//��������������д


	//��鵱ǰԪ�ظ���
	int check_element(Game& g, int pattren[][4]);
	
};

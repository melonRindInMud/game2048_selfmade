#include "stdafx.h"
#include "m_resource.h"

int antipower(int v)
{
	int m = 0;
	while(v != 1)
	{
		m ++;
		v >>= 1;
	}
	return m;
}

Game::Game()
{
	//初始化数据
	elem = 0;
	n_x = 0;
	n_y = 0;
	score = 0;
	gg = 0;
	last_orient = UP;
	has_combine = 0;
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			data[i][ii] = 0;
			//last_data[i][ii] = 0;
			ID[i][ii] = 0;
			last_ID[i][ii] = 0;
			is_change[i][ii] = 0;
		}
	}
	//设置随机种子
	srand(time(NULL));
}
void Game::New_Turn(CDC* pDC, CFont* f)
{
	static int step = 0;
	step ++;
	int value = (rand() % 2 + 1) * 2; //对半几率生成2 或 4

	int pos = rand() % (16 - elem) + 1;
	//注入

	int is_find = 0;

	for(int i = 0; i < 4 && !is_find; i ++)
	{
		for(int ii = 0; ii < 4 && !is_find; ii ++)
		{
			if(0 == data[i][ii])//空位 进行计数
			{
				pos --;
				if(0 == pos)
				{
					data[i][ii] = value;//赋值
					ID[i][ii] = step;
					//标记新的位置
					n_y = i;
					n_x = ii;
					is_find = 1;//跳出循环
				}
			}
		}
	}
	elem ++;
	if(16 == elem)
	{
		if(Check_GG())
		{
			show_game(pDC, f);
			AfxMessageBox("GG");
			::mciSendString("close start", NULL, 0, NULL);
			exit(0);
		}
		else
			;
	}
	//ready = 1;
	return ;
}
void Game::Move(Orient ori)
{
	//确定可以执行移动操作
	//先保存上一步
	Copy_Pro_State(last_data);
	Copy_Pro_ID(last_ID);
	last_orient = ori;
	for(int i = 0; i < 4;i ++)
	{
		Move_a_string(ori, i);
	}
	Record_change();
	return ;
}
void Game::Move_a_string(Orient ori, int ord)
{
	//注意重排
	int n = 0;
	int num[4] = {0};
	int tID[4] = {0};
	switch (ori)
	{
	case UP://从下到上
		{
			//提取
			for(int i = 0; i < 4; i ++)
			{
				if(data[3 - i][ord] != 0)
				{
					num[n] = data[3 - i][ord];
					tID[n] = ID[3 - i][ord]; 
					n ++;
				}
			}
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[n - 1 - i][ord] = num[i];
					ID[n - 1 - i][ord] = tID[i];
				}
				else
				{
					data[(n - 1 - i + 4 ) % 4][ord] = 0;
					ID[(n - 1 - i + 4) % 4][ord] = 0; 
				}
			}
			break;
		}

	case DOWN:
		{
			for(int i = 0; i < 4; i ++)
			{
				if(data[i][ord] != 0)
				{
					num[n] = data[i][ord];
					tID[n] = ID[i][ord]; 
					n ++;
				}
			}
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[i + 4 - n][ord] = num[i];
					ID[i + 4 - n][ord] = tID[i];
				}
				else
				{
					data[(i + 4 - n) % 4][ord] = 0;
					ID[(i + 4 - n) % 4][ord] = 0;
				}
			}
			break;
		}

	case LEFT://从右到左
		{
			for(int i = 0; i < 4; i ++)
			{
				if(data[ord][3 - i] != 0)
				{
					num[n] = data[ord][3 - i];
					tID[n] = ID[ord][3 - i];
					n ++;
				}
			}
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[ord][n - 1 - i] = num[i];
					ID[ord][n - 1 - i] = tID[i];
				}
				else
				{
					data[ord][(n - 1 - i + 4) % 4] = 0;
					ID[ord][(n - 1 - i + 4) % 4] = 0;
				}
			}
			break;
		}
	case RIGHT:
		{
			for(int i = 0; i < 4; i ++)
			{
				if(data[ord][i] != 0)
				{
					num[n] = data[ord][i];
					tID[n] = ID[ord][i];
					n ++;
				}
			}
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[ord][i + 4 - n] = num[i];
					ID[ord][i + 4 - n] = tID[i];
				}
				else
				{
					data[ord][(i + 4 - n) % 4] = 0;
					ID[ord][(i + 4 - n) % 4] = 0;
				}
			}
			break;

		}
	}

	combine[ord] = process_a_string(&n, num, tID);

	//处理完毕后还原
	switch (ori)
	{
	case UP://从下到上
		{
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[n - 1 - i][ord] = num[i];
					ID[n - 1 - i][ord] = tID[i];
				}
				else
				{
					data[(n - 1 - i + 4 ) % 4][ord] = 0;
					ID[(n - 1 - i + 4) % 4][ord] = 0; 
				}
			}
			break;
		}

	case DOWN:
		{

			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[i + 4 - n][ord] = num[i];
					ID[i + 4 - n][ord] = tID[i];
				}
				else
				{
					data[(i + 4 - n) % 4][ord] = 0;
					ID[(i + 4 - n) % 4][ord] = 0;
				}
			}
			break;
		}

	case LEFT://从右到左
		{
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[ord][n - 1 - i] = num[i];
					ID[ord][n - 1 - i] = tID[i];
				}
				else
				{
					data[ord][(n - 1 - i + 4) % 4] = 0;
					ID[ord][(n - 1 - i + 4) % 4] = 0;
				}
			}
			break;
		}
	case RIGHT:
		{
			//整理后赋值
			for(int i = 0; i < 4; i ++)
			{
				if(i < n)
				{
					data[ord][i + 4 - n] = num[i];
					ID[ord][i + 4 - n] = tID[i];
				}
				else
				{
					data[ord][(i + 4 - n) % 4] = 0;
					ID[ord][(i + 4 - n) % 4] = 0;
				}
			}
			break;

		}
	}
}
void Game::Record_change(void)
{
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			if(last_data[i][ii] == data[i][ii])
				is_change[i][ii] = 0;
			else
				is_change[i][ii] = 1;
		}
	}
}
int Game::fake_process_a_string(int* n, int* num)
{
	int k = *n;
	for(int i = 0; i < ((*n) - 1); i ++)
	{
		if(num[i] == num[i + 1])
		{
			num[i] *= 2;
			for(int ii = i + 1; ii < (*n) - 1; ii ++)
			{
				num[ii] = num[ii + 1];
				//tID[ii] = tID[ii + 1];
			}
			*n = (*n) - 1;
		}
	}
	return k - *n;
}


int Game::process_a_string(int* n, int* num, int* tID)
{
	//使用新的合成规则
#ifdef OLD_RULE
	int u = 0;
	//数据前移 是 从前往后
	for(int i = 0; i < ((*n) - 1) && !u; i ++)
	{
		if(num[i] == num[i + 1])
		{
			if(real)
			{
				elem --;
				score += num[i] * 2;
			}
			num[i] *= 2;
			for(int ii = i + 1; ii < (*n) - 1; ii ++)
			{
				num[ii] = num[ii + 1];
			}
			*n = (*n) - 1;
			u = 1;
		}
	}
	if(u == 1)
		return process_a_string(n, num, real);
	else
		return 0;
#else
	int k = *n;
	for(int i = 0; i < ((*n) - 1); i ++)
	{
		if(num[i] == num[i + 1])
		{
			if(!has_combine)
				has_combine = 1;
			elem --;
			score += num[i] * 2;
			num[i] *= 2;
			for(int ii = i + 1; ii < (*n) - 1; ii ++)
			{
				num[ii] = num[ii + 1];
				//tID[ii] = tID[ii + 1];
			}
			*n = (*n) - 1;
		}
	}
	return k - *n;
#endif
}
void Game::show_game(CDC* pDC, CFont* f)
{
	//音效
	if(has_combine)
		PlaySound((LPTSTR)(IDR_COB),NULL,SND_RESOURCE | SND_ASYNC);
	else
		PlaySound((LPTSTR)(IDR_SLIP),NULL,SND_RESOURCE | SND_ASYNC);
	has_combine = 0;


#ifdef NO_ANIMATE



	pDC->SetBkColor(RGB(240, 240, 240));
	pDC->SelectObject(f);
	CString ti;
	ti.Format("当前分数: %d ",score);
	pDC->TextOutA(S_X, 15, ti);
	pDC->SetTextColor(RGB(255,255,255));
	for(int i = 0; i < 4;i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			//基本数据
			int v = data[i][ii];

			//皮肤重绘
#ifdef NO_REMOD
			COLORREF clr;
			if(v != 0)
			{
				int wr = 255 / (antipower(v) + 1);
				clr = RGB(wr ,wr ,wr);
			}
			else 
				clr = RGB(198, 198, 198);
			if(i == n_y && ii == n_x)//新格
			{
				pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(255, 0, 0));
				//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
				pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
			}
			else
			{
				pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
				//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
				pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
			}		
#else 
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);


			if(v == 0)//空格
			{
				pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
				//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
				pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
			}
			else // 数字
			{
				switch (v)
				{
				case 2:
					{
						memDC.SelectObject(&bmp[1]);
						break;
					}
				case 4:
					{
						memDC.SelectObject(&bmp[2]);
						break;
					}
				case 8:
					{
						memDC.SelectObject(&bmp[3]);
						break;
					}
				case 16:
					{
						memDC.SelectObject(&bmp[4]);
						break;
					}
				case 32:
					{
						memDC.SelectObject(&bmp[5]);
						break;
					}
				case 64:
					{
						memDC.SelectObject(&bmp[6]);
						break;
					}
				case 128:
					{
						memDC.SelectObject(&bmp[7]);
						break;
					}
				case 256:
					{
						memDC.SelectObject(&bmp[8]);
						break;
					}
				case 512:
					{
						memDC.SelectObject(&bmp[9]);
						break;
					}
				case 1024:
					{
						memDC.SelectObject(&bmp[10]);
						break;
					}
				default:
					{
						memDC.SelectObject(&bmp[10]);
						break;
					}
				}

				if(i == n_y && ii == n_x)//新格
				{
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(255, 0, 0));
					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					//pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
					pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);
				}
				else
				{
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);
				}

			}
			//REMOD完毕
#endif



			//数字显示
			if(v != 0)
			{
				CString h;
				h.Format("%d", v);
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 - 2, h);
				pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 + 2, h);
				pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 + 2, h);
				pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 - 2, h);
				pDC->SetTextColor(RGB(255, 255, 255));
				pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
				//h.Format("%d", ID[i][ii]);
				//pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 45, h);

			}
		}
	}
	/*
	CString t;
	t.Format(" %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d",data[0][0], data[0][1],data[0][2], data[0][3], data[1][0], data[1][1], data[1][2], data[1][3], data[2][0], data[2][1], data[2][2], data[2][3], data[3][0], data[3][1], data[3][2], data[3][3]);
	AfxMessageBox(t);
	*/
	return ;
#else//动画

	HWND hwnd = FindWindow(NULL, "2048第一版");

	CRect rectDlg;
	GetClientRect(hwnd, rectDlg);
	int cx = rectDlg.right - rectDlg.left;
	int cy = rectDlg.bottom - rectDlg.top;





	pDC->SelectObject(f);
	pDC->SetBkColor(RGB(240, 240, 240));

	CString ti;
	ti.Format("当前分数: %d ",score);
	pDC->TextOutA(S_X, 15, ti);





	CDC memDC;
	memDC.CreateCompatibleDC(pDC);





	for(int z = 1; z <= 10; z ++)
	{
		//正常绘制
		pDC->SetTextColor(RGB(255,255,255));

		int i;
		int ii;

		for(int q = 0; q < 4; q ++)
		{
			for(int qq = 0; qq < 4; qq ++)
			{

				//转换顺序
				if(last_orient == UP)//从下到上
				{
					i = 3 - q;
					ii = qq;
				}
				else if(last_orient == DOWN)
				{
					i = q;
					ii = qq;
				}
				else if(last_orient == LEFT)
				{
					i = q;
					ii = 3 - qq;
				}
				else if(last_orient == RIGHT)
				{
					i = q;
					ii = qq;
				}
				else
				{
					AfxMessageBox("未知的方向信息");
					exit(0);
				}


				//基本数据
				int v = data[i][ii];
				int id = ID[i][ii];

			//皮肤重绘
#ifdef NO_REMOD
				//颜色选择
				COLORREF clr;
				if(v != 0)
				{
					int wr = 255 / (antipower(v) + 1);
					clr = RGB(wr ,wr ,wr);
				}
				else 
					clr = RGB(198, 198, 198);


				//重绘新格动画
				if(i == n_y && ii == n_x)
				{
					int x = S_X + ii * LOS + (10 - z) * 0.05 * LOS + 5;
					int y = S_Y + i * LOS + (10 - z) * 0.05 * LOS + 5;
					COLORREF clr2 = RGB(255 * 0.1 * z + 198 * 0.1 * (10 - z),
						      198 * 0.1 * (10 - z),
							  198 * 0.1 * (10 - z));

					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS , LOS, LOS, clr2);//外正方形框
					pDC->FillSolidRect(S_X + ii * LOS + 5, S_Y + i * LOS + 5, LOS - 10, LOS - 10, RGB(198, 198, 198));//外正方形框
					pDC->FillSolidRect(x, y, ILOS * z * 0.1 - 4, ILOS * z * 0.1 - 4,clr);//内正方形

					//仅在最后一次加上数字
					if(z == 10)
					{
						CString h;
						h.Format("%d", v);
						pDC->SetBkMode(TRANSPARENT);
						pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
					}
				}


				else // 其他包括可以移动的
				{
					if(0 == is_change[i][ii])  //本格没有变化 ---有没有经过它的变化
					{
						if(last_orient == UP || last_orient == DOWN) //上下方向
						{
							if(!is_change[0][ii] && !is_change[1][ii] && !is_change[2][ii] &&! is_change[3][ii])//正行没变
							{
								if(z == 1)//就画第一次
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
								}
								else //其他次不变
									;
							}
							else //其他正常绘制
							{
								pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
								//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
								pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);

							}
						}
						else if(last_orient == LEFT || last_orient == RIGHT)
						{
							if(!is_change[i][0] && !is_change[i][1] && !is_change[i][2] &&! is_change[i][3])//正行没变
							{
								if(z == 1)//就画第一次
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
								}
								else //其他次不变
									;
							}
							else //其他正常绘制
							{
								pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
								//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
								pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);

							}
						}
						if(v != 0)
						{
							CString h;
							h.Format("%d ", v);
							pDC->SetBkMode(TRANSPARENT);
							pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
						}
					}
					else if(1 == is_change[i][ii] && v)//移动的 且有值
					{
						//先画一个空格
						pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
						pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
						

						int l_x = 0;
						int l_y = 0;
						int is_find = 0;
						for(int t = 0; t < 4 && !is_find; t ++)
						{
							for(int tt = 0; tt < 4 && !is_find; tt ++)
							{
								if(id == last_ID[t][tt])
								{
									is_find = 1;
									l_x = tt;
									l_y = t;
								}
							}
						}
						if(!is_find) //已经被吞并了
							;
						else   // 没有被吞并
						{
							int cur_x = (S_X + l_x * LOS + 3) * (1 - 0.1 * z)  + (S_X + ii * LOS + 3)*(0.1 * z);
							int cur_y = (S_Y + l_y * LOS + 3) * (1 - 0.1 * z)  + (S_Y + i * LOS + 3)*(0.1 * z);
							pDC->FillSolidRect(cur_x, cur_y, LOS - 6, LOS - 6, clr);

							CString h;
							h.Format("%d ", v);
							pDC->SetBkMode(TRANSPARENT);
							pDC->TextOutA(cur_x + 15, cur_y + 15, h);

						}

					}
					else
					{
						pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
						pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));					
					}
				}		
				//暂时不处理
#else 

				//测试用代码


				//画刷选择
				if(v == 0)//空格
				{
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
				}
				else // 数字
				{
					switch (v)
					{
					case 2:
						{
							memDC.SelectObject(&bmp[1]);
							break;
						}
					case 4:
						{
							memDC.SelectObject(&bmp[2]);
							break;
						}
					case 8:
						{
							memDC.SelectObject(&bmp[3]);
							break;
						}
					case 16:
						{
							memDC.SelectObject(&bmp[4]);
							break;
						}
					case 32:
						{
							memDC.SelectObject(&bmp[5]);
							break;
						}
					case 64:
						{
							memDC.SelectObject(&bmp[6]);
							break;
						}
					case 128:
						{
							memDC.SelectObject(&bmp[7]);
							break;
						}
					case 256:
						{
							memDC.SelectObject(&bmp[8]);
							break;
						}
					case 512:
						{
							memDC.SelectObject(&bmp[9]);
							break;
						}
					case 1024:
						{
							memDC.SelectObject(&bmp[10]);
							break;
						}
					default:
						{
							memDC.SelectObject(&bmp[10]);
							break;
						}
					}
					if(i == n_y && ii == n_x)
					{
						int x = S_X + ii * LOS + (10 - z) * 0.05 * LOS + 5;
						int y = S_Y + i * LOS + (10 - z) * 0.05 * LOS + 5;
						COLORREF clr2 = RGB(255 * 0.1 * z + 198 * 0.1 * (10 - z),
									198 * 0.1 * (10 - z),
									198 * 0.1 * (10 - z));

						pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS , LOS, LOS, clr2);//外正方形框
						pDC->FillSolidRect(S_X + ii * LOS + 5, S_Y + i * LOS + 5, LOS - 10, LOS - 10, RGB(198, 198, 198));//外正方形框

						pDC->BitBlt(x, y, ILOS * z * 0.1 - 4, ILOS * z * 0.1 - 4, &memDC, 0, 0, SRCCOPY);

						//仅在最后一次加上数字
						if(z == 10)
						{
							CString h;
							h.Format("%d ", v);
							pDC->SetBkMode(TRANSPARENT);
							pDC->SetTextColor(RGB(0, 0, 0));
							pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 + 2, h);
							pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 - 2, h);
							pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 + 2, h);
							pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 - 2, h);

							pDC->SetTextColor(RGB(255, 255, 255));
							pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
						}
					}
					/*
					else
					{
						pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
						//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
						pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);
					}
					*/
					else // 其他包括可以移动的
					{
						if(0 == is_change[i][ii])  //本格没有变化 ---有没有经过它的变化
						{
							if(last_orient == UP || last_orient == DOWN) //上下方向
							{
								if(!is_change[0][ii] && !is_change[1][ii] && !is_change[2][ii] &&! is_change[3][ii])//正行没变
								{
									if(z == 1)//就画第一次
									{
										pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));//框 
										//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
										pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);//图
									}
									else //其他次不变
										;
								}
								else //其他正常绘制
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);

								}
							}
							else if(last_orient == LEFT || last_orient == RIGHT)
							{
								if(!is_change[i][0] && !is_change[i][1] && !is_change[i][2] &&! is_change[i][3])//正行没变
								{
									if(z == 1)//就画第一次
									{
										pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
										//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
										pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);
									}
									else //其他次不变
										;
								}
								else //其他正常绘制
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);

								}
							}
							if(v != 0)
							{
								CString h;
								h.Format("%d ", v);
								pDC->SetBkMode(TRANSPARENT);
								pDC->SetTextColor(RGB(0, 0, 0));
								pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 + 2, h);
								pDC->TextOutA(S_X + ii * LOS + 15 + 2, S_Y + i * LOS + 15 - 2, h);
								pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 + 2, h);
								pDC->TextOutA(S_X + ii * LOS + 15 - 2, S_Y + i * LOS + 15 - 2, h);

								pDC->SetTextColor(RGB(255, 255, 255));
								pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
							}
						}
						else if(1 == is_change[i][ii] && v)//移动的 且有值
						{
							//先画一个空格
							pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
							pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
						

							int l_x = 0;
							int l_y = 0;
							int is_find = 0;
							for(int t = 0; t < 4 && !is_find; t ++)
							{
								for(int tt = 0; tt < 4 && !is_find; tt ++)
								{
									if(id == last_ID[t][tt])
									{
										is_find = 1;
										l_x = tt;
										l_y = t;
									}
								}
							}
							if(!is_find) //已经被吞并了
								;
							else   // 没有被吞并
							{
								int cur_x = (S_X + l_x * LOS + 3) * (1 - 0.1 * z)  + (S_X + ii * LOS + 3)*(0.1 * z);
								int cur_y = (S_Y + l_y * LOS + 3) * (1 - 0.1 * z)  + (S_Y + i * LOS + 3)*(0.1 * z);
								pDC->BitBlt(cur_x, cur_y, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);

								CString h;
								h.Format("%d ", v);
								pDC->SetBkMode(TRANSPARENT);
								pDC->SetTextColor(RGB(0, 0, 0));
								pDC->TextOutA(cur_x + 15 + 2, cur_y + 15 + 2, h);
								pDC->TextOutA(cur_x + 15 + 2, cur_y + 15 - 2, h);
								pDC->TextOutA(cur_x + 15 - 2, cur_y + 15 + 2, h);
								pDC->TextOutA(cur_x + 15 - 2, cur_y + 15 - 2, h);

								pDC->SetTextColor(RGB(255, 255, 255));
								pDC->TextOutA(cur_x + 15, cur_y + 15, h);

							}

						}
						else//无值
						{
							pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
							pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));					
						}
					}

				}
			//REMOD完毕
#endif

			//数字显示
			}
		}
		Sleep(28);
	}
#endif
}

Game::~Game()
{
	;
}
BOOL Game::Check_GG(void)
{
	for(int i = 0; i < 3; i ++)
	{
		for(int ii = 0; ii < 3; ii ++)
		{
			int th = data[i][ii];
			if(th == data[i][ii + 1])
				return FALSE;
			else if(th == data[i + 1][ii])
				return FALSE;
		}
	}
	for(int i = 0; i < 3; i ++)
	{
		int th = data[3][i];
		if(th == data[3][i + 1])
			return FALSE;
	}
	for(int i = 0; i < 3; i ++)
	{
		int th = data[i][3];
		if(th == data[i + 1][3])
			return FALSE;
	}
	gg = 1;
	return TRUE;
}

BOOL Game::Check_Similar(int pro_move[][4])
{
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			if(pro_move[i][ii] != data[i][ii])
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

void Game::Copy_Pro_State(int pro_move[][4])
{
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			pro_move[i][ii] = data[i][ii];
		}
	}
	return ;
}

void Game ::Copy_Pro_ID(int pro_ID[][4])
{
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			pro_ID[i][ii] = ID[i][ii];
		}
	}
}

BOOL Game::pro_Move(Orient ori, int data[][4])
{
	//CString t;
	//int data[4][4] = {0};
	Copy_Pro_State(data);

	for(int z = 0; z < 4;z ++)
	{
		int ord = z;
		int n = 0;
		int num[4] = {0};
		switch (ori)
		{
		case UP://从下到上
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[3 - i][ord] != 0)
					{
						num[n] = data[3 - i][ord];
						n ++;
					}
				}
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[n - 1 - i][ord] = num[i];
					else
						data[(n - 1 - i + 4 ) % 4][ord] = 0;
				}
				break;
			}
		case DOWN:
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[i][ord] != 0)
					{
						num[n] = data[i][ord];
						n ++;
					}
				}
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[i + 4 - n][ord] = num[i];
					else
						data[(i + 4 - n) % 4][ord] = 0;
				}
				break;
			}

		case LEFT://从右到左
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[ord][3 - i] != 0)
					{
						num[n] = data[ord][3 - i];
						n ++;
					}
				}
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[ord][n - 1 - i] = num[i];
					else
						data[ord][(n - 1 - i + 4) % 4] = 0;
				}
				break;
			}
		case RIGHT:
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[ord][i] != 0)
					{
						num[n] = data[ord][i];
						n ++;
					}
				}
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[ord][i + 4 - n] = num[i];
					else
						data[ord][(i + 4 - n) % 4] = 0;
				}
				break;

			}
		}
		fake_process_a_string(&n, num);

		//处理完毕后还原
		switch (ori)
		{
		case UP://从下到上
			{
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[n - 1 - i][ord] = num[i];
					else
						data[(n - 1 - i + 4) % 4][ord] = 0;
				}
				break;
			}
		case DOWN:
			{
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[i + 4 - n][ord] = num[i];
					else
						data[(i + 4 - n) % 4][ord] = 0;
				}
				break;
			}

		case LEFT://从右到左
			{
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[ord][n - 1 - i] = num[i];
					else
						data[ord][(n - 1 - i + 4) % 4] = 0;
				}
				break;
			}
		case RIGHT:
			{
				//整理后赋值
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[ord][i + 4 - n] = num[i];
					else
						data[ord][(i + 4 - n) % 4] = 0;
				}
				break;
			}
		}
	}
	//t.Format(" %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d",data[0][0], data[0][1],data[0][2], data[0][3], data[1][0], data[1][1], data[1][2], data[1][3], data[2][0], data[2][1], data[2][2], data[2][3], data[3][0], data[3][1], data[3][2], data[3][3]);
	//AfxMessageBox(t);

	//t.Format(" %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d \r\n %d %d %d %d",this->data[0][0], this->data[0][1],this->data[0][2], this->data[0][3], this->data[1][0], this->data[1][1], this->data[1][2], this->data[1][3], this->data[2][0], this->data[2][1], this->data[2][2], this->data[2][3], this->data[3][0], this->data[3][1], this->data[3][2], this->data[3][3]);
	//AfxMessageBox(t);

	//预移动处理完毕
	return !Check_Similar(data);
		//return FALSE;
}




//int AI::move_or_not (Game& g,Orient ori)
//{
	/*
	if(!g.pro_Move(ori, ))
		return -9999;
	else
		return 1;
		*/
//}

int AI::where_the_max(Game& g,int pattren [][4])
{
	int max = 0;
	int p_y = 0; //先纵再横
	int p_x = 0;
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			if(pattren[i][ii] > max)
			{
				max = pattren[i][ii];
				p_y = i;
				p_x = ii;
			}
		}
	}
	if( (0 == p_x && 0 == p_y) ||
		(0 == p_x && 3 == p_y) ||
		(3 == p_x && 0 == p_y) ||
		(3 == p_x && 3 == p_y) )
		return 3;
	else if((1 == p_x && 1 == p_y) ||
			(1 == p_x && 2 == p_y) ||
			(2 == p_x && 1 == p_y) ||
			(2 == p_x && 2 == p_y) )
		return 1;
	else
		return 2;
}

//时机 待定 等待询问大佬 
//该主体基本完成
int AI::evenness(Game& g, int pattren[][4])
{
	g.Copy_Pro_State(pattren);
	int soft_threshold_funciton = 0;
	//不初始化还敢自加 您真的是学皮了斜眼笑

	if (pattren[0][0] <= pattren[0][1] && pattren[0][1] <= pattren[0][2] && pattren[0][2] <= pattren[0][3])
		soft_threshold_funciton += 1;

	if (pattren[1][0] <= pattren[1][1] && pattren[1][1] <= pattren[1][2] && pattren[1][2] <= pattren[1][3])
		soft_threshold_funciton += 1;

	if (pattren[2][0] <= pattren[2][1] && pattren[2][1] <= pattren[2][2] && pattren[2][2] <= pattren[2][3])
		soft_threshold_funciton += 1;

	if (pattren[3][0] <= pattren[3][1] && pattren[3][1] <= pattren[3][2] && pattren[3][2] <= pattren[3][3])
		soft_threshold_funciton += 1;

	if (pattren[0][0] >= pattren[0][1] && pattren[0][1] >= pattren[0][2] && pattren[0][2] >= pattren[0][3])
		soft_threshold_funciton += 1;

	if (pattren[1][0] >= pattren[1][1] && pattren[1][1] >= pattren[1][2] && pattren[1][2] >= pattren[1][3])
		soft_threshold_funciton += 1;

	if (pattren[2][0] >= pattren[2][1] && pattren[2][1] >= pattren[2][2] && pattren[2][2] >= pattren[2][3])
		soft_threshold_funciton += 1;

	if (pattren[3][0] >= pattren[3][1] && pattren[3][1] >= pattren[3][2] && pattren[3][2] >= pattren[3][3])
		soft_threshold_funciton += 1;  //横着判断平滑程度可增可减，竖着也得来一次；

	if (pattren[0][0] <= pattren[1][0] && pattren[1][0] <= pattren[2][0] && pattren[2][0] <= pattren[3][0])
		soft_threshold_funciton += 1;

	if (pattren[0][1] <= pattren[1][1] && pattren[1][1] <= pattren[2][1] && pattren[2][1] <= pattren[3][1])
		soft_threshold_funciton += 1;

	if (pattren[0][2] <= pattren[1][2] && pattren[1][2] <= pattren[2][2] && pattren[2][2] <= pattren[3][2])
		soft_threshold_funciton += 1;

	if (pattren[0][3] <= pattren[1][3] && pattren[1][3] <= pattren[2][3] && pattren[2][3] <= pattren[3][3])
		soft_threshold_funciton += 1;

	if (pattren[0][0] >= pattren[1][0] && pattren[1][0] >= pattren[2][0] && pattren[2][0] >= pattren[3][0])
		soft_threshold_funciton += 1;

	if (pattren[0][1] >= pattren[1][1] && pattren[1][1] >= pattren[2][1] && pattren[2][1] >= pattren[3][1])
		soft_threshold_funciton += 1;

	if (pattren[0][2] >= pattren[1][2] && pattren[1][2] >= pattren[2][2] && pattren[2][2] >= pattren[3][2])
		soft_threshold_funciton += 1;

	if (pattren[0][3] >= pattren[1][3] && pattren[1][3] >= pattren[2][3] && pattren[2][3] >= pattren[3][3])
		soft_threshold_funciton += 1;

	return soft_threshold_funciton;
}

int AI::neighborhood(Game& g, int pattren[][4]) //相邻关系比值；  
{
	//========================================================================这TM不是我写的========================================================================
    int power = 0;
    if(pattren[0][0] / 2 == pattren[0][1] || pattren[0][1] / 2 == pattren[0][0]) 
		power++;
    if(pattren[0][1] / 2 == pattren[0][2] || pattren[0][2] / 2 == pattren[0][1]) 
		power++;
	if(pattren[0][2] / 2 == pattren[0][3] || pattren[0][3] / 2 == pattren[0][2]) 
	    power++; //第一排；
 
    if(pattren[1][0] / 2 == pattren[1][1] || pattren[1][1] / 2 == pattren[1][0]) 
		power++;
    if(pattren[1][1] / 2 == pattren[1][2] || pattren[1][2] / 2 == pattren[1][1]) 
		power++;
    if(pattren[1][2] / 2 == pattren[1][3] || pattren[1][3] / 2 == pattren[1][2]) 
		power++; //第二排；

	if(pattren[2][0] / 2 == pattren[2][1] || pattren[2][1] / 2 == pattren[2][0]) 
	    power++;
    if(pattren[2][1] / 2 == pattren[2][2] || pattren[2][2] / 2 == pattren[2][1]) 
		power++;
    if(pattren[2][2] / 2 == pattren[2][3] || pattren[2][3] / 2 == pattren[2][2]) 
		power++; //第三排；
    if(pattren[3][0] / 2 == pattren[3][1] || pattren[3][1] / 2 == pattren[3][0]) 
		power++;
    if(pattren[3][1] / 2 == pattren[3][2] || pattren[3][2] / 2 == pattren[3][1]) 
		power++;
    if(pattren[3][2] / 2 == pattren[3][3] || pattren[3][3] / 2 == pattren[3][2]) 
		power++; //第四排；

    if(pattren[0][0] / 2 == pattren[1][0] || pattren[1][0] / 2 == pattren[0][0]) 
		power++;
    if(pattren[1][0] / 2 == pattren[2][0] || pattren[2][0] / 2 == pattren[1][0]) 
		power++;
    if(pattren[2][0] / 2 == pattren[3][0] || pattren[3][0] / 2 == pattren[2][0]) 
		power++; //第一列；

    if(pattren[0][1] / 2 == pattren[1][1] || pattren[1][1] / 2 == pattren[0][1]) 
	    power++;
    if(pattren[1][1] / 2 == pattren[2][1] || pattren[2][1] / 2 == pattren[1][1]) 
		power++;
    if(pattren[2][1] / 2 == pattren[3][1] || pattren[3][1] / 2 == pattren[2][1]) 
        power++; //第二列；

    if(pattren[0][2] / 2 == pattren[1][2] || pattren[1][2] / 2 == pattren[0][2]) 
		power++;
    if(pattren[1][2] / 2 == pattren[2][2] || pattren[2][2] / 2 == pattren[1][2]) 
		power++;
    if(pattren[2][2] / 2 == pattren[3][2] || pattren[3][1] / 2 == pattren[2][2]) 
		power++; //第三列；
    if(pattren[0][3] / 2 == pattren[1][3] || pattren[1][3] / 2 == pattren[0][3]) 
		power++;
    if(pattren[1][3] / 2 == pattren[2][3] || pattren[2][3] / 2 == pattren[1][3]) 
		power++;
    if(pattren[2][3] / 2 == pattren[3][3] || pattren[3][3] / 2 == pattren[2][3]) 
		power++; //第四列；
    return power; 
}
int AI::check_element(Game& g, int pattren[][4])
{
	int e = 0;
	for(int i = 0; i < 4; i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			if(0 != pattren[i][ii])
			{
				e ++;
			}
		}
	}
	return e;
}

void AI::One_Turn(Game& g)
{
	int most_value = -99;
	Orient m_v_orient = 1;
	for(int i = 1; i <= 4; i ++)
	{
		
		int state[4][4] = {0};
		int v = rand();
		if(g.pro_Move(i, state) == TRUE)//改变了
		{
			v = P1 * where_the_max(g, state) 
				 + P2 * evenness(g, state) 
				 + P3 * neighborhood(g, state)  
				 + P4 * check_combine(g, state) 
				 + P5 * max_move_or_not(g, state)
				;
			if(v > most_value)
			{
				most_value = v;
				m_v_orient = i;
			}
		}
		
	}
	if(m_v_orient == UP)
	{
		
		keybd_event(VK_UP, 0, 0, 0 );//模拟按下某个键(调整VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_UP, 0, KEYEVENTF_KEYUP, 0);//模拟抬起某个键
	}
	else if(m_v_orient == DOWN)
	{
		
		keybd_event(VK_DOWN, 0, 0, 0 );//模拟按下某个键(调整VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_DOWN, 0, KEYEVENTF_KEYUP, 0);//模拟抬起某个键
	} 
	else if(m_v_orient == LEFT)
	{
		
		keybd_event(VK_LEFT, 0, 0, 0 );//模拟按下某个键(调整VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_LEFT, 0, KEYEVENTF_KEYUP, 0);//模拟抬起某个键
	}
	else if(m_v_orient == RIGHT)
	{
		
		keybd_event(VK_RIGHT, 0, 0, 0 );//模拟按下某个键(调整VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);//模拟抬起某个键
	}
	//g.Move(m_v_orient);
}
int AI:: check_combine(Game& g, int pattren[][4])
{
	int q = check_element(g, pattren);
	return (g.elem + 1 - q);
}
int AI:: max_move_or_not (Game& g, int pattren[][4])
{
	int c =  where_the_max(g, g.data);//旧格式
	int l =  where_the_max(g, pattren);//新格式
	if(c == 3 && l == 3)
		return 1;
	else if(c != 3 && l == 3)
		return 2;
}

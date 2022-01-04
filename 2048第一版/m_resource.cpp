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
	//��ʼ������
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
	//�����������
	srand(time(NULL));
}
void Game::New_Turn(CDC* pDC, CFont* f)
{
	static int step = 0;
	step ++;
	int value = (rand() % 2 + 1) * 2; //�԰뼸������2 �� 4

	int pos = rand() % (16 - elem) + 1;
	//ע��

	int is_find = 0;

	for(int i = 0; i < 4 && !is_find; i ++)
	{
		for(int ii = 0; ii < 4 && !is_find; ii ++)
		{
			if(0 == data[i][ii])//��λ ���м���
			{
				pos --;
				if(0 == pos)
				{
					data[i][ii] = value;//��ֵ
					ID[i][ii] = step;
					//����µ�λ��
					n_y = i;
					n_x = ii;
					is_find = 1;//����ѭ��
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
	//ȷ������ִ���ƶ�����
	//�ȱ�����һ��
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
	//ע������
	int n = 0;
	int num[4] = {0};
	int tID[4] = {0};
	switch (ori)
	{
	case UP://���µ���
		{
			//��ȡ
			for(int i = 0; i < 4; i ++)
			{
				if(data[3 - i][ord] != 0)
				{
					num[n] = data[3 - i][ord];
					tID[n] = ID[3 - i][ord]; 
					n ++;
				}
			}
			//�����ֵ
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
			//�����ֵ
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

	case LEFT://���ҵ���
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
			//�����ֵ
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
			//�����ֵ
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

	//������Ϻ�ԭ
	switch (ori)
	{
	case UP://���µ���
		{
			//�����ֵ
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

			//�����ֵ
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

	case LEFT://���ҵ���
		{
			//�����ֵ
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
			//�����ֵ
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
	//ʹ���µĺϳɹ���
#ifdef OLD_RULE
	int u = 0;
	//����ǰ�� �� ��ǰ����
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
	//��Ч
	if(has_combine)
		PlaySound((LPTSTR)(IDR_COB),NULL,SND_RESOURCE | SND_ASYNC);
	else
		PlaySound((LPTSTR)(IDR_SLIP),NULL,SND_RESOURCE | SND_ASYNC);
	has_combine = 0;


#ifdef NO_ANIMATE



	pDC->SetBkColor(RGB(240, 240, 240));
	pDC->SelectObject(f);
	CString ti;
	ti.Format("��ǰ����: %d ",score);
	pDC->TextOutA(S_X, 15, ti);
	pDC->SetTextColor(RGB(255,255,255));
	for(int i = 0; i < 4;i ++)
	{
		for(int ii = 0; ii < 4; ii ++)
		{
			//��������
			int v = data[i][ii];

			//Ƥ���ػ�
#ifdef NO_REMOD
			COLORREF clr;
			if(v != 0)
			{
				int wr = 255 / (antipower(v) + 1);
				clr = RGB(wr ,wr ,wr);
			}
			else 
				clr = RGB(198, 198, 198);
			if(i == n_y && ii == n_x)//�¸�
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


			if(v == 0)//�ո�
			{
				pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
				//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
				pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
			}
			else // ����
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

				if(i == n_y && ii == n_x)//�¸�
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
			//REMOD���
#endif



			//������ʾ
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
#else//����

	HWND hwnd = FindWindow(NULL, "2048��һ��");

	CRect rectDlg;
	GetClientRect(hwnd, rectDlg);
	int cx = rectDlg.right - rectDlg.left;
	int cy = rectDlg.bottom - rectDlg.top;





	pDC->SelectObject(f);
	pDC->SetBkColor(RGB(240, 240, 240));

	CString ti;
	ti.Format("��ǰ����: %d ",score);
	pDC->TextOutA(S_X, 15, ti);





	CDC memDC;
	memDC.CreateCompatibleDC(pDC);





	for(int z = 1; z <= 10; z ++)
	{
		//��������
		pDC->SetTextColor(RGB(255,255,255));

		int i;
		int ii;

		for(int q = 0; q < 4; q ++)
		{
			for(int qq = 0; qq < 4; qq ++)
			{

				//ת��˳��
				if(last_orient == UP)//���µ���
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
					AfxMessageBox("δ֪�ķ�����Ϣ");
					exit(0);
				}


				//��������
				int v = data[i][ii];
				int id = ID[i][ii];

			//Ƥ���ػ�
#ifdef NO_REMOD
				//��ɫѡ��
				COLORREF clr;
				if(v != 0)
				{
					int wr = 255 / (antipower(v) + 1);
					clr = RGB(wr ,wr ,wr);
				}
				else 
					clr = RGB(198, 198, 198);


				//�ػ��¸񶯻�
				if(i == n_y && ii == n_x)
				{
					int x = S_X + ii * LOS + (10 - z) * 0.05 * LOS + 5;
					int y = S_Y + i * LOS + (10 - z) * 0.05 * LOS + 5;
					COLORREF clr2 = RGB(255 * 0.1 * z + 198 * 0.1 * (10 - z),
						      198 * 0.1 * (10 - z),
							  198 * 0.1 * (10 - z));

					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS , LOS, LOS, clr2);//�������ο�
					pDC->FillSolidRect(S_X + ii * LOS + 5, S_Y + i * LOS + 5, LOS - 10, LOS - 10, RGB(198, 198, 198));//�������ο�
					pDC->FillSolidRect(x, y, ILOS * z * 0.1 - 4, ILOS * z * 0.1 - 4,clr);//��������

					//�������һ�μ�������
					if(z == 10)
					{
						CString h;
						h.Format("%d", v);
						pDC->SetBkMode(TRANSPARENT);
						pDC->TextOutA(S_X + ii * LOS + 15, S_Y + i * LOS + 15, h);
					}
				}


				else // �������������ƶ���
				{
					if(0 == is_change[i][ii])  //����û�б仯 ---��û�о������ı仯
					{
						if(last_orient == UP || last_orient == DOWN) //���·���
						{
							if(!is_change[0][ii] && !is_change[1][ii] && !is_change[2][ii] &&! is_change[3][ii])//����û��
							{
								if(z == 1)//�ͻ���һ��
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
								}
								else //�����β���
									;
							}
							else //������������
							{
								pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
								//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
								pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);

							}
						}
						else if(last_orient == LEFT || last_orient == RIGHT)
						{
							if(!is_change[i][0] && !is_change[i][1] && !is_change[i][2] &&! is_change[i][3])//����û��
							{
								if(z == 1)//�ͻ���һ��
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, clr);
								}
								else //�����β���
									;
							}
							else //������������
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
					else if(1 == is_change[i][ii] && v)//�ƶ��� ����ֵ
					{
						//�Ȼ�һ���ո�
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
						if(!is_find) //�Ѿ����̲���
							;
						else   // û�б��̲�
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
				//��ʱ������
#else 

				//�����ô���


				//��ˢѡ��
				if(v == 0)//�ո�
				{
					pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
					//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
					pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));
				}
				else // ����
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

						pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS , LOS, LOS, clr2);//�������ο�
						pDC->FillSolidRect(S_X + ii * LOS + 5, S_Y + i * LOS + 5, LOS - 10, LOS - 10, RGB(198, 198, 198));//�������ο�

						pDC->BitBlt(x, y, ILOS * z * 0.1 - 4, ILOS * z * 0.1 - 4, &memDC, 0, 0, SRCCOPY);

						//�������һ�μ�������
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
					else // �������������ƶ���
					{
						if(0 == is_change[i][ii])  //����û�б仯 ---��û�о������ı仯
						{
							if(last_orient == UP || last_orient == DOWN) //���·���
							{
								if(!is_change[0][ii] && !is_change[1][ii] && !is_change[2][ii] &&! is_change[3][ii])//����û��
								{
									if(z == 1)//�ͻ���һ��
									{
										pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));//�� 
										//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
										pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);//ͼ
									}
									else //�����β���
										;
								}
								else //������������
								{
									pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
									//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
									pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);

								}
							}
							else if(last_orient == LEFT || last_orient == RIGHT)
							{
								if(!is_change[i][0] && !is_change[i][1] && !is_change[i][2] &&! is_change[i][3])//����û��
								{
									if(z == 1)//�ͻ���һ��
									{
										pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
										//pDC->RoundRect(CRect(CPoint(S_X + ii * LOS + 3, S_Y + i * LOS + 3), CPoint(S_X + ii * LOS + LOS - 3, S_Y + i * LOS + LOS - 3)),CPoint(15,15));
										pDC->BitBlt(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, &memDC, 0, 0, SRCCOPY);
									}
									else //�����β���
										;
								}
								else //������������
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
						else if(1 == is_change[i][ii] && v)//�ƶ��� ����ֵ
						{
							//�Ȼ�һ���ո�
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
							if(!is_find) //�Ѿ����̲���
								;
							else   // û�б��̲�
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
						else//��ֵ
						{
							pDC->FillSolidRect(S_X + ii * LOS, S_Y + i * LOS, LOS, LOS, RGB(226, 226, 226));
							pDC->FillSolidRect(S_X + ii * LOS + 3, S_Y + i * LOS + 3, LOS - 6, LOS - 6, RGB(198, 198, 198));					
						}
					}

				}
			//REMOD���
#endif

			//������ʾ
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
		case UP://���µ���
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[3 - i][ord] != 0)
					{
						num[n] = data[3 - i][ord];
						n ++;
					}
				}
				//�����ֵ
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
				//�����ֵ
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[i + 4 - n][ord] = num[i];
					else
						data[(i + 4 - n) % 4][ord] = 0;
				}
				break;
			}

		case LEFT://���ҵ���
			{
				for(int i = 0; i < 4; i ++)
				{
					if(data[ord][3 - i] != 0)
					{
						num[n] = data[ord][3 - i];
						n ++;
					}
				}
				//�����ֵ
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
				//�����ֵ
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

		//������Ϻ�ԭ
		switch (ori)
		{
		case UP://���µ���
			{
				//�����ֵ
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
				//�����ֵ
				for(int i = 0; i < 4; i ++)
				{
					if(i < n)
						data[i + 4 - n][ord] = num[i];
					else
						data[(i + 4 - n) % 4][ord] = 0;
				}
				break;
			}

		case LEFT://���ҵ���
			{
				//�����ֵ
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
				//�����ֵ
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

	//Ԥ�ƶ��������
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
	int p_y = 0; //�����ٺ�
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

//ʱ�� ���� �ȴ�ѯ�ʴ��� 
//������������
int AI::evenness(Game& g, int pattren[][4])
{
	g.Copy_Pro_State(pattren);
	int soft_threshold_funciton = 0;
	//����ʼ�������Լ� �������ѧƤ��б��Ц

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
		soft_threshold_funciton += 1;  //�����ж�ƽ���̶ȿ����ɼ�������Ҳ����һ�Σ�

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

int AI::neighborhood(Game& g, int pattren[][4]) //���ڹ�ϵ��ֵ��  
{
	//========================================================================��TM������д��========================================================================
    int power = 0;
    if(pattren[0][0] / 2 == pattren[0][1] || pattren[0][1] / 2 == pattren[0][0]) 
		power++;
    if(pattren[0][1] / 2 == pattren[0][2] || pattren[0][2] / 2 == pattren[0][1]) 
		power++;
	if(pattren[0][2] / 2 == pattren[0][3] || pattren[0][3] / 2 == pattren[0][2]) 
	    power++; //��һ�ţ�
 
    if(pattren[1][0] / 2 == pattren[1][1] || pattren[1][1] / 2 == pattren[1][0]) 
		power++;
    if(pattren[1][1] / 2 == pattren[1][2] || pattren[1][2] / 2 == pattren[1][1]) 
		power++;
    if(pattren[1][2] / 2 == pattren[1][3] || pattren[1][3] / 2 == pattren[1][2]) 
		power++; //�ڶ��ţ�

	if(pattren[2][0] / 2 == pattren[2][1] || pattren[2][1] / 2 == pattren[2][0]) 
	    power++;
    if(pattren[2][1] / 2 == pattren[2][2] || pattren[2][2] / 2 == pattren[2][1]) 
		power++;
    if(pattren[2][2] / 2 == pattren[2][3] || pattren[2][3] / 2 == pattren[2][2]) 
		power++; //�����ţ�
    if(pattren[3][0] / 2 == pattren[3][1] || pattren[3][1] / 2 == pattren[3][0]) 
		power++;
    if(pattren[3][1] / 2 == pattren[3][2] || pattren[3][2] / 2 == pattren[3][1]) 
		power++;
    if(pattren[3][2] / 2 == pattren[3][3] || pattren[3][3] / 2 == pattren[3][2]) 
		power++; //�����ţ�

    if(pattren[0][0] / 2 == pattren[1][0] || pattren[1][0] / 2 == pattren[0][0]) 
		power++;
    if(pattren[1][0] / 2 == pattren[2][0] || pattren[2][0] / 2 == pattren[1][0]) 
		power++;
    if(pattren[2][0] / 2 == pattren[3][0] || pattren[3][0] / 2 == pattren[2][0]) 
		power++; //��һ�У�

    if(pattren[0][1] / 2 == pattren[1][1] || pattren[1][1] / 2 == pattren[0][1]) 
	    power++;
    if(pattren[1][1] / 2 == pattren[2][1] || pattren[2][1] / 2 == pattren[1][1]) 
		power++;
    if(pattren[2][1] / 2 == pattren[3][1] || pattren[3][1] / 2 == pattren[2][1]) 
        power++; //�ڶ��У�

    if(pattren[0][2] / 2 == pattren[1][2] || pattren[1][2] / 2 == pattren[0][2]) 
		power++;
    if(pattren[1][2] / 2 == pattren[2][2] || pattren[2][2] / 2 == pattren[1][2]) 
		power++;
    if(pattren[2][2] / 2 == pattren[3][2] || pattren[3][1] / 2 == pattren[2][2]) 
		power++; //�����У�
    if(pattren[0][3] / 2 == pattren[1][3] || pattren[1][3] / 2 == pattren[0][3]) 
		power++;
    if(pattren[1][3] / 2 == pattren[2][3] || pattren[2][3] / 2 == pattren[1][3]) 
		power++;
    if(pattren[2][3] / 2 == pattren[3][3] || pattren[3][3] / 2 == pattren[2][3]) 
		power++; //�����У�
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
		if(g.pro_Move(i, state) == TRUE)//�ı���
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
		
		keybd_event(VK_UP, 0, 0, 0 );//ģ�ⰴ��ĳ����(����VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_UP, 0, KEYEVENTF_KEYUP, 0);//ģ��̧��ĳ����
	}
	else if(m_v_orient == DOWN)
	{
		
		keybd_event(VK_DOWN, 0, 0, 0 );//ģ�ⰴ��ĳ����(����VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_DOWN, 0, KEYEVENTF_KEYUP, 0);//ģ��̧��ĳ����
	} 
	else if(m_v_orient == LEFT)
	{
		
		keybd_event(VK_LEFT, 0, 0, 0 );//ģ�ⰴ��ĳ����(����VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_LEFT, 0, KEYEVENTF_KEYUP, 0);//ģ��̧��ĳ����
	}
	else if(m_v_orient == RIGHT)
	{
		
		keybd_event(VK_RIGHT, 0, 0, 0 );//ģ�ⰴ��ĳ����(����VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);//ģ��̧��ĳ����
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
	int c =  where_the_max(g, g.data);//�ɸ�ʽ
	int l =  where_the_max(g, pattren);//�¸�ʽ
	if(c == 3 && l == 3)
		return 1;
	else if(c != 3 && l == 3)
		return 2;
}

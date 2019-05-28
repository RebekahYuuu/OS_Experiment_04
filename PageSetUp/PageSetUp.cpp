#include <iostream>
/*#include <iterator>
#include <algorithm>
#include <vector>*/
using namespace std;

int setBlockNum;//���������������Ŀ
int block[20] = {};//�����ţ����20��

int setPageNum;//�趨�����ɵ�ҳ����Ŀ�����10��
int page[10] = {};//����ÿ��ҳ�汣��Ŀ��
int locateFirstUseInTheFuture[10] = {};

struct Page
{
	int sta;///�ڴ�λ��ǰ״̬
	int A;///����λ
	int M;///�޸�λ
	int tim;///������һ�η���ʱ��
	int dis;///������һ�η��ʾ���
} Q[20];

//����û��㷨
void OPT();
//�Ƚ��ȳ��㷨
void FIFO();
//������δʹ��
void LRU();
//�Ľ���CLock�㷨
void AdCLOCK();
//��ʼ���ڴ��е�ҳ��
void initPages();
//��ʼ����λ����
void initLocatFirstUseInTheFuture();
//��ʼ�������
void initBlock();
//�����ǰ�ڴ�ҳ���зֱ�װ��������
void printPages(int i);
//OPTȷ��ÿһ������������λ��
void loadBlockByOPT(int i);
//FIFO��ǰ�������ҳ����
bool IsIn_FIFO(const int* page, const int& num);
//FIFO��ǰ����鲻��ҳ����
void NoIn(int* page, int* seq, const int& num);
//���ȼ��Ҫ�������������Ƿ���ҳ����,����ڣ���loc��������
bool checkIsInPage(const int& num, int* page, int& loc);
//���������
void IsIn_LRU(int* seq, const int loc);
int Search();

int main()
{
	initPages();
	initBlock();
	cout << "�������ڴ��п��ٵ�ҳ������������10��" << endl;
	cin >> setPageNum;
	cout << "�����뼴��ʹ�õ��������Ŀ��������20��" << endl;
	cin >> setBlockNum;
	cout << "���������������У�������20��" << endl;
	int i = 0;
	while (i<setBlockNum)
	{
		cin >> block[i];
		i++;
	}
	int choice = 0;
	cout << "������ѡ���ҳ���û��㷨" << endl;
	cout << "1--------------------OPT" << endl;
	cout << "2-------------------FIFO" << endl;
	cout << "3--------------------LRU" << endl;
	cout << "4---------------AdCLOCK " << endl;
	cin >> choice;
	switch (choice)
	{
	case 1: {OPT(); break; }
	case 2: {FIFO(); break; }
	case 3: {LRU(); break; }
	case 4: {AdCLOCK(); break; }
	default:
		break;
	}
}

//����û��㷨
void OPT()
{
	int i = 0;
	int k = 0;
	int loadInEmpty = 0;//�ڿ�ҳ������������
	int loadByCover = 0;
	int alreadyLoaded = 0;
	//�����ǰ�ڴ�ҳ���зֱ�װ��������
	printPages(i);
	for (i = 0; i < setBlockNum; ++i)//ÿװ��һ������飬����һ�μ���
	{
		initLocatFirstUseInTheFuture();
		//OPTȷ��ÿһ������������λ��
		loadBlockByOPT(i);
		//�����ǰ�ڴ�ҳ���зֱ�װ��������
		printPages(i);
	}
}

//�Ƚ��ȳ��㷨
void FIFO()
{
	int seq[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int i = 0, j = 0;
	//�����ǰ�ڴ�ҳ���зֱ�װ��������
	printPages(i);

	for (i = 0; i < setBlockNum; ++i)//�����ѭ�������������û�����ѭ��
	{

		if (IsIn_FIFO(page, block[i]))//�ȼ���Ƿ����������
			continue;//������ڣ��������κθı�
		else
			NoIn(page, seq, block[i]);//�����������
		//�����ǰ�ڴ�ҳ���зֱ�װ��������
		printPages(i);
	}
}

//������δʹ��
void LRU()
{
	int seq[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int i, loc;
	for (i = 0; i < setBlockNum; ++i)//�����ѭ����������һ��һ������
	{//1
		//printf("��ǰ����ʵ������Ϊ %d \n", block[i]);
		printPages(i);

		loc = -1;//�����������ڣ���������������
		if (checkIsInPage(block[i], page, loc))//���ж��û����Ƿ����ڴ�ҳ����
			IsIn_LRU(seq, loc);//��������ڣ������û������Ⱥ�˳��Ҫ��
		else
			NoIn(page, seq, block[i]);//����������ڣ�Ҫ�û������Ҹı��Ⱥ�˳��
	}//1
}

//clock�㷨
void AdCLOCK()
{
	int j;
	for (int i = 1; i <= setPageNum; i++)
	{
		Q[i].sta = -1;
		Q[i].A = 0;
		Q[i].M = 0;
	}
	for (int i = 1; i <= setBlockNum; i++)
	{
		printf("��ǰ����ʵ������Ϊ %d \n", block[i]);
		if (IsIn_FIFO(page,block[i]))///ҳ���Ѵ������ڴ���
		{
			printf("��ҳ���Ѵ������ڴ���\n");
		}
		else if (Q[setPageNum].sta != -1)///ҳ��δ�������ڴ��в����ڴ�����
		{
			j = Search();
			Q[j].sta = block[i];
			Q[j].A = 1;
		}
		else ///�ڴ�δ��������ҳ��δ�������ڴ���
		{
			for (int k = 1; k <= setPageNum; k++)
			{
				if (Q[k].sta == -1) {
					Q[k].sta = block[i];
					break;
				}
			}
		}
		printf("%d:", i);
		for (int k = 1; k <= setPageNum; k++)
		{
			printf(" %3d  ", Q[k].sta);
		}
		puts("");
	}
}

//��ʼ���ڴ��е�ҳ��
void initPages()
{
	int i = 0;
	for ( i = 0; i < 10; i++)
	{
		page[i] = -1;
	}
}

//��ʼ����λ����
void initLocatFirstUseInTheFuture()
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		locateFirstUseInTheFuture[i] = -1;
	}
}

//��ʼ�������
void initBlock()
{
	int i = 0;
	for (i = 0; i < 20; i++)
	{
		block[i] = -1;
	}
}

//�����ǰ�ڴ�ҳ���зֱ�װ��������
void printPages(int i)
{
	printf("��ǰ����ʵ������Ϊ %d \n", block[i]);
	cout << i << ":";
	for (int k = 0; k < setPageNum; ++k)
	{
		cout << page[k] << " ";
	}
	cout << endl;
}

//OPTȷ��ÿһ������������λ��
void loadBlockByOPT(int i)
{
	int k = 0;
	int loadInEmpty = 0;//�ڿ�ҳ������������
	int loadByCover = 0;
	int alreadyLoaded = 0;
	for (k = 0; k < setPageNum; ++k)//�Ȳ鿴�Ƿ��п�λ
	{
		if (page[k] == -1)//��ǰҳ��Ϊ��
		{
			page[k] = block[i];//��Ҫװ��������
			loadInEmpty = 1;
			break;
		}
	}
	if (loadInEmpty == 1)//��count=1ʱ��ʾ�ղ��Ǹ����Ѿ����λ��ȥ�ˣ���ʱʣ�µ�ѭ��Ҫ������i++������һ��
	{
		loadInEmpty = 0;
		return;
	}
	//��ǰ�ڴ�ҳ������
	//���ж�Ҫ���õ�������Ƿ��Ѿ����ڴ�ҳ����
	for (k = 0; k < setPageNum; ++k)
	{
		if (page[k] == block[i])
		{
			alreadyLoaded = 1;
			break;
		}
	}
	if (alreadyLoaded == 1)//��count=1ʱ��ʾ��ǰ��num[i]�Ѿ��ڿ���,��ǰ����ѭ����i++
	{
		alreadyLoaded = 0;
		return;
	}
	//ҳ�����������ǵ�ǰҪ���õ�����鲻���ڴ�ҳ���ڣ���Ҫ�滻
	if (i < setBlockNum - 1)//�������һ�������
	{
		for (k = 0; k < setPageNum; k++)//�ڵ�ǰ�ڴ�ҳ������δ����ò�����ֵ������
		{
			int m = 0;
			for (m = i + 1; m < setBlockNum; m++)//�ӵ�ǰҪ�����block��һλ����鿪ʼ�Ƚ�
			{
				if (page[k] == block[m])
				{
					locateFirstUseInTheFuture[k] = m;//��¼��ǰpage��װ���������ڽ�����һ�α�ʹ�õ�λ��
					break;
				}
			}
		}
		int replace = 0;
		for (k = 0; k < setPageNum; ++k)//�ҳ���ò�����ֵ�������,�������ֵ
		{
			if (locateFirstUseInTheFuture[k] == -1)
			{
				replace = k;
				break;
			}
			if (locateFirstUseInTheFuture[replace] < locateFirstUseInTheFuture[k])
			{
				replace = k;
			}
		}
		//�滻replaceλ�õ�ҳ��
		page[replace] = block[i];
	}
	else//�����һ������飬�趨�滻��һ��ҳ��
	{
		page[0] = block[i];
	}
}

bool IsIn_FIFO(const int* page, const int& num)
{
	int i;
	for (i = 0; i < setPageNum; ++i)
	{
		if (page[i] == num)
			return true;
	}
	return false;
}

void NoIn(int* page, int* seq, const int& num)
{
	int i = 0, loc = 0;
	for (i = 0; i < setPageNum; ++i)
	{
		if (seq[i] == 1)
		{
			page[i] = num;
			loc = i;
		}
		seq[i] -= 1;
	}
	seq[loc] = setPageNum;
}

//���ȼ��Ҫ�������������Ƿ���ҳ����,����ڣ���loc��������
bool checkIsInPage(const int& num, int* page, int& loc)
{
	int i;
	for (i = 0; i < setPageNum; i++)
	{
		if (num == page[i])
		{
			loc = i;
			return true;
		}
	}
	return false;
}

//���������
void IsIn_LRU(int* seq, const int loc)
{
	int i;
	for (i = 0; i < setPageNum; ++i)
	{
		//if (loc == setPageNum-1)//��������ҳ��ű������Ǹ�ʹ�ù���ʱ����Ҫ�ı�
			//break;
		if (seq[i] == 1)
			continue;
		seq[i] -= 1;
	}
	seq[loc] = setPageNum;
}

int Search()
{
	int j;
	for (j = 1; j <= setPageNum; j++)
	{
		if (Q[j].A == 0 && Q[j].M == 0)
			return j;
	}
	for (j = 1; j <= setPageNum; j++)
	{
		if (Q[j].A == 0 && Q[j].M == 1)
			return j;
		Q[j].A = 0;
	}
	for (j = 1; j <= setPageNum; j++)
	{
		Q[j].A = 0;
	}
	return Search();
}

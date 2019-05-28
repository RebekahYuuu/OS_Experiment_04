#include <iostream>
/*#include <iterator>
#include <algorithm>
#include <vector>*/
using namespace std;

int setBlockNum;//保存输入物理块数目
int block[20] = {};//物理块号，最多20个

int setPageNum;//设定可容纳的页面数目，最多10个
int page[10] = {};//保存每个页面保存的块号
int locateFirstUseInTheFuture[10] = {};

struct Page
{
	int sta;///内存位当前状态
	int A;///访问位
	int M;///修改位
	int tim;///距离上一次访问时长
	int dis;///距离上一次访问距离
} Q[20];

//最佳置换算法
void OPT();
//先进先出算法
void FIFO();
//最近最久未使用
void LRU();
//改进的CLock算法
void AdCLOCK();
//初始化内存中的页面
void initPages();
//初始化定位数组
void initLocatFirstUseInTheFuture();
//初始化物理块
void initBlock();
//输出当前内存页面中分别装入的物理块
void printPages(int i);
//OPT确定每一个物理块的载入位置
void loadBlockByOPT(int i);
//FIFO当前物理块在页面中
bool IsIn_FIFO(const int* page, const int& num);
//FIFO当前物理块不在页面中
void NoIn(int* page, int* seq, const int& num);
//首先检查要换进的物理块号是否在页面中,如果在，用loc返回索引
bool checkIsInPage(const int& num, int* page, int& loc);
//在物理块内
void IsIn_LRU(int* seq, const int loc);
int Search();

int main()
{
	initPages();
	initBlock();
	cout << "请输入内存中开辟的页面数（不超过10）" << endl;
	cin >> setPageNum;
	cout << "请输入即将使用的物理块数目（不超过20）" << endl;
	cin >> setBlockNum;
	cout << "请输入物理块号序列（不超过20）" << endl;
	int i = 0;
	while (i<setBlockNum)
	{
		cin >> block[i];
		i++;
	}
	int choice = 0;
	cout << "请输入选择的页面置换算法" << endl;
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

//最佳置换算法
void OPT()
{
	int i = 0;
	int k = 0;
	int loadInEmpty = 0;//在空页面载入该物理块
	int loadByCover = 0;
	int alreadyLoaded = 0;
	//输出当前内存页面中分别装入的物理块
	printPages(i);
	for (i = 0; i < setBlockNum; ++i)//每装入一个物理块，进行一次检验
	{
		initLocatFirstUseInTheFuture();
		//OPT确定每一个物理块的载入位置
		loadBlockByOPT(i);
		//输出当前内存页面中分别装入的物理块
		printPages(i);
	}
}

//先进先出算法
void FIFO()
{
	int seq[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int i = 0, j = 0;
	//输出当前内存页面中分别装入的物理块
	printPages(i);

	for (i = 0; i < setBlockNum; ++i)//最外层循环，对物理块号置换依次循环
	{

		if (IsIn_FIFO(page, block[i]))//先检查是否在物理块内
			continue;//如果在内，不用做任何改变
		else
			NoIn(page, seq, block[i]);//不在物理块内
		//输出当前内存页面中分别装入的物理块
		printPages(i);
	}
}

//最近最久未使用
void LRU()
{
	int seq[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int i, loc;
	for (i = 0; i < setBlockNum; ++i)//最外层循环，物理块号一个一个进来
	{//1
		//printf("当前需访问的物理块为 %d \n", block[i]);
		printPages(i);

		loc = -1;//如果在物理块内，用来保存其索引
		if (checkIsInPage(block[i], page, loc))//先判断置换号是否在内存页面内
			IsIn_LRU(seq, loc);//在物理块内，不用置换，但先后顺序要变
		else
			NoIn(page, seq, block[i]);//不在物理块内，要置换，而且改变先后顺序
	}//1
}

//clock算法
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
		printf("当前需访问的物理块为 %d \n", block[i]);
		if (IsIn_FIFO(page,block[i]))///页面已存在于内存中
		{
			printf("该页面已存在于内存中\n");
		}
		else if (Q[setPageNum].sta != -1)///页面未存在于内存中并且内存已满
		{
			j = Search();
			Q[j].sta = block[i];
			Q[j].A = 1;
		}
		else ///内存未满，并且页面未存在于内存中
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

//初始化内存中的页面
void initPages()
{
	int i = 0;
	for ( i = 0; i < 10; i++)
	{
		page[i] = -1;
	}
}

//初始化定位数组
void initLocatFirstUseInTheFuture()
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		locateFirstUseInTheFuture[i] = -1;
	}
}

//初始化物理块
void initBlock()
{
	int i = 0;
	for (i = 0; i < 20; i++)
	{
		block[i] = -1;
	}
}

//输出当前内存页面中分别装入的物理块
void printPages(int i)
{
	printf("当前需访问的物理块为 %d \n", block[i]);
	cout << i << ":";
	for (int k = 0; k < setPageNum; ++k)
	{
		cout << page[k] << " ";
	}
	cout << endl;
}

//OPT确定每一个物理块的载入位置
void loadBlockByOPT(int i)
{
	int k = 0;
	int loadInEmpty = 0;//在空页面载入该物理块
	int loadByCover = 0;
	int alreadyLoaded = 0;
	for (k = 0; k < setPageNum; ++k)//先查看是否有空位
	{
		if (page[k] == -1)//当前页面为空
		{
			page[k] = block[i];//将要装入的物理块
			loadInEmpty = 1;
			break;
		}
	}
	if (loadInEmpty == 1)//当count=1时表示刚才那个数已经填到空位里去了，这时剩下的循环要结束，i++进行下一个
	{
		loadInEmpty = 0;
		return;
	}
	//当前内存页面已满
	//先判断要是用的物理块是否已经在内存页面中
	for (k = 0; k < setPageNum; ++k)
	{
		if (page[k] == block[i])
		{
			alreadyLoaded = 1;
			break;
		}
	}
	if (alreadyLoaded == 1)//当count=1时表示当前的num[i]已经在块中,提前结束循环，i++
	{
		alreadyLoaded = 0;
		return;
	}
	//页面已满，但是当前要是用的物理块不在内存页面内，需要替换
	if (i < setBlockNum - 1)//不是最后一个物理块
	{
		for (k = 0; k < setPageNum; k++)//在当前内存页面中找未来最久不会出现的物理块
		{
			int m = 0;
			for (m = i + 1; m < setBlockNum; m++)//从当前要载入的block后一位物理块开始比较
			{
				if (page[k] == block[m])
				{
					locateFirstUseInTheFuture[k] = m;//记录当前page所装入的物理块在将来第一次被使用的位置
					break;
				}
			}
		}
		int replace = 0;
		for (k = 0; k < setPageNum; ++k)//找出最久不会出现的物理块号,即找最大值
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
		//替换replace位置的页面
		page[replace] = block[i];
	}
	else//是最后一个物理块，设定替换第一个页面
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

//首先检查要换进的物理块号是否在页面中,如果在，用loc返回索引
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

//在物理块内
void IsIn_LRU(int* seq, const int loc)
{
	int i;
	for (i = 0; i < setPageNum; ++i)
	{
		//if (loc == setPageNum-1)//在物理块的页面号本来就是刚使用过的时，不要改变
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

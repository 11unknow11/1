#undef UNICODE
#undef _UNICODE

#include <graphics.h>
#include <conio.h>
#include<stdio.h>
#include<time.h>
#include<Windows.h>

int* randomarray(int max, int min, int len);
int merge(int* arr, int L, int M, int R);
int* partition(int* arr, int l, int r);

int len;
float wid;
int* tag;
int* data;

int* randomarray(int max, int min, int len)
{
	int* ret = (int*)malloc(sizeof(int) * len);
	if (ret == NULL)
		exit(0);
	for (int i = 0; i < len; i++)
		ret[i] = rand() % max + min;
	return ret;
}

void draw(int *data,int* tag)
{
	BeginBatchDraw();
	for (int i = 0; i < len; i++)
	{
		if (tag[i])
		{
			setfillcolor(GREEN);
			fillrectangle(i * wid, 480-data[i], (i + 1) * wid, 480);
			setfillcolor(WHITE);
		}
		else
		{
			setfillcolor(WHITE);
			fillrectangle(i * wid, 480-data[i], (i + 1) * wid, 480);
		}
	}
	FlushBatchDraw();
}

void del(int* data, int index)
{
	setfillcolor(BLACK);
	clearrectangle(index * wid, 480-data[index], (index + 1) * wid, 480);
	setfillcolor(WHITE);
}

void drawred(int* data, int index,int way)
{
	BeginBatchDraw();
	if (index > 0&&way==1)
	{
		setfillcolor(WHITE);
		fillrectangle((index - 1) * wid, 480 - data[index - 1], (index) *wid, 480);
	}
	if (index < 108 && way == 2)
	{
		setfillcolor(GREEN);
		fillrectangle((index +1) * wid, 480 - data[index+1], (index+2) * wid, 480);
	}
	setfillcolor(RED);
	fillrectangle(index * wid, 480-data[index], (index + 1) * wid, 480);
	setfillcolor(WHITE);
	FlushBatchDraw();
}

/*void displaytime()
{
	clock_t start_time = clock();
	char str[100];
	int time_1;
	clock_t end_time = clock();
	time_1 = (int)((end_time - start_time) / CLOCKS_PER_SEC);
	_itoa_s(time_1, str, 10);
	outtextxy(0, 0, _T("当前用时:"));
	outtextxy(65, 0, str);
}*/

void sectionsort()
{
	for (int i = 0; i < len-1; i++)
	{
		int min = data[i], p = i;
		for (int j = i + 1; j < len; j++)
		{
			drawred(data, j,1);
			Sleep(10);
			if (data[j] < min)
			{
				min = data[j];
				p = j;
			}
		}
		if (p != i)
		{
			del(data, i);
			del(data, p);
			int temp = data[p];
			data[p] = data[i];
			data[i] = temp;
		}
		if (i == len-1)
			tag[len-1] = 1;
		tag[i] = 1;
		draw(data, tag);
	}
}

void buffersort()
{
	for (int i = 1; i < len; i++)
	{
		for (int j = 0; j < len - i; j++)
		{
			drawred(data, j,1);
			Sleep(10);
			if (data[j] > data[j + 1])
			{
				del(data, j);
				del(data, j + 1);
				int buffer = data[j];
				data[j] = data[j + 1];
				data[j + 1] = buffer; 
			}
			draw(data, tag);
		}
		tag[len - i] = 1;
		if (i == len-1)
			tag[0] = 1;
	}
}

void insertsort()
{
	for (int i = 1; i < len; i++)
	{
		for (int j = i; j > 0; j--)
		{
			drawred(data, j,2);
			Sleep(10);
			if (data[j] >= data[j - 1] || j == 0)
				break;
			else
				if (data[j] < data[j - 1])
				{
					del(data, j);
					del(data, j - 1);
					int temp = data[j];
					data[j] = data[j - 1];
					data[j - 1] = temp;
				}
			draw(data, tag);
		}
		tag[i - 1] = 1;
		if (i == len-2)
			tag[len-1] = 1;
		draw(data, tag);
	}
}

int  mergesort(int* arr, int l, int r)
{
	if (l == r)
		return 0;
	int mid = l + ((r - l) >> 1);
	return mergesort(arr, l, mid) + mergesort(arr, mid + 1, r) + merge(arr, l, mid, r);
}

//归并
int merge(int* arr, int L, int M, int R)
{
	int ret = 0;
	int* help = (int*)calloc(R - L + 1, sizeof(int));
	if (help == NULL)
		exit(-1);
	int i = 0, p1 = L, p2 = M + 1;
	while (p1 <= M && p2 <= R)
		help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
	while (p1 <= M)
		help[i++] = arr[p1++];
	while (p2 <= R)
		help[i++] = arr[p2++];
	for (int i = 0; i < R - L + 1; i++)
	{
		drawred(arr, L + i,1);
		Sleep(20);
		del(arr, L + i);
		
		arr[L + i] = help[i];
		draw(arr, tag);
	}
	free(help);
	return ret;
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void quickSort(int* arr, int l, int r)
{
	if (l < r)
	{
		int rp = l + rand() % (r - l + 1);
		del(arr, rp);
		del(arr, r);
		swap(&arr[rp], &arr[r]);
		int* p = partition(arr, l, r);
		quickSort(arr, l, p[0] - 1);
		quickSort(arr, p[1] + 1, r);
		free(p);
	}
}

int* partition(int* arr, int l, int r)
{
	int* ret = (int*)calloc(2, sizeof(int));
	if (ret == NULL)
		exit(-1);
	int less = l - 1, more = r;
	while (l < more)
	{
		drawred(arr, l, 1);
		Sleep(15);
		if (arr[l] < arr[r])
		{
			del(arr, less + 1);
			del(arr, l);
			swap(&arr[++less], &arr[l++]);
			draw(arr, tag);

		}
		else
			if (arr[l] > arr[r])
			{
				del(arr, more - 1);
				del(arr, l);
				swap(&arr[--more], &arr[l]);
				draw(arr, tag);

			}
			else
				l++;
	}
	del(arr, more);
	del(arr, r);
	swap(&arr[more], &arr[r]);
	draw(arr, tag);
	Sleep(15);
	ret[0] = less + 1;
	ret[1] = more;
	return ret;
}
void end()
{
	setfillcolor(WHITE);
	for (int i = 0; i < len; i++)
	{
		fillrectangle(i * wid, 480 - data[i], (i + 1) * wid, 480);
	}
	FlushBatchDraw();
	setfillcolor(GREEN);
	for (int i = 0; i < len; i++)
	{
		fillrectangle(i * wid, 480 - data[i], (i + 1) * wid, 480);
		Sleep(2);
		FlushBatchDraw();
	}
	
}

int main()
{
	srand(time(0));
	printf("数组长度:");
	scanf_s("%d", &len);
	wid = (float)((float)1080 / (float)len);
	tag = (int*)calloc(len, sizeof(int));
	data = randomarray(400, 30, len);

	HWND hud=initgraph(1080, 480);
	SetWindowText(hud, "排序可视化");
	setlinecolor(BLACK);
	draw(data,tag);
	settextstyle(20 ,0, _T("黑体"));
	outtextxy(0, 0, _T("                     选择排序方式  1.选择排序  2.冒泡排序  3.插入排序  4.归并排序  5.快速排序"));
	BeginBatchDraw();
	FlushBatchDraw();
	char ch;
	ch = _getch();
	switch (ch)
	{
	case '1': {sectionsort();
		end();
		break; 
		}
	case '2': {buffersort(); 
		end();
		break; 
		}
	case '3': {insertsort();
		end();
		break; 
		}
	case '4': {
		mergesort(data, 0, len-1); 
		end();
		break; 
		}
	case '5': {
		quickSort(data, 0, len-1);
		end();
		break;
		}
	}
	
	while (!_kbhit())
	{
	}
	closegraph();
	return 0;
}

/*
	蚁群算法解关键路径问题

	遗传算法解关键路径问题

*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//蚁群相关常量
#define NUM 20	//蚂蚁数量
#define CITY_NUM 9	//城市数量
#define TOTAL_TURN 160	//迭代总轮数
#define QIFA 0.5	//β，启发式信息作用
#define EVER_FAVOR 0.6	//α，信息素作用
#define FAVOR 0.7	//初始信息素浓度比率
#define ZHENGFA 0.5	//蒸发率

//遗传相关常量
#define BREED 20	//种群数量
#define CITY2 9	//城市数量
#define JIAOPEI	0.88	//交配概率
#define BIANYI 0.1	//变异概率
#define TOTAL_TURN2 100	//迭代总轮数

typedef struct graph {
	int edge[CITY_NUM][CITY_NUM];	//两点间路径长度
	//int numV;	//当前顶点数
	//int numE;	//当前边数
	float qifa[CITY_NUM][CITY_NUM] = { 0 };
	float xinxisu[CITY_NUM][CITY_NUM] = { 0 };	//信息素浓度
}graph;

graph *createGraph(graph *G, int city[][CITY_NUM]) {
	int i, j;
	//int v, e;
	//printf("输入顶点数和边数\n");
	//scanf_s("%d %d", &v, &e);
	//G->numE = e; G->numV = v;
	for(i=0;i<CITY_NUM;i++)
		for (j = 0; j < CITY_NUM; j++) {
			if (i >= j) {
				G->edge[i][j] = 0;
				continue;
			}
			//printf("输入（%d,%d）的值：", i, j);
			//scanf_s("%d", &G->edge[i][j]);
			G->edge[i][j] = city[i][j];
		}
	return G;
}

void print_graph(graph G) {
	int i, j;
	//printf("顶点数：%d，边数：%d", G.numV, G.numE);
	for (i = 0; i < CITY_NUM; i++) {
		printf("\n");
		for (j = 0; j < CITY_NUM; j++) {
			printf("%d ", G.edge[i][j]);
		}
	}
}

typedef struct node{
	int nodenum;
	int length;	//两点间路径长度
	node *next;
}node;

typedef struct headnode {
	int headnum;
	int tail_length = 0;
	node *next;
}head[CITY2];

node *createNode() {
	int n, length = 0;
	node *node_now = NULL;
	node_now = (node *)malloc(sizeof(node));
	if (!node_now)return NULL;
	printf("请输入节点号、两点间路径长度");
	scanf("%d %d", &n, &length);
	node_now->nodenum = n;
	node_now->length = length;
	node_now->next = NULL;
	//假设当节点值为-1则释放节点
	if (node_now->nodenum == -1)
		node_now = NULL;
	return node_now;
}

headnode *createHeadNode(head headlist) {
	int i, j, n, count = 0;
	printf("创建头节点结构体数组\n");
	for (i = 0; i < CITY2; i++) {
		printf("请输入头节点号:");
		scanf("%d", &n);
		headlist[i].headnum = n;
		//temp表示下一个节点的地址
		node **temp = &headlist[i].next;
		count = 0;	//清零，重新计数
		for (j = 0;; j++) {
			node *nextnode = createNode();
			//如果下一个节点存在则把nextnode链接上去
			//否则把next设为NULL
			if (nextnode){
				*temp = nextnode;
				temp = &nextnode->next;
				count += 1;	//若加后继节点成功，则计数器+1
			}
			else {
				*temp = NULL;
				break;
			}	
		}
		//记录该点后有几个可到达的点
		headlist[i].tail_length = count;	
	}

	return headlist;
}

//因为实参是结构体数组，因此要传入数组首地址，并利用偏移量来进行操作
void print_linklist(struct headnode *headlist) {
	int i;
	node *temp = NULL;
	for (i = 0; i < CITY_NUM; i++) {
		printf("v%d", (headlist+i)->headnum);
		temp = (headlist+i)->next;
		while (1) {
			if (temp) {
				printf("->%d", temp->nodenum);
				temp = temp->next;
			}
			else {
				printf("\n");
				break;
			}
		}
	}
}

//轮盘赌算法
int wheelSelection(float city[]) {
	float number = 0.0, order;
	int i;
	do {
		number = rand() / (RAND_MAX + 1.0);
	} while (number == 0.0);

	order = 0.0;
	for (i = 0; i < CITY_NUM; i++) {
		if (number > order && number <= order + city[i])
			return i;	//返回城市列表城市下标号
		order += city[i];
	}
}

//测试轮盘赌
void test_of_WheelSelection() {
	int count_a = 0, count_b = 0, count_c = 0, count_d = 0, count_e = 0;
	int temp = 0, i;
	char city_name[CITY_NUM] = { 'a','b','c','d','e' };
	float city_gailv[CITY_NUM] = { 0.14,0.23,0.06,0.4,0.17 };
	for (i = 0; i < 100000; i++) {
		temp = wheelSelection(city_gailv);
		//printf("选择的城市:%d\n", temp);
		switch (temp) {
		case 0:count_a++; break;
		case 1:count_b++; break;
		case 2:count_c++; break;
		case 3:count_d++; break;
		case 4:count_e++; break;
		}
	}
	printf("a概率:%f\n", count_a / float(count_a + count_b + count_c + count_d + count_e));
	printf("b概率:%f\n", count_b / float(count_a + count_b + count_c + count_d + count_e));
	printf("c概率:%f\n", count_c / float(count_a + count_b + count_c + count_d + count_e));
	printf("d概率:%f\n", count_d / float(count_a + count_b + count_c + count_d + count_e));
	printf("e概率:%f\n", count_e / float(count_a + count_b + count_c + count_d + count_e));
}

//贪婪算法第一次找路
float tanlan(graph G) {
	int i = 0, j, temp_index;
	float count = 0, temp_max;
	for (i = 0; i != CITY_NUM - 1;) {
		temp_max = 0;
		for (j = 0; j < CITY_NUM; j++) {
			if (i >= j)continue;
			if (G.edge[i][j] > temp_max) {
				temp_max = G.edge[i][j];
				temp_index = j;
			}
		}
		i = temp_index;
		count += temp_max;
	}
	return count;
}

//列举下一个可能到达的城市
int selectNextCity(graph G, int now) {
	int j, selected, i;
	float next_city_gailv[CITY_NUM] = { 0 };
	for (j = 0; j < CITY_NUM; j++) {
		next_city_gailv[j] = G.xinxisu[now][j] * EVER_FAVOR + (float)G.edge[now][j] * QIFA;	//下一访问城市概率
	}
	float all = 0;	//概率和
	for (j = 0; j < CITY_NUM; j++) {
		all += next_city_gailv[j];
	}
	//计算从当前城市到每一个可能到达的城市概率
	for (j = 0; j < CITY_NUM; j++) {
		next_city_gailv[j] = next_city_gailv[j] / all;
	}
	//轮盘赌选择
	//do {
		selected = wheelSelection(next_city_gailv);
	//} while (selected >= CITY_NUM);
	

	return selected;	//返回下一个城市下标号
}

//更新信息素
void renew(graph *G, float length[], int route[][CITY_NUM]) {
	int i, j, k, l;	//出发点、到达点、蚂蚁号、检查路径
	float weight;	//
	for (i = 0; i < CITY_NUM; i++) {
		for (j = 0; j < CITY_NUM; j++) {
			if (G->edge[i][j] == 0)continue;	//不计算不通的路径
			weight = 0.0;
			for (k = 0; k < NUM; k++) {
				for (l = 0; l < CITY_NUM; l++) {
					//判断当前蚂蚁的路径中是否经过目前检查的边
					if (route[k][l + 1] == -1)break;
					if (route[k][l] == i && route[k][l + 1] == j)
						//1/weigh
						weight += length[k];
				}
			}
			G->xinxisu[i][j] = G->xinxisu[i][j] * (1 - ZHENGFA) + weight;
		}
	}
}

//运行一次蚁群算法，可在此处修改要测试的图
void ant_system(){ 
	float C;
	int i, j;
	//信息素浓度(全局初始化)
	float t0;

	int city[CITY_NUM][CITY_NUM] = {
	{0,6,4,5,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0},
	{0,0,0,0,1,0,0,0,0},
	{0,0,0,0,0,2,0,0,0},
	{0,0,0,0,0,0,9,7,0},
	{0,0,0,0,0,0,0,4,0},
	{0,0,0,0,0,0,0,0,2},
	{0,0,0,0,0,0,0,0,4},
	{0,0,0,0,0,0,0,0,0}
	};

	//int qifa[CITY_NUM][CITY_NUM] = { 0 };

	//构造邻接矩阵(路径矩阵)
	graph G;
	createGraph(&G, city);
	//print_graph(G);
	C = tanlan(G);	//路径长度
	printf("贪婪路径长度：%f\n", C);
	//初始化信息素浓度
	t0 = FAVOR * NUM * (float)C;
	for (i = 0; i < CITY_NUM; i++) {
		for (j = 0; j < CITY_NUM; j++) {
			if (i >= j)continue;
			if (G.edge[i][j] > 0)G.xinxisu[i][j] = t0;	//只给存在的路径加信息素
		}
	}

	int now, next;	//当前城市下标、下一城市下标
	int route[NUM][CITY_NUM];	//记录每只蚂蚁可能到达的下一城市
	int turn=0;
	float length[NUM] = { 0.0 };	//一只蚂蚁完整路径的长度

	//总轮数
	while (turn < TOTAL_TURN) {
		//printf("turn:%d\n", turn);
		for (i = 0; i < NUM; i++)
			length[i] = 0;
		for (i = 0; i < NUM; i++) {
			
			now = 0; next = 0; 
			for (j = 0; j < CITY_NUM; j++) 				
				route[i][j] = -1;	//每次只蚂蚁搜索完整路径都从头开始
			
			j = 0;
			route[i][0] = 0;	//所有蚂蚁从0号城市出发
			while (now != CITY_NUM - 1) {	//当当前节点下标为最后点下标时，结束循环，得到完整路径
				next = selectNextCity(G, now);
				route[i][++j] = next;
				length[i] += G.edge[now][next];	//计算每只蚂蚁构建出的路径长度,第i只蚂蚁的路径
				now = next;
			}
		}
		//更新每条边上的信息素浓度
		renew(&G, length, route);
		turn++;
	}

	//输出最终稳定的路径
	for (i = 0; i < NUM; i++) {
		printf("\n=====length[%d]:%f=====\n", i, length[i]);
		printf("一轮中每一只蚂蚁构造的完整路径：");//若收敛成功则完成
		for (j = 0; j < CITY_NUM; j++) {
			if (route[i][j] == -1)
				break;
			else
				printf("->");
			printf("%d", route[i][j]);	
		}
	}
}

//返回一个0~(range-1)的整数
int randx(int range) {
	int number;
	number = rand() % range;
	return number;
}

float rand0_1() {
	float number;
	number = rand() / (RAND_MAX + 1.0);
	return number;
}

//返回一个保存了访问序列的数组
void selectNextCity2(struct headnode *city_list,int order[]) {
	int i, j, next = 0, round, tail = 0;
	node *temp = NULL;
	for (i = 0; i < CITY2; i++) {
		order[i] = city_list[next].headnum;	//第一点从0点开始
		printf("order:%d\n", order[i]);
		if (order[i] == CITY2 - 1)break;	//路径已到达图的末尾节点则退出
		//随机选择访问可达的下一个节点
		//round是一个下标，邻接表后每个节点计数下标从0开始
		round = randx(city_list[next].tail_length);
		//tail = city_list[next].tail_length;
		temp = city_list[next].next;
		for (j = 0; j < round + 1; j++) {
			next = temp->nodenum;
			temp = temp->next;
			if (temp == NULL)break;
		}
	}
}

//种群初始化
void init(int c[BREED][CITY2], struct headnode *city_list) {
	int i, j;
	int order[CITY2];
	
	for (i = 0; i < CITY2; i++) {
		order[i] = -1;
	}
	
	for (i = 0; i < BREED; i++) {
		printf("=====第%d号种群路径：=====\n", i);
		selectNextCity2(city_list, order);
		//根据访问序列进行0/1转换
		for (j = 0; j < CITY2; j++) {
			if (order[j] == -1)
				break;
			else
				c[i][order[j]] = 1;
		}
	}
}

//从一点到下一点的路经长度查询
int pathLength(int now, int next, struct headnode *city_list) {
	int length, i, tail_length;
	node *temp = city_list[now].next;
	
	while (temp)
	{
		length = temp->length;
		if (temp->nodenum == next)break;
		temp = temp->next;
	}

	return length;
}

//适应度评估函数
void evaluate(int c[BREED][CITY2], float value[BREED], struct headnode *city_list) {
	int i, j, now, next;
	for (i = 0; i < BREED; i++) {
		now = 0;
		for (j = 0; j < CITY2; j++) {
			if (c[i][j] == 1) {
				next = j;
				value[i] += pathLength(now, next, city_list);
				now = next;
			}
		}
	}
}

//轮盘赌算法2，只把CITY_NUM改成BREED了，看能不能想办法合并两个轮盘赌
int wheelSelection2(float city[]) {
	float number = 0.0, order;
	int i;
	do {
		number = rand() / (RAND_MAX + 1.0);
	} while (number == 0.0);

	order = 0.0;
	for (i = 0; i < BREED; i++) {
		if (number > order && number <= order + city[i])
			return i;	//返回选中种群下标号
		order += city[i];
	}
}

//运用轮盘赌对种群进行替换，有概率增加优秀种群的数量
void select(int c[BREED][CITY2], float value[BREED]) {
	int selected[BREED];	//按顺序选择替换方案
	int c_copy[BREED][CITY2];
	int i, j;
	float all = 0.0;

	for (i = 0; i < BREED; i++) {
		all += value[i];
	}

	for (i = 0; i < BREED; i++) {
		value[i] = value[i] / all;
	}

	for (i = 0; i < BREED; i++) {
		selected[i] = wheelSelection2(value);
	}

	for(i=0;i<BREED;i++)
		for (j = 0; j < CITY2; j++) {
			c_copy[i][j] = c[i][j];
		}

	for (i = 0; i < BREED; i++) {
		for (j = 0; j < CITY2; j++) {
			c[i][j] = c_copy[selected[i]][j];
		}
	}
}

//交换路径
void swap(int x[], int y[], int site) {
	int temp, i;
	for (i = site + 1; i < CITY2; i++) {
		temp = x[i];
		x[i] = y[i];
		y[i] = temp;
	}
}

//交配，设定交配概率，轮盘赌决定是否交配
//交配策略：当交配位的城市节点相同且后继大于1个，交换其后的整条路径
void jiaopei(int c[BREED][CITY2], struct headnode *city_list) {
	float jiaopeigailv[BREED];
	int i, j, count = 0;	//count记录交配个数
	int site = CITY2 - 5;	//交配位
	
	for (i = 0; i < BREED; i++) {
		jiaopeigailv[i] = rand0_1();
		if (jiaopeigailv[i] < JIAOPEI) {
			jiaopeigailv[i] = 1;
			count++;
		}else
			jiaopeigailv[i] = 0;
	}
	i = 0;
	while ((count % 2) != 0) {
		jiaopeigailv[i++] = 0;
		count--;
	}
	
	//可能存在没有一条路能交配的情形，但是不要紧
	for (i = 0; i < BREED; i++) {
		if (jiaopeigailv[i] == 1) {
			for (j = i+1; j < BREED; j++) {
				//没写后继大于一的条件
				if (c[i][site] == c[j][site] && city_list[site].tail_length > 1) {
					//交换交配位后整条路径
					swap(c[i], c[j], site);
					jiaopeigailv[i] = 0;
					count--;
					jiaopeigailv[j] = 0;
					count--;
				}
			}
		}	
	}
}

//变异，设定变异概率，只对分叉结点变异
//要判断点是否是分叉点，需要记录该点直接后继数量
//假定每个种群只变异一次
void bianyi(int c[BREED][CITY2], struct headnode *city_list) {
	float bianyigailv[BREED][CITY2];
	int i, j, k, l;
	node *temp = NULL;
	for (i = 0; i < BREED; i++) {
		for (j = 0; j < CITY2; j++)
			bianyigailv[i][j] = rand0_1();
	}

	for (i = 0; i < BREED; i++) {
		//j不从0开始，因为必须从0开始
		for (j = 1; j < CITY2; j++) {
			//若达到变异概率，经过该点，该点后继大于一个
			if (bianyigailv[i][j] < BIANYI && c[i][j] == 1 && city_list[j].tail_length > 1) {
				do {
					k = randx(CITY2);	//k是要变异的结点
				} while (city_list[k].tail_length > 1);
				do {
					l = randx(CITY2);	//变异后的结点
				} while (city_list[k].tail_length > 1);

				

				break;	//只变异一次
			}
		}
	}
}

void geneticAlgorithm() {
	int c[BREED][CITY2] = { 0 };	//种群
	float value[BREED] = { 0.0 };	//保存每个种群适应度
	srand((unsigned)time(NULL));

	//邻接表
	/*
0
1 6
2 4
3 5
-1 0
1 
4 1
-1 0
2
4 1
-1 0
3
5 2
-1 0
4
6 9
7 7
-1 0
5
7 4
-1 0
6
8 2
-1 0
7
8 4
-1 0
8
-1 0
	*/

	//初始化种群、图
	head city_list;
	createHeadNode(city_list);
	//print_linklist(city_list);
	init(c, city_list);

	//适应度评估
	evaluate(c, value, city_list);

	//循环
	int i, j;
	for (i = 0; i < TOTAL_TURN2; i++) {
		//选择
		select(c, value);

		//交配
		jiaopei(c, city_list);

		//变异
		bianyi(c, city_list);

		//评估
		evaluate(c, value, city_list);
	}

	for (i = 0; i < BREED; i++) {
		printf("\n第%d种群：", i);
		for (j = 0; j < CITY2; j++) {
			if (c[i][j] == 1)
				printf("%d ", j);
		}
	}	
}

int main() {
	int i;

	//初始化时间种子
	srand((unsigned)time(NULL));
	
	//测试轮盘赌
	//test_of_WheelSelection();

	/*for (i = 0; i < 10; i++) {
		printf("\n\n第%d组结果\n", i + 1);
		ant_system();
	}*/


	//构造邻接链表
	//head headlist;//head实际上是结构体数组的首地址
	//createHeadNode(headlist);
	//print_linklist(headlist);

	geneticAlgorithm();

	return 0;
}
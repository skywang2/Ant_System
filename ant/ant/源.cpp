/*
	蚁群算法解关键路径问题

	遗传算法解关键路径问题

*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NUM 20	//蚂蚁数量
#define CITY_NUM 9	//城市数量
#define TOTAL_TURN 160 //迭代总轮数
#define QIFA 0.5	//β，启发式信息作用
#define EVER_FAVOR 0.6	//α，信息素作用
#define FAVOR 0.7	//初始信息素浓度比率
#define ZHENGFA 0.5	//蒸发率

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
	node *next;
}node;

typedef struct headnode {
	int headnum;
	node *next;
}head[CITY_NUM];

node *createNode() {
	int n;
	node *node_now = NULL;
	node_now = (node *)malloc(sizeof(node));
	if (!node_now)return NULL;
	printf("请输入节点号");
	scanf("%d", &n);
	node_now->nodenum = n;
	node_now->next = NULL;
	//假设当节点值为-1则释放节点
	if (node_now->nodenum == -1)
		node_now = NULL;
	return node_now;
}

headnode *createHeadNode(head headlist) {
	int i, j, n;
	printf("创建头节点结构体数组\n");
	for (i = 0; i < CITY_NUM; i++) {
		printf("请输入头节点号:");
		scanf("%d", &n);
		headlist[i].headnum = n;
		//temp表示下一个节点的地址
		node **temp = &headlist[i].next;
		for (j = 0;; j++) {
			node *nextnode = createNode();
			//如果下一个节点存在则把nextnode链接上去
			//否则把next设为NULL
			if (nextnode){
				*temp = nextnode;
				temp = &nextnode->next;
			}
			else {
				*temp = NULL;
				break;
			}
		}
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

//种群初始化
void init() {

}

//适应度评估函数
void evaluate() {

}

//运用轮盘赌对种群进行替换，增加优秀种群的数量
void select() {

}

//交配，设定交配概率，轮盘赌决定是否交配
void jiaopei() {

}

//变异，设定变异概率，只对分叉结点变异
//要判断点是否是分叉点，需要记录该点直接后继数量
void bianyi() {

}

void genetic_algorithm() {

	//初始化

	//适应度评估

	//循环｛
	//选择

	//交配

	//变异

	//评估｝

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

	genetic_algorithm();

	return 0;
}
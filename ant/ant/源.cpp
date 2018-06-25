/*
	蚁群算法解关键路径问题

	遗传算法解关键路径问题

*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NUM 5	//蚂蚁数量
#define CITY_NUM 5	//城市数量
#define TOTAL_TURN 20 //迭代总轮数
#define FAVOR 1.3	//设定启发式信息

typedef struct graph {
	int edge[CITY_NUM][CITY_NUM];	//两点间路径长度
	//int numV;	//当前顶点数
	//int numE;	//当前边数
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
	float number, order;
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
	int j, selected;
	float next_city_gailv[CITY_NUM] = { 0 };
	for (j = 0; j < CITY_NUM; j++) {
		next_city_gailv[j] = G.xinxisu[now][j] * FAVOR;	//下一访问城市概率
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
	selected = wheelSelection(next_city_gailv);

	return selected;	//返回下一个城市下标号
}

void ant_system(){ 
	float C;
	int i, j;
	//信息素浓度(全局初始化)
	float t0;

	int city[CITY_NUM][CITY_NUM] = {
	{0,3,2,0,0},
	{0,0,0,5,0},
	{0,0,0,7,4},
	{0,0,0,0,6},
	{0,0,0,0,0}
	};

	//构造邻接矩阵(路径矩阵)
	graph G;
	createGraph(&G, city);
	//print_graph(G);
	C = tanlan(G);	//路径长度
	printf("贪婪路径长度：%f\n", C);
	//初始化信息素浓度(尚未需要维护每条边的信息素浓度)
	t0 = NUM / (float)C;
	for (i = 0; i < CITY_NUM; i++) {
		for (j = 0; j < CITY_NUM; j++) {
			if (i >= j)continue;
			if (G.edge[i][j] > 0)G.xinxisu[i][j] = t0;	//只给存在的路径加信息素
		}
	}

	int now, next;	//当前城市下标、下一城市下标
	int route[CITY_NUM];	//可能到达的下一城市
	int turn;

	//总轮数,一轮只有一只蚂蚁
	//for (turn = 0; turn < TOTAL_TURN; turn++) {

			now = 0; next = 0; 
			for (j = 0; j < NUM; j++)
				route[j] = -1;	//每次只蚂蚁搜索完整路径都从头开始
			j = 0;
			route[0] = 0;	//所有蚂蚁从0号城市出发
			while (now != CITY_NUM - 1) {	//当当前节点下标为最后点下标时，结束循环，得到完整路径
				next = selectNextCity(G, now);
				route[++j] = next;
				now = next;
			}
			
			//计算每只蚂蚁构建出的路径长度

			//更新每条边上的信息素浓度

	//}

	//输出最终稳定的路径
	printf("一只蚂蚁构造的完整路径：\n");
	for (i = 0; i < CITY_NUM; i++) {
		if (route[i] == -1)
			break;
		else
			printf("->");
		printf("%d", route[i]);			
	}

}

void genetic_algorithm() {

}

int main() {
	

	//初始化时间种子
	srand((unsigned)time(NULL));
	
	//测试轮盘赌
	//test_of_WheelSelection();
	
	ant_system();

	//构造邻接链表
	//head headlist;//head实际上是结构体数组的首地址
	//createHeadNode(headlist);
	//print_linklist(headlist);

	genetic_algorithm();

	return 0;
}
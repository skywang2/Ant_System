#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NUM 5	//蚂蚁数量
#define CITY_NUM 5	//城市数量

typedef struct graph {
	int edge[NUM][NUM];
	int numV;//当前顶点数
	int numE;//当前边数
}graph;

graph *createGraph(graph *G) {
	int v, e, i, j;
	printf("输入顶点数和边数\n");
	scanf_s("%d %d", &v, &e);
	G->numE = e; G->numV = v;
	for(i=0;i<NUM;i++)
		for (j = 0; j < NUM; j++) {
			if (i >= j) {
				G->edge[i][j] = 0;
				continue;
			}
			printf("输入（%d,%d）的值：", i, j);
			scanf_s("%d", &G->edge[i][j]);
		}
	return G;
}

void print_graph(graph G) {
	int i, j;
	printf("顶点数：%d，边数：%d", G.numV, G.numE);
	for (i = 0; i < NUM; i++) {
		printf("\n");
		for (j = 0; j < NUM; j++) {
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
}head[NUM];

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
	for (i = 0; i < NUM; i++) {
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
	for (i = 0; i < NUM; i++) {
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
int wheelSelection(float city[CITY_NUM]) {
	float number, order;
	int i;
	do {
		number = rand() / (float)(RAND_MAX);
	} while (number == 0.0);

	order = 0.0;
	for (i = 0; i < CITY_NUM; i++) {
		if (number > order && number <= order + city[i])
			return i;	//返回城市列表城市下标号
		order += city[i];
	}
}

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

int main() {	
	//初始化时间种子
	srand((unsigned)time(NULL));
	
	test_of_WheelSelection();

	//构造邻接矩阵
	//graph G;
	//createGraph(&G);
	//print_graph(G);

	//构造邻接链表
	//head headlist;//head实际上是结构体数组的首地址
	//createHeadNode(headlist);
	//print_linklist(headlist);



	getchar();
	getchar();
	return 0;
}
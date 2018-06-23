#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define NUM 5

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

int main() {
	//构造邻接矩阵
	//graph G;
	//createGraph(&G);
	//print_graph(G);

	//构造邻接链表
	head headlist;//head实际上是结构体数组的首地址
	createHeadNode(headlist);
	print_linklist(headlist);
	//printf("headlist[1].next.headnum=%d", headlist[1].next->nodenum);


	getchar();
	getchar();
	return 0;
}
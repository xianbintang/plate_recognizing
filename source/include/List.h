struct Node {
	CvRect item;
	Node * next;
};

typedef struct Node * List;

/*有头结点的链表*/

List create_list()
{
	List L;
	L = (struct Node *)malloc(sizeof(Node));

	if (L == NULL) {
		fprintf(stderr, "Error! out of space!\n");
	}

	return L;
}

void push_back(List L, CvRect i)
{
	struct Node * tmp = (struct Node *)malloc(sizeof(Node));
	if (tmp == NULL) {
		fprintf(stderr, "Error! out of space!\n");
	}
	tmp->item = i;
	
	L->next = tmp;
	tmp->next = NULL;
}
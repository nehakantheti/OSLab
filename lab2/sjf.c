#include<stdio.h>
#include<stdlib.h>

struct node{
    int pid;
    int at;
    int bt;
    int ct;
    int tat, wt, rt;
    struct node* left;
    struct node* right;
};

struct node* createNode(int pid, int at, int bt){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->pid = pid;
    temp->at = at;
    temp->bt = bt;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* heapify(struct node* root){
    if(root == NULL)
        return root;
    
    struct node* smallest = root;
    
    if(root->left && root->left->bt < smallest->bt){
        smallest = root->left;
    }
    
    if(root->right && root->right->bt < smallest->bt){
        smallest = root->right;
    }
    
    if(smallest != root){
        struct node* tmp = root;
        root = smallest;
        smallest = tmp;
        
        heapify(smallest);
    }
}

struct node* insert(struct node* root, struct node* temp){
    if(root == NULL){
        root = root;
        return root;
    }else{
        struct node* temp = root;
        struct node* parent = NULL;
        struct node* current = NULL;
        
        struct node** queue = (struct node**)malloc(10*sizeof(struct node*));
        int front = 0, rear = 0;
        
        queue[rear++] = temp;
        
        while(front < rear){
            current = queue[front++];
            if(current->left == NULL){
                current->left = temp;
                break;
            }else{
                queue[rear++] = current->left;
            }
            
            if(current->right == NULL){
                current->right = temp;
                break;
            }else{
                queue[rear++] = current->right;
            }
        }
        free(queue);
        heapify(root);
    }
}

struct node* extractMin(struct node* root){
    if(root == NULL)
        return NULL;
    
    struct node* minNode = root;
    struct node* parent = NULL;
    struct node* temp = NULL;
    struct node* lastNode = root;
    
    struct node** queue = (struct node**)malloc(10*sizeof(struct node*));
        int front = 0, rear = 0;
        
        queue[rear++] = root;
        
        while(front < rear){
            temp = queue[front++];
            if(temp->left){
                queue[rear++] = temp;
                parent = temp;
            }
            
            if(temp->right){
                queue[rear++] = temp;
                parent = temp;
            }
        }
        free(queue);
        
        if(parent!=NULL){
            if(parent->left){
                root = parent->left;
                parent->left = NULL;
            }
            
            if(parent->right){
                root = parent->right;
                parent->right = NULL;
            }
            
            root->pid = minNode->pid;
            root->at = minNode->at;
            root->bt = minNode->bt;
            heapify(root);
        }else{
            root = NULL;
        }
    return minNode;
}

void printHeap(struct node* root){
    if(root == NULL)
        return ;
    
    printHeap(root->left);
    printf("%d ", root->pid);
    printHeap(root->right);
}

int main(){
    int num;
    printf("Enter number of processes : ");
    scanf("%d", &num);
    
    struct node* root = NULL;
    
    for(int i = 0;i < num;i++){
        int pid, at, bt;
        printf("Enter PID : ");
        scanf("%d", &pid);
        printf("Enter AT : ");
        scanf("%d", &at);
        printf("Enter BT : ");
        scanf("%d", &bt);
        
        struct node* newNode = createNode(pid, at, bt);
        root = insert(root, newNode);
        
    }
    if(root == NULL){
        printf("Root is null");
    }
    printHeap(root);
    return 0;
}
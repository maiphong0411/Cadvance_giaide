// in ra danh sach products
// hien thi muc do lien quan input 2 id
// danh sach cac san pham lien quan 1 id outdegree
// moi lien he giua 2 sp input 2 id shortest
// menu
#include "graph.h"
void menu()
{
    printf("===========Menu=========\n");
    printf("1.List of products\n2.List of order\n3.Weighted of 2 products\n4.List of relation of \n5.Relation of 2 products\n6.Exit....Bye bye\n ");
    printf("Please press 1 and 2 to read data before\n");
    printf("Your choice: ");
}
// ok
void InputinProducts(char *filename,Graph G)
{
    FILE * f = fopen(filename,"r");
    if(f == NULL)   printf("ErrorFile\n");
    int id_num;
    char id_str[30],name[30];
    while(!feof(f))
    {
        fscanf(f,"%s %s",id_str,name);
        id_num = atoi(id_str);
        addVertex(G,id_num,name);
    }
    fclose(f);
}
// ok
void InputinOrderhistory(char *filename,Graph G)
{
    FILE * f = fopen(filename,"r");
    if(f == NULL)   printf("ErrorFile");
    char buf[30];
    char str[10];
    int input[10];
    char empty_str[10] = "";
    JRB node1,node2,tree;
    while(!feof(f))
    {
        fgets(buf,255,f);
        int j = 0;
        for(int i = 0;i < strlen(buf);i += 2)
        {
            if(buf[i] == '\n') break;
            str[0] = buf[i];
            str[1] = '\0';
            input[j++] = atoi(str);
            strcpy(str,empty_str);
        }
        
        for(int i = 0;i < j;i++)
        {
            node1 = jrb_find_int(G.edges,input[i]);
            for(int k = i + 1;k < j; k++)
            {
                if(node1 == NULL)   addEdge(G,input[i],input[k],1);
                else
                {
                tree = (JRB)jval_v(node1->val);
                node2 = jrb_find_int(tree,input[k]);
                    if(node2 == NULL)   addEdge(G,input[i],input[k],1);
                    else
                    {
                            double x = (double)jval_d(node2->val);
                            x = x + 1;
                            node2->val = new_jval_d(x) ;
                    }
                }
            }
        }
        for(int i = 0;i < j;i++)
        {
            printf("%s ",getVertex(G,input[i]));
        }
        printf("\n");
    }
    fclose(f);
}

// ok
void PrintProducts(Graph G)
{
    JRB node;
    jrb_traverse(node,G.vertices)
    {
        printf("Ma san pham : %d\n",node->key);
        printf("Ten san pham : %s\n\n",node->val);
    }
}

void main(int argv,char *argc[])
{
    Graph G = createGraph();
    InputinProducts("products.txt",G);
    InputinOrderhistory("orderhistory.txt",G);
    int scan;
    int length;
    int v1, v2,id,len,inlen;
    int output[1000];
    int inoout[1000];
    int path[1000];
    do
    {
        /* code */
        
        menu();
        scanf("%d",&scan);
        switch (scan)
        {
        case 1:
            PrintProducts(G);
            break;
        case 2:
            InputinOrderhistory("orderhistory.txt",G);
            break;
        case 3:
            printf("Nhap v1: ");
            scanf("%d",&v1);
            printf("Nhap v2: ");
            scanf("%d",&v2);
            if(v1 > v2) swap(&v1,&v2);
            printf("Weight of %d and %d is %f\n",v1,v2,getEdgeValue(G,v1,v2));
            break;
        case 4:
            printf("id of product: ");
            scanf("%d",&id);
            inlen = indegree(G,id,inoout);
            len = outdegree(G,id,output);
            for(int i =0;i < len;i++)
            {
                printf("%s ",getVertex(G,output[i]));
            }
            for(int j=0;j < inlen;j++)
            {
                printf("%s ",getVertex(G,inoout[j]));
            }
            printf("\n");
            break;
        case 5:
            printf("Nhap v1: ");
            scanf("%d",&v1);
            printf("Nhap v2: ");
            scanf("%d",&v2);
            
            if(v1 > v2)
            {
                swap(&v1,&v2);
            }

            int weight = shortestPath(G,v1,v2,path,&length);
            if(weight == INFINITIVE_VALUE) printf("no path\n");
            else
                printf("path from %s to %s (with total)\n",getVertex(G,v1),getVertex(G,v2));
                for(int i = 0; i < length; i++)
                {
                    printf(" %s => ",getVertex(G,path[i]));
                }
                printf("%d\n",weight);
            break;
        case 6:
            dropGraph(G);
            break;
        default:
            printf("moi ban chon lai::::\n");
            break;
        }
    } while (scan != 6);
    
}
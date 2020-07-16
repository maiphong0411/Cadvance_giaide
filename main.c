#include <stdio.h>
#include "graph.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STR_LENGTH 1000
void func1(Graph G)
{
    FILE* f = fopen("webpages.txt", "r");
    int n, id1;
    char web[MAX_STR_LENGTH];
    fscanf(f, "%d\n", &n);
    printf("%d\n", n);
    for (int i = 0; i < n; ++i)
    {
        fscanf(f, "%s %d\n", web, &id1);
        printf("%s %d\n", web, id1);
        addVertex(G, id1, web);
    }
    fclose(f);

    char str[MAX_STR_LENGTH], *tmp;
    int id2;
    f = fopen("pageConnections.txt", "r");
    fscanf(f, "%d\n", &n);
    for (int i = 0; i < n; ++i)
    {
        fgets(str, MAX_STR_LENGTH, f);
        id1 = atoi(strtok(str, " "));
        while (1)
        {
            tmp = strtok(NULL, " ");
            if (tmp == NULL)
                break;
            id2 = atoi(tmp);
            addEdge(G, id1, id2, 1);
        }
    }
    fclose(f);

    JRB iter;
    int output[MAX_PATH_LENGTH], max = 0, min = INFINITIVE_VALUE;
    jrb_traverse(iter, G.vertices)
    {
        n = indegree(G, jval_i(iter->key), output);
        if (n > max)
            max = n;
        if (n < min)
            min = n;
    }
    jrb_traverse(iter, G.vertices)
    {
        n = indegree(G, jval_i(iter->key), output);
        if (n == max)
            printf("MAX --- %d: %d\n", jval_i(iter->key), max);
        if (n == min)
            printf("MIN --- %d: %d\n", jval_i(iter->key), min);
    }
}

double PageRank(Graph G, double* weight, int id)
{
    double s = 0;
    int n, output[MAX_PATH_LENGTH], output2[MAX_PATH_LENGTH];
    n = indegree(G, id, output);
    for (int i = 0; i < n; ++i)
        s = s + weight[output[i]]/outdegree(G, output[i], output2);
    // double damping = 0.85;
    // return (1-damping) + damping*s;
    return s;
}

void func2(Graph G)
{
    double weight[MAX_ID];
    for (int i = 0; i < MAX_ID; ++i)
        weight[i] = 1.0;
    double max = 0, min = INFINITIVE_VALUE;
    int max_id, min_id;
    JRB iter, tree, node;
    jrb_traverse(iter, G.vertices)
    {
        if (PageRank(G, weight, jval_i(iter->key)) > max)
        {
            max = PageRank(G, weight, jval_i(iter->key));
            max_id = jval_i(iter->key);
        }
        
        if (PageRank(G, weight, jval_i(iter->key)) < min)
        {
            min = PageRank(G, weight, jval_i(iter->key));
            min_id = jval_i(iter->key);
        }
    }

    printf("Web co trong so cao nhat: %s - %lf\n", getVertex(G, max_id), max);
    printf("Web co trong so thap nhat: %s - %lf\n", getVertex(G, min_id), min);
}

void func3(Graph G)
{
    // Mang luu page rank
    double weight[MAX_ID];
    for (int i = 0; i < MAX_ID; ++i)
        weight[i] = 1.0;
    // Mang luu page rank tam thoi truoc khi cap nhat
    double tmp[MAX_ID];
    for (int i = 0; i < MAX_ID; ++i)
        tmp[i] = weight[i];

    int m;
    JRB iter, tree, node;
    printf("Nhap so lan duyet: "); scanf("%d", &m);
    while (m > 0)
    {
        m--;
        // Tim page rank moi
        jrb_traverse(iter, G.vertices)
            tmp[jval_i(iter->key)] = PageRank(G, weight, jval_i(iter->key));
        // Cap nhat page rank
        for (int i = 0; i < MAX_ID; ++i)
            weight[i] = tmp[i];
    }

    double A[MAX_PATH_LENGTH]; // weight
    int B[MAX_PATH_LENGTH]; // ID
    int n = 0;
    jrb_traverse(iter, G.vertices)
    {
        A[n] = weight[jval_i(iter->key)];
        B[n] = jval_i(iter->key);
        n++;
    }

    for (int i = 0; i < n-1; ++i)
        for (int j = i+1; j < n; ++j)
            if (A[i] < A[j])
            {
                double t1 = A[i];A[i]=A[j];A[j]=t1;
                int t2 = B[i];B[i]=B[j];B[j]=t2;
            }
    printf("Top 3 trang web gia tri nhat la: \n");
    for (int i = 0; i < 3; ++i)
        printf("%s - %lf\n", getVertex(G, B[i]), A[i]);
}

void func4(Graph G)
{
    JRB iter;
    int output[MAX_PATH_LENGTH];
    int in = 0, out = 0;
    jrb_traverse(iter, G.vertices)
    {
        if ((indegree(G, jval_i(iter->key), output) > 0) && (outdegree(G, jval_i(iter->key), output) == 0))
            in++;
        if ((indegree(G, jval_i(iter->key), output) == 0) && (outdegree(G, jval_i(iter->key), output) > 0))
            out++;
    }
    
    printf("So webpage chi co lien ket den: %d\n", in);
    printf("So spam bot: %d\n", out);
}

void func5(Graph G)
{
    int id1, id2;

    printf("Nhap ID1 ID2: "); scanf("%d %d", &id1, &id2);
    if (!jrb_find_int(G.vertices, id1))
    {
        printf("ID-%d khong hop le\n", id1);
        return;
    }
    if (!jrb_find_int(G.vertices, id2))
    {
        printf("ID-%d khong hop le\n", id2);
        return;
    }
    if (id1 == id2)
    {
        printf("-1\n");
        return;
    }

    int length, path[MAX_PATH_LENGTH];
    double distance;
    distance = shortestPath(G, id1, id2, path, &length);
    if (distance == INFINITIVE_VALUE)
        printf("Khong ton tai duong di tu webpage co ID-%d toi webpage co ID-%d\n", id1, id2);
    else
        printf("Khoang cach nho nhat tu webpage co ID-%d toi webpage co ID-%d: %.0lf\n", id1, id2, distance);
}

int main()
{
    // Menu
    Graph G = createGraph();
    int select;
    do
    {
        puts("");
        puts("MENU: ");
        puts("1. Doc file");
        puts("2. PageRank 1 lan");
        puts("3. PageRank m lan");
        puts("4. ?");
        puts("5. Khoang cach nho nhat");
        puts("6. Thoat");
        printf(">>> "); scanf("%d", &select);
        switch (select)
        {
        case 1:
            func1(G);
            break;
        case 2:
            func2(G);
            break;
        case 3:
            func3(G);
            break;
        case 4:
            func4(G);
            break;
        case 5:
            func5(G);
            break;
        default:
            break;
        }
    } while (select >= 1 && select <= 5);

    dropGraph(G);
    return 0;
}
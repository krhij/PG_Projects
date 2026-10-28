#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#pragma warning(disable:4996) //to make scanf work

#define INFINITY 2147483647 //i assume that int limit is enough for infinity

struct Point
{
    int column;
    int row;
};

struct Lift
{
    Point start;
    Point end;
    int travel_time;
    int interval; //when the lift leaves
};

struct liftNode 
{
    int lift_index;
    liftNode* next;
};


struct HeapNode
{
    int dist; //distance from the source
    Point pos;
};

struct MinHeap 
{
    int current_size; //nodes present currently
    int capacity; //max possible size
    int** pos_in_heap; //index of a point
    HeapNode* nodes; //pointer to array of heapnodes
};

void swap_nodes(int idx1, int idx2, MinHeap* heap) 
{
    HeapNode temp = heap->nodes[idx1];
    heap->nodes[idx1] = heap->nodes[idx2];
    heap->nodes[idx2] = temp;

    heap->pos_in_heap[heap->nodes[idx1].pos.column][heap->nodes[idx1].pos.row] = idx1;
    heap->pos_in_heap[heap->nodes[idx2].pos.column][heap->nodes[idx2].pos.row] = idx2;
}

//to keep the heap properties - each parent node is less than or equal to its child nodes
void sift_down(int idx, MinHeap* heap)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    //find the smallest from root, left child, and right child
    if (left < heap->current_size && heap->nodes[left].dist < heap->nodes[smallest].dist)
    {
        smallest = left;
    }
        
    if (right < heap->current_size && heap->nodes[right].dist < heap->nodes[smallest].dist)
    {
        smallest = right;
    }

    //if the smallest is not the root, swap and heapify the affected subtree
    if (smallest != idx) {
        swap_nodes(idx, smallest, heap);
        sift_down(smallest, heap);
    }
}

//get minimum distance node from the heap
HeapNode extract_min(MinHeap* heap)
{

    if (heap->current_size == 1)
    {
        heap->current_size--;
        //mark as removed from heap
        heap->pos_in_heap[heap->nodes[0].pos.column][heap->nodes[0].pos.row] = -1;
        return heap->nodes[0];
    }

    //store the root node (minimum)
    HeapNode root = heap->nodes[0];

    //replace root with the last node
    heap->nodes[0] = heap->nodes[heap->current_size - 1];
    heap->pos_in_heap[heap->nodes[0].pos.column][heap->nodes[0].pos.row] = 0; //update position of moved node

    //mark original root as removed from heap
    heap->pos_in_heap[root.pos.column][root.pos.row] = -1;

    //reduce heap size and heapify the root
    heap->current_size--;
    sift_down(0, heap);

    return root;
}

void sift_up(int idx, MinHeap* heap) 
{
    int parent = (idx - 1) / 2;
    while (idx > 0 && heap->nodes[idx].dist < heap->nodes[parent].dist) 
    {
        swap_nodes(idx, parent, heap);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

bool is_in_heap(Point p, MinHeap* heap) 
{
    //check if the point's position index is valid (not -1) and within heap bounds
    int pos = heap->pos_in_heap[p.column][p.row];
    return pos != -1 && pos < heap->current_size;
}

//decrease distance of a given point p
void decrease_key(Point p, int new_dist, MinHeap* heap)
{
    int index = heap->pos_in_heap[p.column][p.row];

    heap->nodes[index].dist = new_dist;

    //travel up the tree to maintain heap property
    //this is needed because decreasing the key might violate the heap property with its parent
    sift_up(index, heap);
}

void insert(HeapNode node, MinHeap* heap) 
{
    //insert new node at the end
    int i = heap->current_size;
    heap->nodes[i] = node;
    heap->pos_in_heap[node.pos.column][node.pos.row] = i; //store index
    heap->current_size++;

    //fix the min heap property if it is violated
    sift_up(i, heap);
}

MinHeap* create_min_heap(int width, int height)
{
    MinHeap* min_heap = (MinHeap*)malloc(sizeof(MinHeap));
    min_heap->capacity = width * height;
    min_heap->current_size = 0;
    min_heap->nodes = (HeapNode*)malloc(min_heap->capacity * sizeof(HeapNode));

    //allocate and initialize position map
    min_heap->pos_in_heap = (int**)malloc(width * sizeof(int*));
    for (int c = 0; c < width; ++c)
    {
        min_heap->pos_in_heap[c] = (int*)malloc(height * sizeof(int));
        for (int r = 0; r < height; ++r) 
        {
            min_heap->pos_in_heap[c][r] = -1; //-1 means not in heap
        }
    }
    return min_heap;
}

void destroy_min_heap(MinHeap* heap, int width) 
{
    //free the inner arrays of pos_in_heap
    if (heap->pos_in_heap)
    {
        for (int c = 0; c < width; c++) 
        {
            free(heap->pos_in_heap[c]);
        }
        //free the outer array of pos_in_heap
        free(heap->pos_in_heap);
    }

    //free the nodes array
    if (heap->nodes) 
    {
        free(heap->nodes);
    }

    //free the heap structure
    free(heap);
}

int calculate_walk_cost(int from_height, int to_height) 
{
    if (to_height > from_height)
    {
        return to_height - from_height + 1;
    }
    else
    {
        return 1;
    }
}

int calculate_lift_arrival(int current_time, int travel_time, int interval)
{
    int wait_time = 0;
    if (interval > 0)
    {
        int remainder = current_time % interval;
        if (remainder != 0) {
            wait_time = interval - remainder;
        }
    }
    return current_time + wait_time + travel_time;
}

int find_shortest_path(int width, int height,
    Point start_pos, Point end_pos,
    int** map, Lift* all_lifts, int num_lifts,
    int** dist, liftNode*** lift_starts)
{
    
    //create minheap and initialize
    MinHeap* min_heap = create_min_heap(width, height);

    //distance for start node = 0
    dist[start_pos.column][start_pos.row] = 0;
    HeapNode start_node = { 0, start_pos };
    insert(start_node, min_heap);

    //main dijkstras loop
    while (min_heap->current_size > 0) 
    {

        //extract node with minimum distance
        HeapNode current_heap_node = extract_min(min_heap);
        Point current_pos = current_heap_node.pos;
        int current_time = current_heap_node.dist; //the shortest time found so far

        //if already found a shorter path
        if (current_time > dist[current_pos.column][current_pos.row]) 
        {
            continue;
        }
        //check if destination reached
        if (current_pos.column == end_pos.column && current_pos.row == end_pos.row) 
        {
            break; //found the shortest path to the destination
        }

        //relax the neighbours
        int current_c = current_pos.column;
        int current_r = current_pos.row;

        //walking Neighbors
        int dr[] = { -1, 1, 0, 0 }; //Up, Down, Left, Right (row changes)
        int dc[] = { 0, 0, -1, 1 }; //Up, Down, Left, Right (column changes)

        for (int i = 0; i < 4; ++i)
        {
            int neighbor_c = current_c + dc[i];
            int neighbor_r = current_r + dr[i];

            if (neighbor_c >= 0 && neighbor_c < width && neighbor_r >= 0 && neighbor_r < height) 
            {
                //calculate cost to walk to neighbor
                int cost = calculate_walk_cost(map[current_c][current_r], map[neighbor_c][neighbor_r]);

                int time_via_current = current_time + cost;

                //relaxing check: is the path through current_pos shorter?
                if (time_via_current < dist[neighbor_c][neighbor_r])
                {
                    //update the distance
                    dist[neighbor_c][neighbor_r] = time_via_current;

                    //update the node in the heap or insert if not present
                    Point neighbor_pos = { neighbor_c, neighbor_r };
                    if (is_in_heap(neighbor_pos, min_heap))
                    {
                        decrease_key(neighbor_pos, time_via_current, min_heap);
                    }
                    else
                    {
                        HeapNode neighbor_node = { time_via_current, neighbor_pos };
                        insert(neighbor_node, min_heap);
                    }
                }
            }
            
        }

        //lift Neighbors
        liftNode* lift_node = lift_starts[current_c][current_r];
        while (lift_node != nullptr)
        {
            const Lift* lift = &all_lifts[lift_node->lift_index];
            int dest_c = lift->end.column;
            int dest_r = lift->end.row;

            if (dest_c >= 0 && dest_c < width && dest_r >= 0 && dest_r < height)
            {
                //calculate arrival time using the lift
                int arrival_time = calculate_lift_arrival(current_time, lift->travel_time, lift->interval);

                //relaxing check
                if (arrival_time < dist[dest_c][dest_r])
                {
                    dist[dest_c][dest_r] = arrival_time;

                    Point dest_pos = { dest_c, dest_r };
                    if (is_in_heap(dest_pos, min_heap))
                    {
                        decrease_key(dest_pos, arrival_time, min_heap);
                    }
                    else 
                    {
                        //insert the destination node if it wasnt in the heap
                        HeapNode dest_node = { arrival_time, dest_pos };
                        insert(dest_node, min_heap);
                    }
                }
                lift_node = lift_node->next;
            }
            
        }
    }

    //final distance
    int result_time = dist[end_pos.column][end_pos.row];

    destroy_min_heap(min_heap, width);

    return result_time;
}

int main()
{

    int width, height;
    scanf("%d %d", &width, &height);

    Point start_pos;
    scanf("%d %d", &start_pos.column, &start_pos.row);

    Point end_pos;
    scanf("%d %d", &end_pos.column, &end_pos.row);

    int** map = (int**)malloc(width * sizeof(int*));

    for (int i = 0; i < width; i++)
    {
        map[i] = (int*)malloc(height * sizeof(int));
    }

    int num_lifts;
    scanf("%d", &num_lifts);

    Lift* all_lifts = (Lift*)malloc(num_lifts * sizeof(Lift));

    for (int i = 0; i < num_lifts; ++i) 
    {
        scanf("%d %d %d %d %d %d", &all_lifts[i].start.column, &all_lifts[i].start.row, &all_lifts[i].end.column, 
            &all_lifts[i].end.row, &all_lifts[i].travel_time, &all_lifts[i].interval);
    }

    for (int r = 0; r < height; ++r) 
    {
        for (int c = 0; c < width; ++c) 
        {
            scanf("%d", &map[c][r]);
        }
    }

    int** dist = (int**)malloc(width * sizeof(int*));
    for (int c = 0; c < width; c++)
    {
        dist[c] = (int*)malloc(height * sizeof(int));
        //initialize all dists to infinity
        for (int r = 0; r < height; r++) 
        {
            dist[c][r] = INFINITY;
        }
    }
    dist[start_pos.column][start_pos.row] = 0;

    liftNode*** lift_starts = (liftNode***)malloc(width * sizeof(liftNode**));
    for (int c = 0; c < width; ++c)
    {
        lift_starts[c] = (liftNode**)malloc(height * sizeof(liftNode*));
        //initialize all heads to nullptr
        for (int r = 0; r < height; ++r)
        {
            lift_starts[c][r] = nullptr;
        }
    }

    for (int i = 0; i < num_lifts; ++i)
    {
        int start_c = all_lifts[i].start.column;
        int start_r = all_lifts[i].start.row;

        //new node for a linked list
        liftNode* new_node = (liftNode*)malloc(sizeof(liftNode));
        new_node->lift_index = i;

        new_node->next = lift_starts[start_c][start_r];
        lift_starts[start_c][start_r] = new_node;
    }

    int final_time = find_shortest_path(width, height, start_pos, end_pos,
        map, all_lifts, num_lifts,
        dist, lift_starts);

    printf("%d", final_time);
}
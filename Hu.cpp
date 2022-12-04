#include <iostream>
#include <vector>
#include <queue>

#define M 3 //number of machines
#define N 12 //number of tasks

struct Node{

    int key;
    int level;
    std::vector<Node*> child;
    bool free = false;
    bool isDone = false;
    bool isInQueue = false;

};

// Utility function to create a new tree node
Node* newNode(int key){

    Node* temp = new Node;
    temp->key = key;
    return temp;

}

// Prints the n-ary tree level wise
void LevelOrderTraversal(Node* root){

    if(root == NULL){

        return;

    }


    // Standard level order traversal code using queue

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()){

        int n = q.size();

        // If this node has children
        while(n > 0){

            // Dequeue an item from queue and print it

            Node* p = q.front();
            q.pop();
            std::cout << p->key << " lvl:" << p->level << " "; //" free:" << p->free << " ";

            // Enqueue all children of the dequeued item

            for(int i = 0; i < p->child.size(); i++){

                q.push(p->child[i]);

            }

            n--;
        }

        std::cout << std::endl; // New line between two levels

    }

}

void Leveling(Node* root){

    //root->level = 1;

    int lvl = 1;

    std::queue<Node*> q;
    q.push(root);

    while(!q.empty()){

        int children = q.size();

        // If this node has children
        while(children > 0){

            // Dequeue an item from queue and print it

            Node* p = q.front();
            q.pop();

            p->level = lvl;

            // Verifying if node is free to push on the working machine
            if(p->child.size() == 0){

                p->free = true;

            }

            // Enqueue all children of the dequeued item

            for(int i = 0; i < p->child.size(); i++){

                q.push(p->child[i]);

            }

            children--;
        }

        lvl++;

    }

}

bool checkIfChildrenAreFree(Node* father){

    int free = true;

    for(int i = 0; i < father->child.size(); i++){

        if(father->child.at(i)->isDone != true){

            free = false;
            break;

        }

    }

    return free;

}

std::vector<Node*> FindFreeTasks(Node* root){

    std::vector<Node*> list;

    std::queue<Node*> q;
    q.push(root);

    while(!q.empty()){

        int children = q.size();

        // If this node has children
        while(children > 0){

            // Dequeue an item from queue and print it

            Node* p = q.front();
            q.pop();

            if(!p->child.empty()){

                if(checkIfChildrenAreFree(p)){

                    p->free = true;

                }

            }

            
            if(p->free == true && p->isDone == false){

                if(p->isInQueue == false){

                    list.push_back(p);
                    p->isInQueue = true;

                }

            }

            // Enqueue all children of the dequeued item

            for(int i = 0; i < p->child.size(); i++){

                q.push(p->child[i]);

            }

            children--;
        }

    }

    return list;
}

void deleteTask(Node* root, int key){

    std::queue<Node*> q;
    q.push(root);

    while(!q.empty()){

        int n = q.size();

        // If this node has children
        while(n > 0){

            // Dequeue an item from queue and print it

            Node* p = q.front();
            q.pop();

            if(p->key == key){

                p->isDone = true;
                return;

            }

            // Enqueue all children of the dequeued item

            for(int i = 0; i < p->child.size(); i++){

                q.push(p->child[i]);

            }

            n--;
        }

    }

}

void Hu(Node* root){

    int moment = 0;
    int finishedTasks = 0;
    std::vector<std::vector<int>> machines;
    std::vector<Node*> todo;
    std::vector<Node*> tasks;
    
    // prepare machines for tasks
    for(int i = 0; i< M; i++){

        std::vector<int> m;
        machines.push_back(m);

    }

    while(finishedTasks < N){

        // find and sort tasks to do
        tasks.clear();
        tasks = FindFreeTasks(root);

        todo.insert(todo.end(), tasks.begin(), tasks.end());

        std::sort(todo.begin(), todo.end(),

            [](Node* n1, Node* n2){

                return n1->level > n2->level;

            }

        );

        std::cout <<  "Tasks to do:\n";

        for(Node* n : todo){

            std::cout << n->key << " ";

        }

        std::cout <<  "\nTasks done:\n";

        for(int i = 0; i< M; i++){

            if(!todo.empty()){

                machines.at(i).push_back(todo.front()->key);
                deleteTask(root, todo.front()->key);
                std::cout << todo.front()->key << " ";
                //std::cout << "\nbegin: " << todo.front()->key;
                todo.erase(todo.begin());
                //std::cout << "\nnew begin: " << todo.front()->key;
                finishedTasks++;

            }

        }

        std::cout << "\n\n";

        moment++;

        //LevelOrderTraversal(root);
        //std::cout << "\n";

    }

    std::cout << "\n\n-----------------------------------MACHINES-----------------------------------\n\n   ";

    for(int i = 1; i <= moment; i++){

        std::cout << i << "  ";

    }

    std::cout << "\n";

    for(int i = 0; i < M; i++){

        std::cout << "M" << i+1 << " ";

        for(int j = 0; j < machines.at(i).size(); j++){


            if(machines.at(i).at(j) > 9){

                std::cout << machines.at(i).at(j) << " ";

            }
            else{

                std::cout << machines.at(i).at(j) << "  ";

            }


        }

        std::cout << "\n";

    }

}

int main(){

    /*
    *            12
    *          /    \
    *        10       11
    *       / \      /  \
    *     6   7     8    9
    *    / \  /\    |   / \
    *         1 2   3   4  5
    * 
    * 
    *   albo
    * 
    *         
    *       12        13
    *      /  \      /  \
    *     11   10    9   8
    * 
    * 
    * 
    */

    int katastrofa = 0;

    std::cout << "Tree(0) or las(1)? ";
    std::cin >> katastrofa;
    std::cout << "\n";

    if(katastrofa == 0){

        Node* root = newNode(12);

        (root->child).push_back(newNode(10));
        (root->child).push_back(newNode(11));

        (root->child[0]->child).push_back(newNode(6));
        (root->child[0]->child).push_back(newNode(7));
        (root->child[1]->child).push_back(newNode(8));
        (root->child[1]->child).push_back(newNode(9));
        (root->child[0]->child[1]->child).push_back(newNode(1));
        (root->child[0]->child[1]->child).push_back(newNode(2));
        (root->child[1]->child[0]->child).push_back(newNode(3));
        (root->child[1]->child[1]->child).push_back(newNode(4));
        (root->child[1]->child[1]->child).push_back(newNode(5));

        Leveling(root);

        LevelOrderTraversal(root);

        std::cout << "\n";

        Hu(root);

    }else if(katastrofa > 0){

        Node* fake = newNode(-1);

        fake->isDone = true;

        Node* root1 = newNode(12);
        Node* root2 = newNode(13);

        (fake->child).push_back(root1);
        (fake->child).push_back(root2);

        (root1->child).push_back(newNode(11));
        (root1->child).push_back(newNode(10));

        (root2->child).push_back(newNode(9));
        (root2->child).push_back(newNode(8));

        Leveling(fake);

        LevelOrderTraversal(fake);

        std::cout << "\n";

        Hu(fake);

    }

    std::cout << "Level order traversal Before Mirroring\n";
    //LevelOrderTraversal(root);

    return 0;
}

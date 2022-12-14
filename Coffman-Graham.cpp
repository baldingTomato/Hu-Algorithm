#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

struct Node{

    bool isTaken = false;
    int index;
    std::string name;
    std::vector<std::string> children;
    std::vector<std::string> parents;
    std::vector<int> s_list;

};

Node CreateNode(std::string key, std::vector<std::string> children){

    Node newNode;

    newNode.name = key;
    
    if(!children.empty()) {

        for(std::string c : children){

            newNode.children.push_back(c);

        }

    }

    return newNode;

}

void FindParents(std::vector<Node> &root){

    std::vector<std::string> parents;

    for(Node n : root){

        parents.clear();

        for(int i = 0; i < root.size(); i++){

            for(int j = 0; j < root.at(i).children.size(); j++){

                if(root.at(i).children.at(j) == n.name){

                    parents.push_back(root.at(i).name);

                }

            }

        }

        for(int k = 0; k < root.size(); k++){

            if(root.at(k).name == n.name){

                root.at(k).parents = parents;

            }

        }

    }

}

void ReadNetwork(std::vector<Node> &root){

    std::ifstream input_file("graph.txt");
    std::string line;
    std::string label;
    std::string child;
    std::vector<std::string> children;

    if(!input_file.is_open()){

        std::cout << "Could not open the file \"graph.txt\"" << std::endl;
        std::exit(1);

    }

    while(std::getline(input_file, line)){

        std::stringstream lineStream(line);
        children.clear();

        lineStream >> label;

        // read precedents if you can and after reading each one, add it to the matching element of main vector as consequent
        while(lineStream >> child){

            children.push_back(child);

        }

        root.push_back(CreateNode(label, children));
        //root.back().parents = parents;

    }

    FindParents(root);

}

void PrintNetwork(std::vector<Node> &root){

    for(Node n : root){

        std::cout << n.name;

        if(!n.children.empty()){

            std::cout << ", children: ";

            for(std::string c : n.children) {

                std::cout << c << " ";

            }

        }

        if(!n.parents.empty()){

            std::cout << ", parents: ";

            for(std::string p : n.parents){

                std::cout << p << " ";

            }

        }

        std::cout << std::endl;

    }

}

void CoffmanGraham(std::vector<Node> &root){

    std::vector<std::string> labels;
    std::vector<int> lexicoComp;
    int oj = 0;
    int index = 0;
    int moment = 0;
    bool allChildrenTaken = false;

    for(int i = 0; i < root.size(); i++){

        if(root.at(i).children.empty()){

            root.at(i).isTaken = true;
            labels.push_back(root.at(i).name);

        }

    }

    int indexOfLabel = 0;

    for(int iteration = 0; iteration < root.size(); iteration++){

        for(int i = 0; i < root.size(); i++){

            allChildrenTaken = false;

            for(int j = 0; j < root.at(i).children.size(); j++){

                if(std::find(labels.begin(), labels.end(), root.at(i).children.at(j)) != labels.end() && !root.at(i).isTaken){

                    allChildrenTaken = true;

                }
                else{

                    allChildrenTaken = false;
                    break;

                }

            }

            if((allChildrenTaken == true) && (!root.at(i).isTaken)){

                for(int k = 0; k < root.at(i).children.size(); k++){

                    auto it = std::find(labels.begin(), labels.end(), root.at(i).children.at(k));

                    // If element was found
                    if(it != labels.end()){

                        int index = it - labels.begin();

                        auto id = std::find(root.at(i).s_list.begin(), root.at(i).s_list.end(), index + 1);

                        if(id == root.at(i).s_list.end()){

                            root.at(i).s_list.push_back(index + 1);
                            sort(root.at(i).s_list.begin(), root.at(i).s_list.end(), std::greater<int>());

                        }


                    }
                    else{

                        //std::cout << "Nicośc" << std::endl;

                    }

                }

            }

        }

        lexicoComp.clear();
        lexicoComp.push_back(10000);
        oj = 0;

        for(int l = 0; l < root.size(); l++){

            if(!root.at(l).s_list.empty() && !root.at(l).isTaken){

                auto const firstSmaller = std::lexicographical_compare(begin(lexicoComp), end(lexicoComp), begin(root.at(l).s_list), end(root.at(l).s_list));

                if(!firstSmaller){

                    lexicoComp.clear();
                    lexicoComp = root.at(l).s_list;
                    indexOfLabel = l;
                    oj = 420;

                }

            }

        }

        if(oj != 0){

            root.at(indexOfLabel).isTaken = true;
            labels.push_back(root.at(indexOfLabel).name);

        }

        moment = iteration;

    }    

    for(int i = 0; i < root.size(); i++){

        std::cout << i+1 << " -> " << labels.at(i) << "\n";

    }

    /*

    std::cout << std::endl;

    std::cout << "\n\n-----------------------------------MACHINES-----------------------------------\n\n   ";

    for(int i = 1; i <= moment/2; i++){

        std::cout << i << "  ";

    }

    std::cout << "\n";

    std::cout << "M" << 1 << " ";

    for(int j = labels.size()-1; j > -1; j -= 1){

        std::cout << labels.at(j) << " ";

    }

    std::cout << "M" << 1 << " ";

    for(int j = labels.size()-1; j > -1; j -= 1){

        std::cout << labels.at(j) << " ";

    }

    std::cout << "\n";
    */

}

int main(){

    std::vector<Node> nodes;

    ReadNetwork(nodes);

    PrintNetwork(nodes);

    CoffmanGraham(nodes);

    std::cout << "\n\n";

    for(Node n : nodes){

        std::cout << n.name << " (";

            for(int c : n.s_list) {

                std::cout << c << ", ";

            }

        std::cout << ") " << n.isTaken << std::endl;

    }

    return 0;
}

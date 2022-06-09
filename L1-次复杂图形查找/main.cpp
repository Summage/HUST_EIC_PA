#include "Tree.h"
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>

static const int boardSize = 3;
#define get_x(x) (x%boardSize)
#define get_y(y) (y/boardSize)
using namespace std;
//是否允许顺序完全相反的图案存在
#define RE_ENABLE false
vector<string> slope{string("1"), string("-1"), string("2"), string("-2"),
                     string("1/2"), string("-1/2"), string("0"), string("inf")};

class Series{
public:
    int vtx[boardSize*boardSize]{};
    int flag_reuse;
    bool slope_used[8]{};
    int index;
    int count_slope;
    explicit Series(int pos){
        flag_reuse = -1;
        for(int i = 0; i < 8; slope_used[i++] = false);
        count_slope = index = 0;
        vtx[index] = pos;
    }
    ~Series()=default;
    //进行下一步，更新边计数，当前节点，id(_re)以及shift(_re)
    bool forward(int pos, int slope_index){
        //最复杂
//        if (slope_used[slope_index])
//            return false;
        if(slope_used[slope_index]) {
            if(flag_reuse != -1)
                return false;
            flag_reuse = slope_index;
        }else {
            slope_used[slope_index] = true;
            count_slope++;
        }
        vtx[++index] = pos;
        return true;
    }
    void backward(int pos, int slop_index){
        if(flag_reuse == slop_index)
            flag_reuse = -1;
        else {
            slope_used[slop_index] = false;
            count_slope--;
        }
        index--;
    }
    void init(int pos){
        flag_reuse = -1;
        for(int i = 0; i < 8; slope_used[i++] = false);
        count_slope = index = 0;
        vtx[index] = pos;
    }
};

//可交互环境
class Locker{
public:
    bool availble[boardSize * boardSize]{};
    Locker(){for(int i = 0; i < 9; availble[i++] = true);}
    ~Locker()=default;

    void clear(){for(int i = 0; i < 9; availble[i++] = true);}
    void forward(int vertx){availble[vertx] = false;}
    void backward(int vertx){availble[vertx] = true;}
    int get_slope(int A, int B){
        if (!availble[B]) return -1;
        int edge[2]{(get_x(B) - get_x(A)), (get_y(B) - get_y(A))};
        if (edge[0] == 0){ //无横向移动
            //纵移两格并且中间未连接
            if ((edge[1] == 2 || edge[1] == -2) && availble[min(A, B) + boardSize])
                return -1;
            return 7;
        }else if (edge[0] == 1 || edge[0] == -1){
            switch((int)(edge[1] / edge[0])){
                case 0: return 6;
                case 1: return 0;
                case -1: return 1;
                case 2: return 2;
                case -2: return 3;
            }
        }else{
            switch((int)((2*edge[1]) / edge[0])){
                case 0:
                    if(availble[min(A, B) + 1])
                        return -1;
                    return 6;
                case 2:
                    if(availble[4])
                        return -1;
                    return 0;
                case -2:
                    if(availble[4])
                        return -1;
                    return 1;
                case 1: return 4;
                case -1: return 5;
            }
        }
        return -1;
    }
};

void dfs(ofstream &fp, vector<int>& vertex,Locker * locker, Series * series, int * counter){
    int vecSize = vertex.size();
    int poppedVtx, currentVtx = series->vtx[series->index], slopeIndex;
    if (vecSize == 0){
        if (series->count_slope != 7 || series->flag_reuse == -1)
            return;
        for(int i = 0; i < 8;)
            fp << series->vtx[i++] << ',';
        fp << series->vtx[8] << endl;
        (*counter)++;
        return;
    }
    for(int i = 0; i < vecSize; i++){
        poppedVtx = vertex[i];
        slopeIndex = locker->get_slope(currentVtx, poppedVtx);
        if(slopeIndex == -1)
            continue;

        if(!series->forward(poppedVtx, slopeIndex))
            continue;
        locker->forward(poppedVtx);
        vertex.erase(vertex.begin()+i);

        dfs(fp, vertex, locker, series, counter);
        vertex.insert(vertex.begin()+i, poppedVtx);
        series->backward(poppedVtx, slopeIndex);
        locker->backward(poppedVtx);
    }
}

int main() {
    AVLTree tree;
    vector<int> vertex{0,1,2,3,4,5,6,7,8};
    int counter = 0;
    Locker locker;
    Series series(0);
    ofstream fp;
    fp.open("res.txt");
    for(int i = 0; i < 9; i++){
        locker.clear();
        series.init(i);
        vertex.erase(vertex.begin() + i);
        locker.forward(i);
        dfs(fp, vertex, &locker, &series, &counter);
        vertex.insert(vertex.begin()+i, i);
        locker.backward(i);
    }
    cout << counter;
    fp.close();
    return 0;
}


//涉及自己写的平衡树，主要是方便查找和机构化展示
//#include "Tree.h"
//#include <iostream>
//#include <stack>
//#include <vector>
//#include <string>
//#include <cassert>
//#include <fstream>
//
//static const int boardSize = 3;
//#define get_x(x) (x%boardSize)
//#define get_y(y) (y/boardSize)
//using namespace std;
////是否允许顺序完全相反的图案存在
//#define RE_ENABLE false
//vector<string> slope{string("1"), string("-1"), string("2"), string("-2"),
//                     string("1/2"), string("-1/2"), string("0"), string("inf")};
//
//class Series{
//public:
//    int64_t id, id_re, mask = 0xf;  // 将节点序列映射到一个int的不同位上进行编码,方便检索
//    int shift, shift_re, count;
//    int flag_reuse;
//    bool slope_used[8]{};
//    int current_pos = 0;
//    explicit Series(){
//        count = 0;
//        flag_reuse = -1;
//        for(int i = 0; i < 8; slope_used[i++] = false);
//        id = id_re = 0;
//        shift_re = 0;
//        shift = 4 * (boardSize * boardSize - 1);
//    }
//    ~Series()=default;
//    //进行下一步，更新边计数，当前节点，id(_re)以及shift(_re)
//    void forward(int64_t pos){
//        count ++;
//        current_pos = (int)pos;
//        id += (pos << shift);
//        id_re += (pos << shift_re);
//        shift -= 4;
//        shift_re += 4;
//    }
//    void backward(int pos){
//        count --;
//        current_pos = pos;
//        shift += 4;
//        shift_re -= 4;
//        id -= (id & (mask << shift));
//        id_re -= (id_re & (mask << shift_re));
//    }
//    void init(int pos){
//        current_pos = pos;
//        count = 0;
//        flag_reuse = -1;
//        for(int i = 0; i < 8; slope_used[i++] = false);
//        id = id_re = 0;
//        shift_re = 0;
//        shift = 4 * (boardSize * boardSize - 1);
//    }
//    static void id2str(uint64_t x, int res[]){
//        uint64_t mask = 0xf;
//        int shift = 0;
//        for(int i = (boardSize * boardSize); i-- > 0; shift += 4)
//            res[i] = (int)((x & (mask<<shift)) >> shift);
//    }
//};
//
////可交互环境
//class Locker{
//public:
//    bool availble[boardSize * boardSize]{};
//    Locker(){
//        for(int i = 0; i < 9; availble[i++] = true);
//    }
//    ~Locker()=default;
//    void clear(){
//        for(int i = 0; i < 9; availble[i++] = true);
//    }
//    void forward(int vertx){
//        availble[vertx] = false;
//    }
//    void backward(int vertx){
//        availble[vertx] = true;
//    }
//    int get_slope(int A, int B){
//        if (!availble[B]) return -1;
//        int edge[2]{(get_x(B) - get_x(A)), (get_y(B) - get_y(A))};
//        if (edge[0] == 0){
//            if ((edge[1] == 2 || edge[1] == -2) && availble[min(A, B) + boardSize])
//                return -1;
//            return 7;
//        }else if (edge[0] == 1 || edge[0] == -1){
//            switch((int)(edge[1] / edge[0])){
//                case 0:
//                    return 6;
//                case 1:
//                    return 0;
//                case -1:
//                    return 1;
//                case 2:
//                    return 2;
//                case -2:
//                    return 3;
//                default:
//                    assert(0);
//            }
//        }else{
//            switch((int)((2*edge[1]) / edge[0])){
//                case 0:
//                    return 6;
//                case 2:
//                    if(availble[4])
//                        return -1;
//                    return 0;
//                case -2:
//                    if(availble[4])
//                        return -1;
//                    return 1;
//                case 1:
//                    return 4;
//                case -1:
//                    return 5;
//                default:
//                    assert(0);
//            }
//        }
//        return -1;
//    }
//};
//
//void dfs(AVLTree * tree, vector<int>& vertex,Locker * locker, Series * series, int * counter_dul, int * counter_unq){
//    int vecSize = vertex.size();
//    int poppedVtx, currentVtx = series->current_pos, slopeIndex;
//    if (vecSize == 0){
//        if (series->count != 8)
//            return;
////        if (tree->find(min(series->id, series->id_re)) == nullptr){
////            tree->insert(min(series->id, series->id_re));
////            (*counter_unq)++;
////        }
//        if (tree->find(series->id) == nullptr){
//            tree->insert(series->id);
//            (*counter_unq)++;
//        }
//        (*counter_dul)++;
//        cout << *counter_dul << '\t' << series->id << '\t' << series->id_re << endl;
//        return;
//    }
//    for(int i = 0; i < vecSize; i++){
//        poppedVtx = vertex[i];
//        slopeIndex = locker->get_slope(currentVtx, poppedVtx);
//        //非法连线
//        if(slopeIndex == -1)
//            continue;
//        else if(series->slope_used[slopeIndex]){
//            if (series->flag_reuse == -1)
//                series->flag_reuse = slopeIndex;
//            else
//                continue;
//        }else{
//            series->slope_used[slopeIndex] = true;
//        }
//        //else
//        series->forward(poppedVtx);
//        locker->forward(poppedVtx);
//        vertex.erase(vertex.begin()+i);
//
//        dfs(tree, vertex, locker, series, counter_dul, counter_unq);
//
//        if(series->flag_reuse == slopeIndex)
//            series->flag_reuse = -1;
//        else
//            series->slope_used[slopeIndex] = false;
//        vertex.insert(vertex.begin()+i, poppedVtx);
//        series->backward(currentVtx);
//        locker->backward(poppedVtx);
//    }
//}
//
//void save(ofstream& fp, int res[]){
//    for(int i = 0; i < 8;)
//        fp << res[i++] << ',';
//    fp << res[8] << endl;
//}
//
//int main() {
//    AVLTree tree;
//    vector<int> vertex{0,1,2,3,4,5,6,7,8};
//    int counter_dul = 0, counter_unq = 0;
//    Locker locker;
//    Series series;
//    for(int i = 0; i < 9; i++){
//        locker.clear();
//        series.init(i);
//        vertex.erase(vertex.begin() + i);
//        dfs(&tree, vertex, &locker, &series, &counter_dul, &counter_unq);
//        vertex.insert(vertex.begin()+i, i);
//    }
//    ofstream fp;
//    fp.open("res.txt");
//    int res[9];
//    //平衡树，成功的话这里不用检查
////    if(tree.root == nullptr || tree.root->left == nullptr)
////        return -1;
//    Block_AVLTree * cursor = tree.root->left;
//    stack<Block_AVLTree*> nodeStack;
//    nodeStack.push(tree.root);
//    while(cursor){
//        if(cursor->LDepth){
//            nodeStack.push(cursor);
//            cursor = cursor->left;
//        }else{
//            Series::id2str(cursor->key, res);
//            save(fp, res);
//            if(cursor->RDepth){
//                cursor = cursor->right;
//            }else{
//                cursor = nodeStack.top();
//                nodeStack.pop();
//            }
//        }
//    }
//    fp.close();
//    return 0;
//}
//

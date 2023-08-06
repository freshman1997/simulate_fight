#ifndef __HEAP_H__
#define __HEAP_H__

#include <functional>
#include <vector>

template<class T, class less_func = std::less<T>>
class Heap
{
typedef less_func Cmp;
public:
    Heap();

public:
    void push(T);
    T pop();
    T peek() { return datas[0]; }
    size_t size() const { return datas.size(); }

private:
    int left_child(int idx) { return 2 * idx + 1; }
    int right_child(int idx) { return 2 * idx + 2; }
    int parent(int idx) { return (idx - 1) / 2; }

    void shift_up(int);      // 数据上浮
    void shift_down(int);    // 数据下沉

    Cmp cmp;
    std::vector<T> datas;
};

//----------------------------------- impl --------------------------------------
template<class T, class less_func>
Heap<T, less_func>::Heap() : cmp(std::less<T>())
{}

template<class T, class less_func>
void Heap<T, less_func>::push(T t)
{
    datas.push_back(t);
    shift_up(datas.size() - 1);
}

template<class T, class less_func>
void Heap<T, less_func>::shift_up(int index)
{
    while ( index > 0 && cmp(datas[parent(index)], datas[index])){
        std::swap(datas[index], datas[parent(index)]);
        index = parent(index);
    }
}

template<class T, class less_func>
void Heap<T, less_func>::shift_down(int index)
{
    int j = left_child(index);
    while (j < datas.size()){
        // 右节点大于左节点元素的值
        if(j + 1 < datas.size() && !cmp(datas[j + 1], datas[j]))
            // j 的值就是右孩子的下标
            j++;

        // 否则的话，上边的条件不成立，j没有++操作，index这个位置的元素比它的两个孩子节点的元素的值都要大，下沉结束
        if(!cmp(datas[index], datas[j]))
            break;

        std::swap(datas[index], datas[j]);
        index = j;
        j = left_child(index);
    }
}

template<class T, class less_func>
T Heap<T, less_func>::pop()
{
    T tmp = datas[0];
    std::swap(datas[0], datas[datas.size() - 1]);
    datas.erase(datas.begin() + datas.size() - 1);
    shift_down(0);
    return tmp;
}

#endif
// 多重ソートの描き方忘れるので備忘
#include <iostream>
#include <algorithm>

using namespace std;

struct Test{
    int age;
    int height;
};

int main(){
    Test test[] = 
    {
        {3, 2},
        {1, 3},
        {1, 2}
    };

    int size = sizeof(test) / sizeof(Test);

    sort(test, test + size, [](auto const  &lhs, auto const &rhs){
        if(lhs.age == rhs.age) return(lhs.height < rhs.height);
        return(lhs.age < rhs.age);
    });

    for(auto i = 0; i < size; i++) printf("%d, age = %d, height = %d\n", i, test[i].age, test[i].height);

    return 0;
}

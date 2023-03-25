#include "GG_vector_test.h"
#include "GG_test_utils.h"
#include <cassert>

__GG_BEGIN_NAMESPACE

namespace GGTest {
    void testVectorCase1() {
        std::vector<std::string> v1(10,"kzw");
        vector<std::string> v2(10,"kzw");
        assert(container_equal(v1,v2));

        std::vector<std::string> v3(10);
        vector<std::string> v4(10);
        assert(container_equal(v3,v4));

        std::array<std::string,3> arr = {"abc","def","ghi"};
        std::vector<std::string> v5(std::begin(arr),std::end(arr));
        vector<std::string> v6(std::begin(arr),std::end(arr));
        assert(container_equal(v5,v6));
    }

    void testVectorCase2(){
        std::vector<int> temp1(10, 0);
        vector<int> temp2(10, 0);
        auto v1(temp1);
        auto v2(temp2);
        assert(container_equal(v1, v2));

        auto v3(std::move(temp1));
        auto v4(std::move(temp2));
        assert(container_equal(v3, v4));

        auto v5 = v1;
        auto v6 = v2;
        assert(container_equal(v5, v6));

        auto v7 = std::move(v3);
        auto v8 = std::move(v4);
        assert(container_equal(v7, v8));
    }
    void testVectorCase3(){
        vector<int> v1,v2;
        for(int i=0; i<100;i++) {
            v1.push_back(i);
            v2.push_back(i);
        }
        assert(v1 == v2);
        assert(!(v1 != v2));
    }
    void testVectorCase4(){
        vector<int> vec;
        for(int i=1; i<= 5; i++)
            vec.push_back(i);
        auto i = 1;
        for(vector<int>::iterator iter=vec.begin(); iter != vec.end(); iter++)
            assert(* iter == i);
        i = 1;
        for(vector<int>::const_iterator iter=vec.cbegin(); iter != vec.cend(); iter++) 
            assert(*iter == i);
        
    }
    void testVectorCase5(){
        vector<int> vec(5);
        int i=0;
        vector<int>::reverse_iterator riter = vec.rbegin();
        for(; riter != vec.rend(); ++riter) 
            *riter = ++i;
        i = 5;
        for(vector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter,--i)
            assert(*iter == i);
        i = 1;
        for(vector<int>::reverse_iterator iter = vec.rbegin(); iter != vec.rend(); ++iter,++i)
            assert(*iter == i);
    }
    void testVectorCase6(){
        vector<int> v(11,0);
        assert(v.size() == 11);

        v.resize(5);
        assert(v.size() == 5);

        v.resize(20);
        assert(v.size() == 20);
    }
    void testVectorCase7(){
        vector<int> v;
        v.reserve(20);
        assert(v.capacity() == 20);
    }
    void testVectorCase8(){
        vector<int> v1(10);
        vector<int> v2(10);
        for( unsigned i=0; i<10; i++) {
            v1[i] = i;
            v2[i] = i;
        }
        assert(container_equal(v1, v2));
        v1.front() = 99;
        v2.front() = 99;
        v1.back() = 100;
        v2.back() = 100;

        assert(container_equal(v1, v2));
    }

    void testVectorCase9(){
        std::vector<std::string> v1;
        vector<std::string> v2;
        v1.push_back("hello "); v1.push_back("world");
        v2.push_back("hello "); v2.push_back("world");
        assert(container_equal(v1, v2));

        v1.pop_back();
        v2.pop_back();
        assert(container_equal(v1, v2));   
    }
    void testVectorCase10(){
        std::vector<int> v1;
        vector<int> v2;
        v1.insert(v1.begin(), 0);
        v2.insert(v2.begin(), 0);
        assert(container_equal(v1, v2));

        v1.insert(v1.end(), 1);
        v2.insert(v2.end(), 1);
        assert(container_equal(v1, v2));

        int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        v1.insert(v1.end(), std::begin(arr), std::end(arr));
        v2.insert(v2.end(), std::begin(arr), std::end(arr));
        assert(container_equal(v1, v2));

    }
    void testVectorCase11(){
        std::vector<int> v1;
        vector<int> v2;
        for(int i=1; i<=10; i++) {
            v1.push_back(i);
            v2.push_back(i);
        }
        v1.erase(v1.begin()+5);
        v2.erase(v2.begin()+5);
        assert(container_equal(v1, v2));

        v1.erase(v1.begin(), v1.begin()+3);
        v2.erase(v2.begin(), v2.begin()+3);
        assert(container_equal(v1, v2));
    }
    void testVectorCase12(){
        vector<int> foo(3, 100);
        vector<int> bar(2, 200);

        assert(!(foo == bar));
        assert(foo != bar);
    }

    void testVectorAllCases() {
        testVectorCase1();
        testVectorCase2();
        testVectorCase3();
        testVectorCase4();
        testVectorCase5();
        testVectorCase6();
        testVectorCase7();
        testVectorCase8();
        testVectorCase9();
        testVectorCase10();
        testVectorCase11();
        testVectorCase12();
    }
}

__GG_END_NAMESPACE

int main(){
    gadget::GGTest::testVectorAllCases();
}
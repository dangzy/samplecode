#include <gmock/gmock.h>
using testing::Return;
int Factorial( int n )  
{  
    return n<=0? 1 : n*Factorial(n - 1);  
}
TEST(TestFactorial, ZeroInput)
{  
    EXPECT_EQ(1, Factorial(0));
} 
TEST(TestFactorial, OtherInput)  
{  
    EXPECT_EQ(1, Factorial(1));  
    EXPECT_EQ(2, Factorial(2));  
    EXPECT_EQ(6, Factorial(3));  
    EXPECT_EQ(40320, Factorial(8));  
}
class my_mock{
    public:
        MOCK_METHOD0(mod_ctl,int());
};
TEST(TestMock, NoInput)
{
    my_mock mm;
    int rc=3;
    ON_CALL(mm, mod_ctl()).WillByDefault(Return(rc));
    EXPECT_CALL(mm,mod_ctl()).Times(1);
    EXPECT_EQ(3,mm.mod_ctl());
}
class code_class{
    public:
        virtual int open(int)=0;
        virtual int close(int a){return a*a;}
};
class my_mock2: public code_class{
    public:
        MOCK_METHOD1(open,int(int));
};
TEST(TestMock, InputFD)
{
    my_mock2 mm;
    ON_CALL(mm, open(2)).WillByDefault(Return(7));
    EXPECT_CALL(mm,open(2))
        .Times(1)
        .WillOnce(Return(7));
    EXPECT_EQ(7,mm.open(2));
    EXPECT_EQ(9,mm.close(3));
}

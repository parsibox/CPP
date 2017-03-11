#include<stdio.h>

class Parent
{
  public:
   virtual void function1(){
     printf("Parent::function1\n");
  }
  virtual void function2()=0;
};
class child1:public Parent
{
 public:
   void function3()
    {
      printf("child1::function2\n");
   } 
};

class child2:public child1
{
  public:
     void function1(){
     printf("child2::function1\n");
    }
     void function2()
     {
      printf("child2::function2\n");
     }
};

int main()
{
	Parent *P ;/*= new Parent();
	P->function1();*/
	/*P = new child1();
	P->function1();
	P->function2();
	delete P;*/
	P = new child2();
	P->function1();
	P->function2();
	delete P;

}

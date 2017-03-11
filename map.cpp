#include<iostream>
#include<map>
#include<string>
int main()
{
    std::map<std::string,int> Map;
       Map.insert(std::pair<std::string,int>("#",20));
    for(int i=10;i>=0;i--)
    {
       char number[10]={0x00};
       sprintf(number,"%d",i);
       Map.insert(std::pair<std::string,int>(number,i));
    }
       Map.insert(std::pair<std::string,int>("*",21));
    std::map<std::string,int>::iterator itr = Map.begin(); 
    while(itr!=Map.end())
    {
          std::cout<<"Key:"<<itr->first<<",value:"<<itr->second<<std::endl;
          itr++;
    }
  return 0;
}
